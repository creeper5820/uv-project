#pragma once

#include "main.h"
#include "tim.h"

typedef struct LED_Struct {
    TIM_HandleTypeDef *htim;
    uint32_t Channel;
} LED_Struct;

/**
 * @param flag_turn when to turn
 * @param flag_slow when to slow down
 * @param flag_stop when to stop
 * @param offset how much uv turn to keep moving along the straight line
 */
struct Data_OpenCV {
    int flag_turn;
    int flag_slow;
    int flag_stop;
    int offset;
};

/**
 * @param distance
 */
struct Data_Tof {
    float distance;
};

/**
 * @brief Value to control motion
 */
struct Control_Motion {
    float speed;
    float direction;
};

/**
 * @brief Value to control light
 */
struct Control_Light {
    float speed;
    float direction;
};

/**
 * @param Factor_K
 * @param Factor_I
 * @param Factor_P
 */
struct Control_System {
};

/**
 * @param R
 * @param G
 * @param B
 */
struct Control_Led {
    float R;
    float G;
    float B;
};

typedef struct Data_OpenCV Data_OpenCV;
typedef struct Data_Tof Data_Tof;

typedef struct Control_System Control_System;
typedef struct Control_Motion Control_Motion;
typedef struct Control_Light Control_Light;
typedef struct Control_Led Control_Led;

typedef struct LED_Struct LED_Struct;
