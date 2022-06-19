#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RS485���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
	  	


//ģʽ����
#define RS485_TX_EN		PGout(8)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ�����EN_USART2_RXΪ1����������Ϊ0
#define EN_USART2_RX 	1			//0,������;1,����.


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
















