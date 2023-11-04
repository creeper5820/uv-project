// #ifndef _ENCODE_H_
// #define _ENCODE_H_

// #include "stm32f1xx.h"
// #include "tim.h"

// //定时器号
// #define ENCODER_TIM_1 htim2
// #define ENCODER_TIM_2 htim3
// #define GAP_TIM     htim4

// #define PULSE_PRE_ROUND 11 //一圈多少个脉冲
// #define RELOADVALUE_1 __HAL_TIM_GetAutoreload(&ENCODER_TIM_1)    //获取自动装载值,本例中为20000
// #define COUNTERNUM_1 __HAL_TIM_GetCounter(&ENCODER_TIM_1)        //获取编码器定时器中的计数值
// #define RELOADVALUE_2 __HAL_TIM_GetAutoreload(&ENCODER_TIM_2)
// #define COUNTERNUM_2 __HAL_TIM_GetCounter(&ENCODER_TIM_2)

// typedef struct _Motor
// {
//     int32_t lastCount;   //上一次计数值
//     int32_t totalCount;  //总计数值
//     int16_t overflowNum; //溢出次数
//     float speed;         //电机转速
//     uint8_t direct;      //旋转方向
// }Motor;

// void Encoder_Init(void);

// #endif

#pragma once

#include "tim.h"

int Read_Encoder(uint8_t TIMX);
void Encoder_Init();
// void Until_Distance(int32_t distance);
