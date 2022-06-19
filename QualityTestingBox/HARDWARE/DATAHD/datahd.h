#ifndef __DATAHD_H
#define __DATAHD_H			  	 
#include "sys.h"
#include "delay.h"

#define DataFunctionCode01  0x01
#define DataFunctionCode03  0x03
#define DataFunctionCode06  0x06
#define DataFunctionCode0f  0x0f
#define DataFunctionCode10  0x10


#define MdbReceiveID  0
#define MdbReceiveFunCode 1
#define MdbReceiveAddr   2

#define IOSlaveID  1
#define Motor28ID   0x02
#define Motor42ID   0x03
#define  Mode1   1
#define  Mode3   3
#define DelayTime  10
	


typedef enum{
	AccurateData = 0,
	DataCrcError,
	DataIdError,
	DataFuncCodeError,
}DataHandleStatus; //数据校验状态


typedef struct{
	DataHandleStatus  status;
	u8  WriteDO[64];
}DataHandle; //接收数据处理

typedef enum  
 {
	 ReadDI = 1,
	 ReadAI ,
	 WriteDO
 }IO;   //IO轮询状态切换
 
typedef enum
 {
	PositiveMove= 1,
	PosStartMove,
	PosStopMove,
	NegativeMove,
	NegStartMove,
	NegStopMove,	
}MotorStatus; //电机运行状态切换


typedef struct{
	MotorStatus Status;
	u16 MotorTimeCount;
}Motor;

typedef void (*dataConfig_t)(u8, int16_t);
typedef void (*Start_t)(u8);
typedef void (*Stop_t)(u8);


void RecDataProcessing(DataHandle *Mdb);
void SendDataProcessing();
void SendDataInit(void);
#endif
