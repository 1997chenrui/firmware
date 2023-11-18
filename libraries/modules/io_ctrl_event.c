#include "io_ctrl_event.h"
#include "tool.h"
#include "cpu.h"

typedef enum{
    NO_ERROR = 0,
    TIME_OUT = 1
}IOCtrl_e;

static IOCtrl_t ctrl;
static SENSOR_READ func_sensor_read;
static IO_WRITE func_io_write;

static void io_timing_write(IOCtrl_timing_t* timing, u8 timing_len);
static void io_timing_before(IOCtrl_cfg_t* cfg);
static void io_timing_after(IOCtrl_cfg_t* cfg);
static bool threshold_value_reach(IOCtrl_cfg_t* cfg, u32 value);
static bool query_cnt_reach(IOCtrl_cfg_t* cfg, u32 cnt);
static void set_sensor_value(IOCtrl_entity_t* self, u32 value);
static void result_report(IOCtrl_entity_t* self);
static void add(IOCtrl_cfg_t* param);
static void parse(bus_packet* packet);
static bool sensor_reach(void* elem);
static void listen_thread(void* data);

/**全局函数**/
void ioctrl_init(void (*thread_create)(void (*thread)(void* data)), SENSOR_READ sensor_read, IO_WRITE io_write){
    ctrl.list = new_list(5);
    func_sensor_read = sensor_read;
    func_io_write = io_write;
    ctrl.add = add;
    ctrl.parse = parse;
	thread_create(listen_thread);
}

IOCtrl_t* ioctrl_get_as_ref(){
    return &ctrl;
}

/** IOCtrl_timing_t 成员函数 **/
/**
 * @Description 根据时序参数写
 * @Param 
    timing时序参数 
    timing_len 参数长度
 * @Return none
 */
static void io_timing_write(IOCtrl_timing_t* timing, u8 timing_len){
    for(u8 i = 0; i < timing_len; i++){
        func_io_write(timing->io_value);
        bus_os_sleep(timing->time_delay);
    }
}

/**IOCtrl_cfg_t成员函数 **/
/**
 * @Description 传感器触发前 写
 * @Param cfg 控制参数
 * @Return none
 */
static void io_timing_before(IOCtrl_cfg_t* cfg){
    io_timing_write(cfg->before, cfg->before_len);
}

/**
 * @Description 传感器触发后 写
 * @Param cfg 控制参数
 * @Return none
 */
static void io_timing_after(IOCtrl_cfg_t* cfg){
    io_timing_write(cfg->after, cfg->after_len);
}
/**
 * @Description 是否达到阈值
 * @Param 
    cfg 控制参数 
    value 传感读出的值
 * @Return 
    true 触发
    false 不触发
 */
static bool threshold_value_reach(IOCtrl_cfg_t* cfg, u32 value){
    IOCtrl_threshold_u threshold_u = cfg->threshold_u;
    if(!threshold_u.split.threshold_enable){
        return false;
    }
    switch(threshold_u.split.trigger_way){
        case 0:
            return threshold_u.split.threshold == value;
        case 1:
            return threshold_u.split.threshold < value;
        case 2:
            return threshold_u.split.threshold > value;
        default:ASSERT(1, "threshold way param error\r\n");
    }
}

/**
 * @Description 是否达到轮训次数
 * @Param
    cfg 控制参数 
    cnt 当前轮训次数
 * @Return
    false 未达到
    true 达到
 */
static bool query_cnt_reach(IOCtrl_cfg_t* cfg, u32 cnt){
    return cfg->query_cnt < cnt;
}

/**
 * @Description 创建cfg
 * @Param data 数据流
 * @Return 根据数据流创建的cfg
 */
static IOCtrl_cfg_t* new_cfg(u8* data){
    IOCtrl_cfg_t* cfg = (IOCtrl_cfg_t* )malloc(sizeof(IOCtrl_cfg_t));
    cfg->report = true;
    int offset = 4;
    cfg->before_len = ReadU32(&data[offset]);
    offset+=4;
    cfg->before = (IOCtrl_timing_t*)malloc(cfg->before_len * sizeof(IOCtrl_timing_t));
    for(int i = 0; i < cfg->before_len; i++){
        cfg->before[i].io_value = ReadU32(&data[offset]);
        offset+=4;
        cfg->before[i].time_delay = ReadU16(&data[offset]);
        offset+=2;
    }
    cfg->after_len = ReadU32(&data[offset]);
    offset+=4;
    cfg->after = (IOCtrl_timing_t*)malloc(cfg->after_len * sizeof(IOCtrl_timing_t));
    for(int i = 0; i < cfg->after_len; i++){
        cfg->after[i].io_value = ReadU32(&data[offset]);
        offset+=4;
        cfg->after[i].time_delay = ReadU16(&data[offset]);
        offset+=2;
    }
    cfg->threshold_u.value = ReadU32(&data[offset]); 
    offset+=4;
    cfg->query_cnt = ReadU32(&data[offset])/QUERY_TIME_SPAN; 
    offset+=4;
    return cfg;
}

/**
 * @Description 拷贝cfg
 * @Param src 源cfg
 * @Return copy后的cfg
 */
static IOCtrl_cfg_t* deep_copy_cfg(const IOCtrl_cfg_t* src){
    IOCtrl_cfg_t* cfg = (IOCtrl_cfg_t* )malloc(sizeof(IOCtrl_cfg_t));
    cfg->report = src->report;
    cfg->before_len = src->before_len;
    cfg->before = (IOCtrl_timing_t*)malloc(cfg->before_len * sizeof(IOCtrl_timing_t));
    for(int i = 0; i < cfg->before_len; i++){
        cfg->before[i].io_value = src->before[i].io_value;
        cfg->before[i].time_delay = src->before[i].time_delay;
    }
    cfg->after_len = src->after_len;
    cfg->after = (IOCtrl_timing_t*)malloc(cfg->after_len * sizeof(IOCtrl_timing_t));
    for(int i = 0; i < cfg->after_len; i++){
        cfg->after[i].io_value = src->after[i].io_value;
        cfg->after[i].time_delay = src->after[i].time_delay;
    }
    cfg->threshold_u.value = src->threshold_u.value; 
    cfg->query_cnt =  src->query_cnt; 
    return cfg;
}

/** IOCtrl_entity_t 成员函数 **/
/**
 * @Description 设置传感器当前值并更新轮训次数
 * @Param 
    self
    value 当前传感器值
 * @Return none
 */
static void set_sensor_value(IOCtrl_entity_t* self, u32 value){
    self->sensor_value = value;
    self->query_time++;
}

/**
 * @Description 事件触发上报
 * @Param self
 * @Return none
 */
static void result_report(IOCtrl_entity_t* self){
    if(self->cfg->report){
        u8 data[] = {self->error_code};
        bus_packet packet = self->packet;
        bus_ita_return(packet.adapterId,packet.srcId,packet.sessionId,1,data);
    }
    free(self->cfg);
    free(self);
}

static IOCtrl_entity_t* new_entity(IOCtrl_cfg_t* cfg, bus_packet* packet){
    IOCtrl_entity_t* entity = (IOCtrl_entity_t* )malloc(sizeof(IOCtrl_entity_t));
    entity->sensor_value = 0;
    entity->query_time = 0;
    entity->error_code = NO_ERROR;
    entity->cfg = cfg;
    if(packet != NULL){
        entity->packet = *packet;
        entity->packet.data = NULL;
    }
    return entity;
}

/**IOCtrl_t成员函数**/
static void parse(bus_packet* packet){
    IOCtrl_cfg_t* cfg = new_cfg(packet->data);
    IOCtrl_entity_t* entity = new_entity(cfg, packet);
    //先调用触发前的io时序
    io_timing_before(cfg);
    //多线程
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    ctrl.list->itf_append(ctrl.list, entity);
    OS_EXIT_CRITICAL();
}

static void add(IOCtrl_cfg_t* param){
    IOCtrl_cfg_t* cfg = deep_copy_cfg(param);
    IOCtrl_entity_t* entity = new_entity(cfg, NULL);
    //先调用触发前的io时序
    io_timing_before(cfg);
    //多线程
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    ctrl.list->itf_append(ctrl.list, entity);
    OS_EXIT_CRITICAL();
}

/**局部函数**/
static bool sensor_reach(void* elem){
    IOCtrl_entity_t* entity = (IOCtrl_entity_t*)elem;
    set_sensor_value(entity, func_sensor_read(entity->cfg->threshold_u.split.sensor_idx));
    //printf("%p : sensor=%d\r\n", entity, entity->sensor_value);
    bool threshold_reach = threshold_value_reach(entity->cfg, entity->sensor_value);
    bool query_reach = query_cnt_reach(entity->cfg, entity->query_time);
    if(query_reach){
        printf("timeout\r\n");
        entity->error_code = TIME_OUT;
    }
    if(threshold_reach || query_reach){
         printf("reach\r\n");
        io_timing_after(entity->cfg);
        printf("report\r\n");
        result_report(entity);
        return true;
    }
    return false;
}

static void listen_thread(void* data){
    while(1){
        // 多线程保护
        if(!ctrl.list->itf_empty(ctrl.list)){
            CPU_SR_ALLOC();
			OS_ENTER_CRITICAL();
            ctrl.list->itf_delete_when(ctrl.list, sensor_reach);
            OS_EXIT_CRITICAL();
        }
        bus_os_sleep(QUERY_TIME_SPAN);
    }
}
