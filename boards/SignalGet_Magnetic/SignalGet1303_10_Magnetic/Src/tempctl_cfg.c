#include "tempctl_cfg.h"
#include "ad5245.h"
#include "stm32f4xx_hal.h"
#include "ds18b20.h"

static u8 sensor0_db_read(void);
static void sensor0_db_write(u8 val);
static u8 target0_over(f32 current);
static void hd0_open(u8 open);
static void temp0_hd_init(void);
static f32 adjust0_to_code(f32 adjust);

static u8 sensor1_db_read(void);
static void sensor1_db_write(u8 val);
static u8 target1_over(f32 current);
static void hd1_open(u8 open);
static void temp1_hd_init(void);
static f32 adjust1_to_code(f32 adjust);


static u8 sensor2_db_read(void);
static void sensor2_db_write(u8 val);
static u8 target2_over(f32 current);
static void hd2_open(u8 open);
static void temp2_hd_init(void);
static f32 adjust2_to_code(f32 adjust);


static float read_temperature0(void);
static float read_buffer_temperature0(void);

static float read_temperature1(void);
static float read_buffer_temperature1(void);

static float read_temperature2(void);
static float read_buffer_temperature2(void);

static Ds_sensor_t* sensor0;
static Ds_sensor_t* sensor1;
static Ds_sensor_t* sensor2;

//加热
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
    },
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
    },
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
	temp1_hd_init();
	temp2_hd_init();
}


static void temp0_hd_init(void) {
    Temp_thread_t *self = &temperature[0];
    self->iic_id = ad5245_create(PD1, PA6);
    // 打开LM控制开关
    gpio_config(PC10, eGPIO_OUT_PP_UP, 0);
	
    sensor0 = ds_sensor_new(sensor0_db_write, sensor0_db_read);
    self->pid = pid_mod_new(self->pid_cfg);
    self->convert_code = adjust0_to_code;
    self->read_buffer_temperature = read_buffer_temperature0;
    self->temperature_read = read_temperature0;
    self->target_over = target0_over;
    self->hd_open = hd0_open;
}

static void temp1_hd_init(void) {
    Temp_thread_t *self = &temperature[1];
    self->iic_id = ad5245_create(PD4, PD5);
    // 打开LM控制开关
    gpio_config(PC11, eGPIO_OUT_PP_UP, 0);
	
    sensor1 = ds_sensor_new(sensor1_db_write, sensor1_db_read);
    self->pid = pid_mod_new(self->pid_cfg);
    self->convert_code = adjust1_to_code;
    self->read_buffer_temperature = read_buffer_temperature1;
    self->temperature_read = read_temperature1;
    self->target_over = target1_over;
    self->hd_open = hd1_open;
}

static void temp2_hd_init(void) {
    Temp_thread_t *self = &temperature[2];
    self->iic_id = ad5245_create(PD7, PB3);
    // 打开LM控制开关
    gpio_config(PC12, eGPIO_OUT_PP_UP, 0);
	
    sensor2 = ds_sensor_new(sensor2_db_write, sensor2_db_read);
    self->pid = pid_mod_new(self->pid_cfg);
    self->convert_code = adjust2_to_code;
    self->read_buffer_temperature = read_buffer_temperature2;
    self->temperature_read = read_temperature2;
    self->target_over = target2_over;
    self->hd_open = hd2_open;
}

static u8 sensor0_db_read(void){
    return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
}
static void sensor0_db_write(u8 val){
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, (GPIO_PinState)val);
}

static u8 sensor1_db_read(void){
    return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
}
static void sensor1_db_write(u8 val){
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, (GPIO_PinState)val);
}

static u8 sensor2_db_read(void){
    return HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15);
}
static void sensor2_db_write(u8 val){
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15, (GPIO_PinState)val);
}

/**
 * @brief 将pid输出的功率转换成实际电压，注：-adjust代表实际pid功率
 *
 * @param adjust pid输出的功率
 * @return code
 */
static f32 adjust0_to_code(f32 adjust) {
    Temp_thread_t *self = &temperature[0];
    f32 min_voltage = 2.855;
    f32 max_voltage = 14.465;
    //转化为电压 由电子提供公式
    f32 voltage = max_voltage * adjust/self->pid_cfg.limit;
    voltage = voltage < min_voltage ? min_voltage : (voltage > max_voltage ? max_voltage : voltage);
    //转化成code
    return 208.64 / (voltage - 1.63) - 0.256;
}

static f32 adjust1_to_code(f32 adjust) {
    Temp_thread_t *self = &temperature[0];
    f32 min_voltage = 2.855;
    f32 max_voltage = 14.465;
    //转化为电压 由电子提供公式
    f32 voltage = max_voltage * adjust/self->pid_cfg.limit;
    voltage = voltage < min_voltage ? min_voltage : (voltage > max_voltage ? max_voltage : voltage);
    //转化成code
    return 208.64 / (voltage - 1.63) - 0.256;
}
static f32 adjust2_to_code(f32 adjust) {
    Temp_thread_t *self = &temperature[0];
    f32 min_voltage = 2.855;
    f32 max_voltage = 14.465;
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
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, open ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static void hd1_open(u8 open){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, open ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static void hd2_open(u8 open){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, open ? GPIO_PIN_RESET : GPIO_PIN_SET);
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

static u8 target1_over(f32 current){
    return current > temperature[1].pid->entity.cfg.target;
}


static u8 target2_over(f32 current){
    return current > temperature[2].pid->entity.cfg.target;
}



static float read_temperature0(){
    return sensor0->read_temperature_median_filter(sensor0);
}                                                                  

static float read_buffer_temperature0(){
    return sensor0->read_buffer_temperature(sensor0);
}

static float read_temperature1(){
    return sensor1->read_temperature_median_filter(sensor1);
}                                                                  

static float read_buffer_temperature1(){
    return sensor1->read_buffer_temperature(sensor1);
}

static float read_temperature2(){
    return sensor2->read_temperature_median_filter(sensor2);
}                                                                  

static float read_buffer_temperature2(){
    return sensor2->read_buffer_temperature(sensor2);
}
