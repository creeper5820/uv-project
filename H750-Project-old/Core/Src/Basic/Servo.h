#pragma once

#include "main.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

void Servo_Init();
void Servo_Control(uint16_t duty);

#ifdef __cplusplus
}
#endif
