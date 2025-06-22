#ifndef _TYPES_H_
#define _TYPES_H_


//E:\BaiduNetdiskDownload\硬石F407-STD资料\3. 例程\13.EtherCAT主站程序\1.控制台达EtherCAT协议伺服驱动器\汇川SV660电机\YSF4STD_HAL_ETHERCAT-001. 主站测试-速度（汇川）\Inc\types.h
#include "stm32F4xx_hal.h"


typedef unsigned char bool;


#define htons(x) ((((x)&0xff)<<8)|(((x)&0xff00)>>8))
#define ntohs(x) htons(x)
#define htonl(x) ((((x)&0xff)<<24)| \
                 (((x)&0xff00)<<8) | \
                 (((x)&0xff0000)>>8) | \
                 (((x)&0xff000000)>>24))

#define ntohl(x) htonl(x)




#endif

