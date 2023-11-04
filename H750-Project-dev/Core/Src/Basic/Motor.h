#pragma once

#include "main.h"
#include "tim.h"
#include "gpio.h"

#define PWM_limit 500

#ifdef __cplusplus
extern "C" {
#endif

void Set_Pwm(int motor_left, int motor_right);
void Motor_Init();

#ifdef __cplusplus
}
#endif
