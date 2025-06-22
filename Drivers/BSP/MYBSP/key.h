/*
*********************************************************************************************************
*
*	模块名称 : 
*	文件名称 : 
*	版    本 : 
*	说    明 : 
*
*	修改记录 :
*		版本号      日期         作者         说明
*		V1.0    2025-05-14   shisanxiang1       0
*
*********************************************************************************************************
*/
#ifndef __KEY_H__
#define __KEY_H__


#include <stdint.h>

/*按键引脚定义 */
//设置为下降沿触发外部中断
#define KEY1_GPIO_PIN                 key1_Pin
#define KEY1_GPIO                     GPIOG
#define KEY1_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */

#define KEY2_GPIO_PIN                 key2_Pin
#define KEY2_GPIO                     GPIOG
#define KEY2_DOWN_LEVEL               0  /* 根据原理图设计，KEY2按下时引脚为低电平，所以这里设置为0 */

#define KEY3_GPIO_PIN                 key3_Pin
#define KEY3_GPIO                     GPIOG
#define KEY3_DOWN_LEVEL               0  /* 根据原理图设计，KEY3按下时引脚为低电平，所以这里设置为0 */

#define KEY4_GPIO_PIN                 key4_Pin
#define KEY4_GPIO                     GPIOG
#define KEY4_DOWN_LEVEL               0  /* 根据原理图设计，KEY4按下时引脚为低电平，所以这里设置为0 */

/* 按键fifo缓存大小设置 */
#define KEY_FIFO_SIZE	10


/* 按键fifo结构体声明 */
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;									/* 缓冲区读指针1 */
	uint8_t Write;								/* 缓冲区写指针 */
}KEY_FIFO_T;
/* 外部声明按键结构体 */
extern KEY_FIFO_T s_tKey;

/* 函数声明 */
void key_fifoinit(KEY_FIFO_T *s_tKey);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);



/*定义键值代码*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_1_DOWN,				/* 1键按下 */

	KEY_2_DOWN,				/* 2键按下 */

	KEY_3_DOWN,				/* 3键按下 */

	KEY_4_DOWN,				/* 4键按下 */

}KEY_ENUM;

#endif
