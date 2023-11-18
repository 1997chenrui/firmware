#include "ds18b20.h"
#include "delayus.h"
#include "tool.h"
#include <stdio.h>
#include <stdlib.h>

#define TEMP_INVALID -274.0f

// 带有{可防止不成对出现
#define TEMP_READ_PEND(self)                                                   \
    {                                                                          \
        bus_os_pend_sem(&self->mutex, 0)
#define TEMP_READ_POST(self)                                                   \
    bus_os_post_sem(&self->mutex);                                             \
    }

static u8 reset(Ds_sensor_t *self);
static void write_byte(Ds_sensor_t *self, u8 Dat);
static u8 read_byte(Ds_sensor_t *self);
static u8 crc8(u8 *pBuf, u8 iSize);
static float read_data(Ds_sensor_t *self);
static float read_temperature_try(Ds_sensor_t *self, int time, int ms);
static float read_temperature(Ds_sensor_t *self);
static float read_temperature_median_filter(Ds_sensor_t *self);
static float read_temperature_buffer(Ds_sensor_t *self);

Ds_sensor_t * ds_sensor_new(void (*set)(u8 val), u8 (*get)()) {
    Ds_sensor_t *self = malloc(sizeof(Ds_sensor_t));
    memset(self, 0, sizeof(Ds_sensor_t));
    self->mfunc_get_dq = get;
    self->mfunc_set_dq = set;
    self->read_temperature = read_temperature;
	self->read_temperature_median_filter = read_temperature_median_filter;
    self->read_buffer_temperature = read_temperature_buffer;
    bus_os_create_sem(&self->mutex, 1);
    return self;
}

static u8 reset(Ds_sensor_t *self) {
    self->mfunc_set_dq(1);
    delayUs(1);
    self->mfunc_set_dq(0);
    delayUs(500); // 500us
    self->mfunc_set_dq(1);
    delayUs(100); // 40us
    return self->mfunc_get_dq();
}

static void write_byte(Ds_sensor_t *self, u8 Dat) {
    for (u8 i = 8; i > 0; i--) {
        self->mfunc_set_dq(0);
        delayUs(5); // 5us
        if (Dat & 0x01)
            self->mfunc_set_dq(1);
        else
            self->mfunc_set_dq(0);
        delayUs(55); // 65us
        self->mfunc_set_dq(1);
        delayUs(2); // 1us
        Dat >>= 1;
    }
}

static u8 read_byte(Ds_sensor_t *self) {
    u8 i, Dat;
    self->mfunc_set_dq(1);
    delayUs(5);
    for (i = 8; i > 0; i--) {
        Dat >>= 1;
        self->mfunc_set_dq(0);
        delayUs(3); // 5us
        self->mfunc_set_dq(1);
        delayUs(5); // 5us
        if (self->mfunc_get_dq())
            Dat |= 0x80;
        else
            Dat &= 0x7f;
        delayUs(50); // 65us
    }
    return Dat;
}

static u8 crc8(u8 *pBuf, u8 iSize) {
    u8 iCrc = 0;
    for (; iSize--;) {
        u8 i;
        iCrc ^= *pBuf++;
        for (i = 0; i < 8; i++) {
            if (iCrc & 0x01)
                iCrc = (iCrc >> 1) ^ 0x8c;
            else
                iCrc >>= 1;
        }
    }
    return iCrc;
}

static float read_data(Ds_sensor_t *self) {
    u8 i;
    TempSensorRAM_TypeDef TempSensorRAM;
    float Temperature;
    // StartConvert
    if (reset(self)) {
        printf("Reset_DS18B20 Error\r\n");
        return TEMP_INVALID;
    }
    delayUs(100); // 40us
    write_byte(self, SkipROM);
    write_byte(self, StartConvert);
    // Wait Convert
    bus_os_sleep(750);
    if (reset(self)) {
        printf("18b20 reset err\r\n");
        return TEMP_INVALID;
    }
    delayUs(100); // 40us
    write_byte(self, SkipROM);
    write_byte(self, ReadScratchpad);
    for (i = 0; i < DS18B20_RAM_SIZE; i++) {
        *((u8 *)(&TempSensorRAM) + i) = read_byte(self);
    }
    if (crc8((u8 *)(&TempSensorRAM), DS18B20_RAM_SIZE)) {
        printf("18b20 crc err\r\n");
        return TEMP_INVALID;
    }
    if (0xf8 == (0xf8 & TempSensorRAM.TempHigh))
        Temperature = (float)(((((TempSensorRAM.TempHigh & 0x0f) << 4) |
                                ((TempSensorRAM.TempLow & 0xf0) >> 4)) -
                               0xff) +
                              (-(1 - (TempSensorRAM.TempLow & 0x0f) *
                                         0.0625))); //除16  把小数部分剔除
    else
        Temperature = (float)((((TempSensorRAM.TempHigh & 0x07) << 4) |
                               ((TempSensorRAM.TempLow & 0xf0) >> 4)) +
                              (TempSensorRAM.TempLow & 0x0f) *
                                  0.0625); //除16  把小数部分剔除

    self->current_temp = Temperature;
    return Temperature;
}

static float read_temperature_try(Ds_sensor_t *self, int time, int ms) {
    float temp = TEMP_INVALID;
    u8 i;
    for (i = 0; i < time; i++) { // 读time次，读到正确跳出
        temp = read_data(self);
        if (temp != TEMP_INVALID) {
            break;
        } else {
            OSTimeDly(ms);
        }
    }
	return temp>=TEMP_INVALID+1?temp*10/10:TEMP_INVALID;
}

static float read_temperature(Ds_sensor_t *self) {
    self->current_temp = read_temperature_try(self, 3, 500);
    return self->current_temp;
}

static float read_temperature_median_filter(Ds_sensor_t *self) {
    float temp_array[3]={0.0f,0.0f,0.0f};
    TEMP_READ_PEND(self);
    for (int i = 0; i < 3; i++) {
        temp_array[i] = read_temperature(self);
    }
    TEMP_READ_POST(self);
    self->current_temp =
        median_filter_float(temp_array, sizeof(temp_array) / sizeof(float));
    return self->current_temp;
}

static float read_temperature_buffer(Ds_sensor_t *self) { return self->current_temp; }
