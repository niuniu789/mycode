#include "motor.h"
#include "rs485.h"
#include "crc.h"
#include "datahd.h"



 void MotorInit(void)
{
		MotorMode(Motor42ID,Mode3);
		delay_ms(DelayTime);
		MotorMode(Motor28ID,Mode1);
		delay_ms(DelayTime);
		MotorSpeed1(Motor28ID,280);
		delay_ms(DelayTime);			
}


void MotorMode1Start(u8 id)
{
	u32 CRCdata1,CRCdata2;
	u16 CRCHigh1,CRCLow1,CRCHigh2,CRCLow2;
	u8 ControlWord1[20] = {0x00,0x06,0x31,0x00,0x00,0x6f};
	u8 ControlWord2[20] = {0x00,0x06,0x31,0x00,0x00,0x7f};
	ControlWord1[0] = id;
	ControlWord2[0] = id;
	CRCdata1 = MdbCrc16(ControlWord1,6);
	CRCdata2 = MdbCrc16(ControlWord2,6);
	
	CRCLow1 = CRCdata1&0xff;
	CRCHigh1 = (CRCdata1>>8)&0xff;
	ControlWord1[6] = CRCHigh1;
	ControlWord1[7] = CRCLow1;
	
	CRCLow2 = CRCdata2&0xff;
	CRCHigh2 = (CRCdata2>>8)&0xff;
	ControlWord2[6] = CRCHigh2;
	ControlWord2[7] = CRCLow2;
	
	RS485_Send_Data(ControlWord1,8);
	delay_ms(10);
	RS485_Send_Data(ControlWord2,8);
	
}


void MotorMode3Start(u8 id)
{
	u32 CRCdata;
	u16 CRCHigh,CRCLow;
	u8 ControlWord1[20] = {0x00,0x06,0x31,0x00,0x00,0x0f};
	ControlWord1[0] = id;
	CRCdata = MdbCrc16(ControlWord1,6);
	CRCLow = CRCdata&0xff;
	CRCHigh= (CRCdata>>8)&0xff;
	ControlWord1[6] = CRCHigh;
	ControlWord1[7] = CRCLow;
	RS485_Send_Data(ControlWord1,8);
}


void MotorStop(u8 id)
{
	u32 CRCdata;
	u16 CRCHigh,CRCLow;
	u8 Stop[20] = {0x00,0x06,0x31,0x00,0x00,0x06};
	Stop[0] = id;
	CRCdata = MdbCrc16(Stop,6);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	Stop[6] = CRCHigh;
	Stop[7] = CRCLow;
	RS485_Send_Data(Stop,8);
}



void MotorSpeed1(u8 id,int16_t speed)
{
	u32 DEC,CRCdata;
	u16 CRCHigh,CRCLow;
	u8 Speed1[20] = {0x00,0x10,0x4a,0x00,0x00,0x02,0x04,};
	Speed1[0] = id;
	if(speed>=0) DEC = speed*512*32;
	if(speed<0) DEC =0xffffffff-(-speed*512*32);
	Speed1[7] =(DEC>>8)&0xff;
	Speed1[8] = DEC&0xff;
	Speed1[9] =(DEC>>24)&0xff;
	Speed1[10] = (DEC>>16)&0xff;
	CRCdata = MdbCrc16(Speed1,11);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	Speed1[11] = CRCHigh;
	Speed1[12] = CRCLow;
	RS485_Send_Data(Speed1,13);
	
}

void MotorSpeed3(u8 id,int16_t speed)
{
	u32 DEC, CRCdata;
	u16 CRCHigh,CRCLow;
	u8 Speed3[20] = {0x00,0x10,0x6f,0x00,0x00,0x02,0x04};
	Speed3[0] = id;
	if(speed>=0) DEC = speed*512*32;
	if(speed<0) DEC =0xffffffff-(-speed*512*32);
	Speed3[7] =(DEC>>8)&0xff;
	Speed3[8] = DEC&0xff;
	Speed3[9] =(DEC>>24)&0xff;
	Speed3[10] = (DEC>>16)&0xff;
	CRCdata = MdbCrc16(Speed3,11);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	Speed3[11] = CRCHigh;
	Speed3[12] = CRCLow;
	RS485_Send_Data(Speed3,13);
}


void MotorMode(u8 id,uint8_t mode)
{
	u32 CRCdata;
	u16 CRCHigh,CRCLow;
	u8 Mode[10] = {0x00,0x06,0x35,0x00,0x00,0x00};
	Mode[0] = id;
	Mode[5] = mode;
	CRCdata = MdbCrc16(Mode,6);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	Mode[6] = CRCHigh;
	Mode[7] = CRCLow;
	RS485_Send_Data(Mode,8);	
}


void MotorLocation(u8 id,int16_t location)
{
	u32 DEC, CRCdata;
	u16 CRCHigh,CRCLow;
	u8 Location[20] = {0x00,0x10,0x40,0x00,0x00,0x02,0x04,};
	Location[0] = id;
	DEC = location*512*32;
	Location[7] =(DEC>>8)&0xff;
	Location[8] = DEC&0xff;
	Location[9] =(DEC>>24)&0xff;
	Location[10] = (DEC>>16)&0xff;
	CRCdata = MdbCrc16(Location,11);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	Location[11] = CRCHigh;
	Location[12] = CRCLow;
	RS485_Send_Data(Location,13);
}


