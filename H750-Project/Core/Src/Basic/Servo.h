#pragma once

#include "main.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

void Servo_Init(TIM_HandleTypeDef *htim);
void Servo_Control(TIM_HandleTypeDef *htim, uint16_t duty);

#ifdef __cplusplus
}
#endif
