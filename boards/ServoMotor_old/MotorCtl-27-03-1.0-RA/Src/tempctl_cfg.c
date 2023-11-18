#include "tempctl_cfg.h"
#include "ad5245.h"
#include "stm32f4xx_hal.h"
#include "pb7_43.h"

extern ADC_HandleTypeDef hadc2;

static u8 sensor0_db_read(void);
static void hd0_open(u8 open);
static u8 target0_over(f32 current);
static void sensor0_db_write(u8 val);
static void temp0_hd_init(void);
static f32 adjust0_to_code(f32 adjust);
static float read_r_value(void);
static f32 temperature_read(u8 id);
static f32 temperature_buffer_read(u8 id);
static Pb7_43_t *sensor;

//制冷、加热
Temp_thread_t temperature[TEMP_CTRL_CNT] = {
    {
         .is_start = 1,
         .lowest_power = 255,
         .pid_cfg.type = POSITION,
         .pid_cfg.target = 45,
         .pid_cfg.kp = 200,
         .pid_cfg.ki = 10,
         .pid_cfg.kd = 50,
         .pid_cfg.limit = 255,
         .pid_cfg.separate_i_range = 2
    }
};


const f32 temperature_range[] = {-30, 70};

                             
void temp_hd_init(void) {
    temp0_hd_init();
}


static void temp0_hd_init(void) {
    Temp_thread_t *self = &temperature[0];
    self->iic_id = ad5245_create(PD7, PD6);
    power_set_lowest(self);
//     // 打开LM控制开关
    gpio_config(PB3, eGPIO_OUT_PP_NP, 0);
	
    // 打开加热控制开关
    gpio_config(PD4, eGPIO_OUT_PP_NP, 0);
	
    HAL_ADC_Start(&hadc2);
    sensor = pb7_43_new(read_r_value);
    self->pid = pid_mod_new(self->pid_cfg);
    self->convert_code = adjust0_to_code;
    self->temperature_read = temperature_read;
    self->read_buffer_temperature = temperature_buffer_read;
    self->hd_open = hd0_open;
    self->target_over = target0_over;
}

/**
 * @brief 将pid输出的功率转换成实际电压，注：adjust代表实际pid功率
 *
 * @param adjust pid输出的功率
 * @return code
 */
static f32 adjust0_to_code(f32 adjust) {
    Temp_thread_t *self = &temperature[0];
    f32 min_voltage = 1.79;
    f32 max_voltage = 12.9;
    //转化为电压 由电子提供公式
    f32 voltage = max_voltage * (adjust)/self->pid_cfg.limit;
    voltage = voltage < min_voltage ? min_voltage : (voltage > max_voltage ? max_voltage : voltage);
    //转化成code
    return 147.9936 / (voltage - 1.23) - 7.68;
}

/**
 * @brief  关闭温控线程，这里采用输出最低功率而不是关闭总温控开关
 * 1. 关闭总开关会导致液面探测误触发
 * 2.
 * 输出最低功率仍有功率输出，当外界温度与目标温度接近时，会导致最终温度高于目标温度
 * 但此处用于试剂针温控，目标温度>40摄氏度，高于外界使用温度，故不会有问题。
 *
 * @return none
 */
static void hd0_open(u8 open){
	

//     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, open ? GPIO_PIN_SET : GPIO_PIN_RESET);
    // 为防止干扰液面探测，此处选择将功率调整至最低来关闭温度设置
    power_set_lowest(&temperature[0]);
	
	
}

/**
 * @brief 当前温度是否超过阈值，加热时为 > ， 制冷时为 <
 * 
 * @param current 当前温度
 * @return u8 0 未超过 1 超过
 */
static u8 target0_over(f32 current){
    return current > temperature[0].pid->entity.cfg.target;
}

/**
 * @brief 计算热敏电阻组织
 * 
 * @return float 电阻阻值
 */
static float read_r_value(void) {
    HAL_ADC_Start(&hadc2);
    u16 ad = ADC2->DR;
	//printf("resd = %f\r\n",ad*10.0/(204800.0/33-ad));
    return  ad*10.0/(204800.0/33-ad);
}

/**
 * @brief 读取温度值 
 * 
 * @return f32 当前温度值
 */
static f32 temperature_read(u8 id){
    return sensor->read_temperature(sensor);
}

/**
 * @brief 读取温度缓存值，对于PB7_43温度传感器来说读取非耗时操作，可以直接读取温度值
 * 
 * @return f32 温度缓存值
 */
static f32 temperature_buffer_read(u8 id){
    return sensor->read_temperature(sensor);
}
