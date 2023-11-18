#include <stdlib.h>
#include "tct_pressure_singal.h"
#include "tool.h"
/**
 * 包含
    1字节error_code
    2字节持续时间
    2字节结束时压力值
    4字节压力数据长度
 */
#define DATA_HEAD_LEN 9

typedef enum{
    NO_ERROR = 0,
    THRESHOLD_NO_REACH = 1,
    PRESSURE_OUT_OF_RANGE = 2
}ERROR_CODE;

static Pressure_t pressure;
static SENSOR_READ func_sensor_read;
static IO_TIMING_WRITE func_io_timing_write;

static u8 singal_capture(Pressure_Entity_t* entity);
static u16 calc_continue_time(Pressure_Entity_t* entity);
static void result_report(Pressure_Entity_t* entity);
static bool pressure_range_error(Pressure_Cfg_t* cfg, u16 val);
static bool pressure_reach_threshold(Pressure_Cfg_t* cfg, const u16 old, const u16 new);
static void pressure_listen_thread(void* data);
void add(Pressure_Cfg_t* param);
void parse(bus_packet* packet);

/** 外部函数 **/
void pressure_singal_init(void (*thread_create)(void (*thread)(void* data)),
        void (sem_create)(bus_os_sem* sem),
        SENSOR_READ sensor_read,
        IO_TIMING_WRITE io_timing_write){
    Pressure_Cfg_t* cfg = (Pressure_Cfg_t*)malloc(sizeof(Pressure_Cfg_t));
    Pressure_Entity_t* entity = (Pressure_Entity_t*)malloc(sizeof(Pressure_Entity_t));
    memset(cfg, 0, sizeof(Pressure_Cfg_t));
    memset(entity, 0, sizeof(Pressure_Entity_t));
    entity->cfg = cfg;
    pressure.entity = entity;
    pressure.add = add;
    pressure.parse = parse;
    func_sensor_read = sensor_read;
    func_io_timing_write = io_timing_write;
	sem_create(&pressure.sem);
	thread_create(pressure_listen_thread);
}

Pressure_t* pressure_get_as_ref(){
    return &pressure;
}


/** Pressure_Cfg_t 成员函数 **/
/**
 * @Description 解析并设置cfg
 * @Param 
    self
    data 字节流
 * @Return none
 */
static void cfg_parse_set(Pressure_Cfg_t* self, u8* data){
    self->report_result_enable = true;
    int offset = 4;
    u8 id_flag = data[offset++];
    u32 diff = ReadU32(&data[offset]);
    offset+=4;
    u16 time_span = ReadU16(&data[offset]);
    offset+=2;
    u32 error_range = ReadU32(&data[offset]);
    offset+=4;
    u16 listen_time = ReadU16(&data[offset]);
    offset+=2;
    self->io_timing_len = ReadU32(&data[offset]);
    offset+=4;
    self->io_timing_array = (Pressure_IoTiming_t*)malloc(self->io_timing_len * sizeof(Pressure_IoTiming_t));
    for(int i = 0; i < self->io_timing_len; i++){
        self->io_timing_array[i].value = ReadU32(&data[offset]);
        offset+=4;
        self->io_timing_array[i].time_delay = ReadU16(&data[offset]);
        offset+=2;
    }
    self->sensor_idx = id_flag & 0x1f;
    self->report_pressure_enable = id_flag >> 5 & 1;
    self->threshold_enable = id_flag >> 6 & 1;
    self->pressure_range_enable = id_flag >> 7 & 1;
    self->is_pressure_bigger = diff >> 31 & 1;
    self->diff_threthold = diff & 0x7fffffff;
    self->listen_time_span = time_span;
    self->low_pressure = error_range & 0xffff;
    self->high_prsspure = error_range >> 16 & 0xffff;
    self->data_cnt = listen_time / time_span;
}

/**
 * @Description 将源cfg 拷贝到self中
 * @Param
    self
    src源cfg
 * @Return none
 */
static void cfg_copy(Pressure_Cfg_t* self, Pressure_Cfg_t* src){
    self->report_result_enable = src->report_result_enable;
    self->sensor_idx = src->sensor_idx;
    self->report_pressure_enable = src->report_pressure_enable;
    self->threshold_enable = src->threshold_enable;
    self->pressure_range_enable = src->pressure_range_enable;
    self->is_pressure_bigger = src->is_pressure_bigger;
    self->diff_threthold = src->diff_threthold;
    self->listen_time_span = src->listen_time_span;
    self->low_pressure = src->low_pressure;
    self->high_prsspure = src->high_prsspure;
    self->data_cnt = src->data_cnt;
    self->io_timing_array = self->io_timing_array;
    self->io_timing_len = self->io_timing_len;
}

static bool pressure_range_error(Pressure_Cfg_t* self, const u16 val){
    return self->pressure_range_enable && (val > self->high_prsspure || val < self->low_pressure);
}

static bool pressure_reach_threshold(Pressure_Cfg_t* self, const u16 old, const u16 val){
    if(!self->threshold_enable){
        return false;
    }
    u16 diff;
    if(self->is_pressure_bigger){
        diff = val > old ? val - old : 0;
    }else{
        diff = old > val ? old - val : 0;
    }
    printf("diff=%d\r\n", diff);
    return diff > self->diff_threthold;
}

/** Pressure_Entity_t成员函数 **/
/**
 * @Description 清除缓存数据
 * @Param self
 * @Return none
 */
static void entity_clear_set(Pressure_Entity_t* self, const bus_packet* packet){
    self->data_len = DATA_HEAD_LEN;
    self->error_code = NO_ERROR;
    self->pressure = 0;
    if(packet != NULL){
        self->packet = *packet;
        self->packet.data = NULL;
    }
}

/**
 * @Description 结果上报
 * @Param self
 * @Return none
 */
static void result_report(Pressure_Entity_t* self){
    if(self->cfg->report_result_enable){
        u16 offset = 0;
        self->buffer[offset++] = self->error_code;
        WriteU16(&self->buffer[offset], calc_continue_time(self));
        offset+=2;
        WriteU16(&self->buffer[offset], self->pressure);
        offset+=2;
        WriteU32(&self->buffer[offset], self->cfg->report_pressure_enable ? self->data_len : 0);
        offset+=4;
//        printf("data_len = %d, cnt = %d,", self->data_len, self->cfg->data_cnt);
//        for(int i = offset; i< offset+self->cfg->data_cnt*2; i+=2){
//            printf("%d,", ReadU16(&self->buffer[i]));
//        }
//        printf("\r\n");
        bus_packet packet = self->packet;
        bus_ita_return(packet.adapterId,packet.srcId,packet.sessionId,
            self->cfg->report_pressure_enable?self->data_len:DATA_HEAD_LEN, self->buffer);
    }
}

/**
 * @Description 将数据写入缓存
 * @Param 
    self
    val 采集的数据
 * @Return none
 */
static void singal_write_to_buffer(Pressure_Entity_t* self, u16 val){
    WriteU16(&self->buffer[self->data_len], val);
    self->data_len+=2;
}

/**
 * @Description 计算持续时间
 * @Param self
 * @Return 过程的持续时间ms
 */
static u16 calc_continue_time(Pressure_Entity_t* self){
    return self->cfg->listen_time_span * ((self->data_len - DATA_HEAD_LEN)>>1);
}

/**
 * @Description 信号捕获
 * @Param self
 * @Return 错误码
    0 无错误
    1 阈值未触发
    2 压力值不在正常范围
 */
static u8 singal_capture(Pressure_Entity_t* self){
    Pressure_Cfg_t* cfg = self->cfg;
    do{
        u16 val = func_sensor_read(self->cfg->sensor_idx);
        printf("sensor val = %d\r\n", val);
        singal_write_to_buffer(self,val);
        if(self->pressure == 0){
            self->pressure = val;
            continue;
        }
        if(pressure_range_error(cfg, val)){
            return PRESSURE_OUT_OF_RANGE;
        }
        if(pressure_reach_threshold(cfg, self->pressure, val)){
            return NO_ERROR;
        }
        self->pressure = val;
        bus_os_sleep(cfg->listen_time_span);
    }while(self->data_len < (cfg->data_cnt * 2) + DATA_HEAD_LEN);
    return THRESHOLD_NO_REACH;
}


/** Pressure_t成员函数 **/
/**
 * @Description 解析数据并开始执行
 * @Param packet 总线数据
 * @Return none
 */
static void parse(bus_packet* packet){
    Pressure_t* self = &pressure;
    ASSERT(self->is_busy, "busy\r\n");
    self->is_busy = true;
    entity_clear_set(self->entity, packet);
    cfg_parse_set(self->entity->cfg, packet->data);
    bus_os_post_sem(&self->sem);
}

/** Pressure_t成员函数 **/
static void add(Pressure_Cfg_t* param){
    Pressure_t* self = &pressure;
    ASSERT(self->is_busy, "busy\r\n");
    self->is_busy = true;
    entity_clear_set(self->entity, NULL);
    cfg_copy(self->entity->cfg, param);
    bus_os_post_sem(&self->sem);
}

/**
 * @Description 此函数调用后会释放io_timing_array持有的内存
 * @Param none
 * @Return none
 */
static void io_timing_write_before_listen(void){
    Pressure_t* self = &pressure;
    Pressure_Cfg_t* cfg = self->entity->cfg;
    func_io_timing_write(cfg->io_timing_array, cfg->io_timing_len);
    free(cfg->io_timing_array);
    cfg->io_timing_array = NULL;
    cfg->io_timing_len = 0;
}

//队列等待线程
static void pressure_listen_thread(void* data)
{
	while(1)
	{
		bus_os_pend_sem(&pressure.sem, 0);
        if(pressure.entity->cfg->io_timing_len != 0){
            io_timing_write_before_listen();
        }
        pressure.entity->error_code = singal_capture(pressure.entity);
        printf("error_code = %d\r\n", pressure.entity->error_code);
        result_report(pressure.entity);
        pressure.is_busy = false;
	}
}


