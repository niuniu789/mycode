#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"
#include "timer.h"
#include "oled.h"
#include "timer2.h"
#include "motor.h"
#include "datahd.h"
#include "myiic.h"



extern	float AI0,AI1,AI2,AI3;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);   //初始化延时函数
	MdbInit(57600);
	IIC_Init();
	OLED_Init();
	TIM3_Int_Init(300-1,4200-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms     
	TIM2_Int_Init(500-1,8400-1);	// 50ms
	SendDataInit();
	MotorInit();
	
	OLED_ShowString(2,2,"AI0:",12); 
	OLED_ShowString(2,17,"AI1:",12); 
	OLED_ShowString(2,32,"AI2:",12); 
	OLED_ShowString(2,45,"AI3:",12); 
	OLED_ShowString(102,2,"mm",12); 
	OLED_ShowString(102,17,"mm",12); 
	OLED_ShowString(102,32,"mm",12); 
	OLED_ShowString(102,45,"mm",12); 
	OLED_Refresh_Gram();//更新显示到OLED		
	
	while(1)
	{ 	
		SendDataProcessing();
		
   }
}

