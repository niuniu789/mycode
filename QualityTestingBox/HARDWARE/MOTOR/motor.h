#ifndef __MOTOR_H
#define __MOTOR_H			  	 
#include "sys.h"
#include "delay.h"

void MotorInit(void);
void MotorMode(u8 id,uint8_t mode);
void MotorMode1Start(u8 id);
void MotorMode3Start(u8 id);
void MotorStop(u8 id);
void MotorSpeed1(u8 id,int16_t speed);
void MotorSpeed3(u8 id,int16_t speed);
void MotorLocation(u8 id,int16_t location);

#endif
