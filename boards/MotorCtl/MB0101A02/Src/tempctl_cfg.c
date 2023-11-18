#include "tempctl_cfg.h"
#include "ad5245.h"
#include "stm32f4xx_hal.h"
#include "ds18b20.h"

static u8 sensor0_db_read(void);
static void sensor0_db_write(u8 val);
static void temp0_hd_init(void);
static f32 adjust0_to_code(f32 adjust);

static f32 temperature0_read(u8 );
static f32 temperature0_buffer_read(u8 );
static void hd0_open(u8 open);
static u8 target0_over(f32 current);
static Ds_sensor_t* sensor0;

//制冷、加热
Temp_thread_t temperature[TEMP_CTRL_CNT] = {
    {
         .is_start = 1,
         .lowest_power = 255,
         .pid_cfg.type = POSITION,
         .pid_cfg.target = 37,
         .pid_cfg.kp = 200,
         .pid_cfg.ki = 10,
         .pid_cfg.kd = 50,
         .pid_cfg.limit = 255,
         .pid_cfg.separate_i_range = 2
    }
};

                             
void temp_hd_init(void) {
    temp0_hd_init();
}


static void temp0_hd_init(void) {
    Temp_thread_t *self = &temperature[0];
    self->iic_id = ad5245_create(PE0, PE1);
    sensor0 = ds_sensor_new(sensor0_db_write, sensor0_db_read);
    self->pid = pid_mod_new(self->pid_cfg);
    self->convert_code = adjust0_to_code;
    self->read_buffer_temperature = temperature0_buffer_read;
    self->temperature_read = temperature0_read;
    self->hd_open = hd0_open;
    self->target_over = target0_over;
}

static u8 sensor0_db_read(void){
    return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
}

static void sensor0_db_write(u8 val){
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, (GPIO_PinState)val);
}


/**
 * @brief 将pid输出的功率转换成实际电压，注：adjust代表实际pid功率
 *
 * @param adjust pid输出的功率
 * @return code
 */
static f32 adjust0_to_code(f32 adjust) {
    Temp_thread_t *self = &temperature[0];
    f32 min_voltage = 2.8;
    f32 max_voltage = 22;
    //转化为电压 由电子提供公式
    f32 voltage = max_voltage * adjust/self->pid_cfg.limit;
    voltage = voltage < min_voltage ? min_voltage : (voltage > max_voltage ? max_voltage : voltage);
    //转化成code
    return 208.64 / (voltage - 1.63) - 0.256;
}

/**
 * @brief  关闭温控开关
 *
 * @return none
 */
static void hd0_open(u8 open){
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,(GPIO_PinState) open);
}


/**
 * @brief 当前温度是否超过阈值，加热时为 > ， 制冷时为 <
 * 
 * @param current 当前温度
 * @return u8 0 未超过 1 超过
 */
static u8 target0_over(f32 current){
    return current > temperature[0].pid->entity.cfg.target + 0.2;
}

static f32 temperature0_read(u8 idx){
    return sensor0->read_temperature(sensor0);
}

static f32 temperature0_buffer_read(u8 idx){
    return sensor0->read_buffer_temperature(sensor0);
}

void read_buffer_temperature_test(void)
{
	printf("%lf\r\n",sensor0->read_buffer_temperature(sensor0));
}