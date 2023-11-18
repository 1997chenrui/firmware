#ifndef __PUBLIC_H
#define __PUBLIC_H

#ifdef __cplus_plus
extern "C" {
#endif


#undef NULL
#define NULL 0

#ifndef bool
	#ifndef __cplusplus // In C++, 'bool', 'true' and 'false' and keywords
		#define bool _Bool
		#define true 1
		#define false 0
	#else
		#ifdef __GNUC__ // GNU C++ supports direct inclusion of stdbool.h to provide C99 compatibility by defining _Bool
		#define _Bool bool
		#endif
	#endif
#endif


	
typedef union
{
	float fdat;
	unsigned char bytes[sizeof(float)];
} uFloatType;


typedef union
{
	double dfloat;
	unsigned char bytes[sizeof(double)];
} uDoubleType;

typedef union
{
	unsigned long ulint;
	unsigned char bytes[sizeof(unsigned long)];
} uDwordType;

typedef union
{
	unsigned short int usint;
	unsigned char bytes[sizeof(unsigned short int)];
} uWordType;



// 计算字符串长度（不含字符串结束符）
unsigned int public_strlen(const void *p);

// 内存设置
bool public_memset(void * const p, unsigned char dat, unsigned int length);

// 内存拷贝
bool public_memcpy(void * const des, const void *src, unsigned int length);

// 内存数据比较
// 返回值：0=(p1==p2); 1=(p1>p2); 2=(p1<p2); 3=error
unsigned char public_memcmp(const void *p1, const void *p2, unsigned int length);

// 字符串转IP端口号，IP和端口号可单独获取
// p："192.168.1.100:8088"
// ip：输出IP，长度必须大于等于4字节，NULL表示不解析该参数
// port：输出端口号，NULL表示不解析该参数
bool public_ip_string_to_ip_bytes(const void *p, unsigned char * const out_ip, unsigned short int * const out_port);

// 将字节数据转成十进制字符串，例：123 = “123”
// dat: 输入数据
// str: 输出字符串指针
// count：输出字符串长度，自动添加前导零（该值小于数据位数时不影响实际输出）
// 返回值：字符串长度
unsigned char public_int_to_dec_string(unsigned int data, void * const str, unsigned char count);

// 将字节数据转成十六进制字符串，例：123 = “7B”
// dat: 输入数据
// str: 输出字符串指针
// count：输出字符串长度，自动添加前导零（该值小于数据位数时不影响实际输出）
// 返回值：字符串长度
unsigned char public_int_to_hex_string(unsigned int data, void * const str, unsigned char count);

// 将浮点数据转成十进制字符串，例：1.23f = "1.23"
// dat: 输入数据
// str: 输出字符串指针
// count：保留小数点后的位数，不够自动补0
// 返回值：字符串长度
unsigned char public_flaot_to_dec_string(float data, void * const str, unsigned char count);

// 数组逆序
// p:数组指针; len:数组长度
bool public_array_reversed(const void *p, const unsigned int length);

// 字节数组转短整形数据
unsigned short public_bytes_to_short(const void *pbuf);

// 字节数组转长整形数据
unsigned int public_bytes_to_int(const void *pbuf);

// 字节数组转双浮点类型数据
double public_bytes_to_double(const void *pbuf);

// 字节数组转单浮点类型数据
float public_bytes_to_float(const void *pbuf);

// 短整型数据转字节数组
bool public_short_to_bytes(void * const pbuf, unsigned short data);

// 长整型数据转字节数组
bool public_int_to_bytes(void * const pbuf, unsigned int data);

// 双浮点类型数据转字节数组
bool public_double_to_bytes(void * const pbuf, double data);

// 单浮点类型数据转字节数组
bool public_float_to_bytes(void * const pbuf, float data);

/********************************************************
 * 函数功能：将指定数据转成以2为底的指数形式
 * 形    参：指定数据
 * 返 回 值：返回指数
 * 开 发 者：王志超
 * 维护日期：2020年7月31日
 * 修订日志：开发
 ********************************************************/
unsigned char public_data_index(unsigned short data);

// 计算校验和
unsigned char public_check_sum(const void *p, unsigned int len);

// 计算CRC16校验码
unsigned short public_check_crc16(const void *pbuf, unsigned int length);

// 计算CRC32校验码
unsigned int public_check_crc32(const void *pbuf, unsigned int length);





#ifdef __cplus_plus
}
#endif

#endif
