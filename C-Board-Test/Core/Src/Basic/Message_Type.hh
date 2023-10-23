#pragma once

#include "main.h"
#include "tim.h"

/**
 * @brief The type of light
 * @param htim The TIM_HandleTypeDef to control pwm
 * @param Channel The Channel of timer to control pwm
 */
typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t Channel;
} LED_Struct;

/**
 * @brief Some Factors to adjust PID or other controller
 * @param Factor_K
 * @param Factor_I
 * @param Factor_P
 *
 */
typedef struct {

} Factor_System;

/**
 * @brief Struct define to control the motion
 * @param flag_turn when to turn
 * @param flag_slow when to slow down
 * @param flag_stop when to stop
 * @param offset how much uv turn to keep moving along the straight line
 */
typedef struct {
    int flag_turn;
    int flag_slow;
    int flag_stop;
    int offset;
} Data_OpenCV;