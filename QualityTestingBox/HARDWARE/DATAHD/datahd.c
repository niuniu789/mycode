#include "datahd.h"
#include "rs485.h"
#include "crc.h"
#include "motor.h"
#include "oled.h"


extern  MdbReceive  MdbReceiveData;
extern  u8 Timer2Flag;
//extern u8 Timestatus;
float AI0,AI1,AI2,AI3;
DataHandle  Mdb;
Motor Motor28;
Motor *pMotor28 = &Motor28;
Motor Motor42;
Motor *pMotor42 = &Motor42;
IO IORead;


static DataHandleStatus MdbrtuCRCcheck( void )
{
	uint16_t CheckCrc;
	uint16_t CalCrc;

	CalCrc = MdbCrc16( MdbReceiveData.MdbReceiveBuff , (MdbReceiveData.MdbReceiveCount)-2);
	CheckCrc = (MdbReceiveData.MdbReceiveBuff[(MdbReceiveData.MdbReceiveCount)-2]<<8) |
	(MdbReceiveData.MdbReceiveBuff[(MdbReceiveData.MdbReceiveCount)-1]);
	if( CalCrc == CheckCrc ) return AccurateData;
	else  return DataCrcError;
}



static void mdbrtuFuncCode01( DataHandle *Mdb, uint8_t *pdata )
{
		u16 CrcWriteData;
		Mdb->WriteDO[0] = 0x01;
		Mdb->WriteDO[1] = 0x0f;
		Mdb->WriteDO[2] = 0x00;
		Mdb->WriteDO[3] = 0x10;
		Mdb->WriteDO[4] = 0x00;
		Mdb->WriteDO[5] = 0x08;
		Mdb->WriteDO[6] = 0x01;
		Mdb->WriteDO[7] = pdata[3];
		CrcWriteData = MdbCrc16(Mdb->WriteDO,8);
		Mdb->WriteDO[8] = (CrcWriteData>>8)&0xff;	
		Mdb->WriteDO[9] = CrcWriteData&0xff;
		RS485_Send_Data(Mdb->WriteDO,10);	
		IORead = WriteDO;
	
	//	Timer2Flag = 0;
}



static void mdbrtuFuncCode03( DataHandle *Mdb, uint8_t *pdata )
{
	u16 AnalogData0,AnalogData1,AnalogData2,AnalogData3;
	
	AnalogData0 = (MdbReceiveData.MdbReceiveBuff[3]<<8) | MdbReceiveData.MdbReceiveBuff[4];
	AnalogData1 = (MdbReceiveData.MdbReceiveBuff[5]<<8) | MdbReceiveData.MdbReceiveBuff[6];
	AnalogData2 = (MdbReceiveData.MdbReceiveBuff[7]<<8) | MdbReceiveData.MdbReceiveBuff[8];
	AnalogData3 = (MdbReceiveData.MdbReceiveBuff[9]<<8) | MdbReceiveData.MdbReceiveBuff[10];
	if(AnalogData0<0x0a) AI0 = 0;
	else AI0 = (AnalogData0/8.19)-300;
	if(AnalogData1<0x0a) AI1 = 0;
	else AI1 = (AnalogData1/8.19)-300;
	if(AnalogData2<0x0a) AI2 = 0;
	else AI2 = (AnalogData2/8.19)-300;
	if(AnalogData3<0x0a) AI3 = 0;
	else AI3 = (AnalogData3/8.19)-300;
	
	OLED_ShowFNum(52,2,AI0,12);	
	OLED_ShowFNum(52,17,AI1,12);		
	OLED_ShowFNum(52,32,AI2,12);	
	OLED_ShowFNum(52,45,AI3,12);			
	OLED_Refresh_Gram();//更新显示到OLED	
	//Timer2Flag = 0;
}



static void mdbrtuFuncCode06( DataHandle *Mdb, uint8_t *pdata )
{
}




static void mdbrtuFuncCode0f( DataHandle *Mdb, uint8_t *pdata )
{
}




static void mdbrtuFuncCode10( DataHandle *Mdb, uint8_t *pdata )
{
}



static void DataError(DataHandleStatus status)
{
	if(status == DataCrcError)
	{
	
	}
	if(status == DataIdError)
	{
	
	}
	if(status == DataFuncCodeError)
	{
	
	}
	
}



static void RS485ReadDI(u8 id)
{
	u32 CRCdata;
	u16 CRCHigh,CRCLow;
	u8 ReadDI[20] = {0x00,0x01,0x00,0x00,0x00,0x08};
	ReadDI[0] = id;
	CRCdata = MdbCrc16(ReadDI,6);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	ReadDI[6] = CRCHigh;
	ReadDI[7] = CRCLow;
	RS485_Send_Data(ReadDI,8);
}

static void RS485ReadAI(u8 id)
{
	u32 CRCdata;
	u16 CRCHigh,CRCLow;
	u8 ReadAI[20] = {0x00,0x03,0x01,0x00,0x00,0x04};
	ReadAI[0] = id;
	CRCdata = MdbCrc16(ReadAI,6);
	CRCLow = CRCdata&0xff;
	CRCHigh = (CRCdata>>8)&0xff;
	ReadAI[6] = CRCHigh;
	ReadAI[7] = CRCLow;
	RS485_Send_Data(ReadAI,8);
}



void RecDataProcessing(DataHandle *Mdb)
{
	if(MdbReceiveData.State == MdbReceiveOver)
	{
		if(MdbReceiveData.MdbReceiveBuff[MdbReceiveID] == IOSlaveID)
		{
			if( MdbrtuCRCcheck() == AccurateData)
			{
				switch(MdbReceiveData.MdbReceiveBuff[MdbReceiveFunCode])
				{
					case DataFunctionCode01 :
						mdbrtuFuncCode01(Mdb,MdbReceiveData.MdbReceiveBuff);
					break;
					
					case DataFunctionCode03 :
						mdbrtuFuncCode03(Mdb,MdbReceiveData.MdbReceiveBuff);
					break;
					
					case DataFunctionCode06 :
						mdbrtuFuncCode06(Mdb,MdbReceiveData.MdbReceiveBuff);
					break;
					
					case DataFunctionCode0f :
						mdbrtuFuncCode0f(Mdb,MdbReceiveData.MdbReceiveBuff);
					break;
					
					case DataFunctionCode10 :
						mdbrtuFuncCode10(Mdb,MdbReceiveData.MdbReceiveBuff);
					break;
					default:
						DataError(DataFuncCodeError);
						Mdb ->status  = DataFuncCodeError;
				}
		
			}
			else  DataError(DataCrcError);
			Mdb ->status  = DataCrcError;
		}
		else DataError(DataIdError);
		Mdb ->status  = DataCrcError;	
		MdbReceiveData.State = MdbReceiveStart;
	}
	 //MdbReceiveData.MdbReceiveCount = 0;
}


void SendDataInit(void)
{
	IORead = ReadDI;
	Motor28.Status = PositiveMove;
	Motor28.MotorTimeCount  = 0;
	Motor42.Status = PositiveMove;
	Motor42.MotorTimeCount  = 0;
}



//static void MotorRunning(Motor *motorID,u8 id,int16_t posdata,int16_t negdata,dataConfig_t dataConfig,void (*Start)(),void (*Stop)())
//static void MotorRunning(Motor *motorID,u8 id,int16_t posdata,int16_t negdata, void (*dataConfig_t)(u8, int16_t)  dataConfig(u8, int16_t)  ,void (*Start)(),void (*Stop)())


static void MotorRunning(Motor *motorID,u8 id,int16_t posdata,int16_t negdata,dataConfig_t dataConfig,Start_t Start ,Stop_t Stop,u8 RunPeriod,u8 StartTime,u8 StopTime)
{ 
	motorID->MotorTimeCount++;
	switch(motorID->Status)
	{ 
		case PositiveMove:
			if(motorID->MotorTimeCount ==StartTime)
			{
			dataConfig(id,posdata);
			motorID->Status = PosStartMove;
			}
		break;
		
		
		case PosStartMove:	 
			if(motorID->MotorTimeCount ==StopTime)
			{
				Start(id);
				motorID->MotorTimeCount = 0;
				motorID->Status = PosStopMove;
			}				
		break;
			
			
		case PosStopMove:
		
			if(motorID->MotorTimeCount==RunPeriod)
			{
				Stop(id);
				motorID->MotorTimeCount = 0;
			  motorID->Status = NegativeMove;
			}
		break;
			
			
		case NegativeMove:
			dataConfig(id,negdata);
			motorID->Status = NegStartMove;
		break;
			
		
		case NegStartMove:
			if(motorID->MotorTimeCount==StopTime)
				{
				 Start(id);
				 motorID->MotorTimeCount = 0;
				 motorID->Status = NegStopMove;
				}
			break;
		
	
		case NegStopMove:
		 if(motorID->MotorTimeCount==RunPeriod)
			{
			 Stop(id);
			 motorID->MotorTimeCount = 0;
			 motorID->Status = PositiveMove;	
			}
			break;		
		}		
}



void SendDataProcessing()
{
	if(Timer2Flag ==1)
	{ 
	  MotorRunning(pMotor28,Motor28ID,150,-150,MotorLocation,MotorMode1Start,MotorStop,140,2,10);//几个数字的意义：前边两个是正反方向的位移距离 ，后边三个依次是 运行时间 启动时间 中间间隔时间
	  MotorRunning(pMotor42,Motor42ID,-600,600,MotorSpeed3,MotorMode3Start,MotorStop,80,8,20);	//前边两个数字是设置正反方向速度	 后边三个跟上边意思一样
			
		if(MdbReceiveData.State == MdbReceiveOver)
		{	
			RecDataProcessing(&Mdb);
			switch(IORead)
				{
					case ReadDI:
					RS485ReadDI(IOSlaveID);
					IORead = ReadAI ;
					break;
				 
					case ReadAI:
					RS485ReadAI(IOSlaveID);
					IORead = ReadDI ;
					break;
					
					case WriteDO:
					IORead = ReadAI;
					break;
		    }
			
    }
				Timer2Flag =0;
	}

}
