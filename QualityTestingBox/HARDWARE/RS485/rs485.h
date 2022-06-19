#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
	  	


//模式控制
#define RS485_TX_EN		PGout(8)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，设置EN_USART2_RX为1，否则设置为0
#define EN_USART2_RX 	1			//0,不接收;1,接收.


typedef enum{
	MdbReceiveStart = 0,
	MdbReceiveOver,
}MdbReceiveState;

typedef struct{
	MdbReceiveState State;
	u8 TimeCount;
	u8 TimeOut;
	u8 MdbReceiveBuff[64];
	u8 MdbReceiveCount;
}MdbReceive;



void MdbInit(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);

#endif	   
















