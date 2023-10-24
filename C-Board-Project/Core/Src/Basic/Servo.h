#ifndef __SERVO_H__
#define __SERVO_H__

#include "main.h"
#include "tim.h"

void Servo_Init(TIM_HandleTypeDef *htim);
void Servo_Control(TIM_HandleTypeDef *htim, uint16_t duty);

#endif
