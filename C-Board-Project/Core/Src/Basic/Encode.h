#pragma once

#include "tim.h"

extern int Encoder_Left, Encoder_Right; //左右编码器的脉冲计数
extern int64_t encoderSum;
extern uint8_t flagMeasure;

int Read_Encoder(uint8_t TIMX);
void Encoder_Init(TIM_HandleTypeDef *htim_1, TIM_HandleTypeDef *htim_2);
// void Until_Distance(int32_t distance);
