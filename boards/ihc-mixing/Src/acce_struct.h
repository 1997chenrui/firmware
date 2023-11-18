#ifndef ACCE_STRUCT_H_ 
#define ACCE_STRUCT_H_ 
#include <stdint.h> 
typedef struct { 
	uint16_t Count;
	uint16_t Delta;
}acce_struct;

typedef struct {
	uint32_t  	       acce_value;
	const acce_struct  *period;
	uint32_t           period_cnt;
}acce_map;
#define MOTOR_MIN_PERIOD 16
extern const acce_struct ACCE_1600000[112];
extern const acce_struct ACCE_800000[146];
extern const acce_struct ACCE_400000[188];
extern const acce_struct ACCE_200000[240];
extern const acce_struct ACCE_100000[307];
extern const acce_struct ACCE_50000[391];
extern const acce_struct ACCE_20000[537];
extern const acce_struct ACCE_10000[680];
extern const acce_struct ACCE_5000[861];
extern const uint32_t acce_value[9] ; 
extern const acce_map acce_map_values[9];
#endif 
