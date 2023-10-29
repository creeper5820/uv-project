#pragma once

#include "main.h"
#include "tim.h"

/*******************************
 * @param flag_turn when to turn
 * @param flag_slow when to slow down
 * @param flag_stop when to stop
 * @param offset how much uv turn to keep moving along the straight line
 */
typedef struct Data_OpenCV Data_OpenCV;
struct Data_OpenCV {
    int flag_turn;
    int flag_slow;
    int flag_stop;
    int flag_offset;
};

/*****************
 * @param distance
 */
typedef struct Data_Tof Data_Tof;
struct Data_Tof {
    int distance;
};

/*****************
 * @param factor_k
 * @param factor_i
 * @param factor_p
 * @param factor_encode
 */
typedef struct Control_System Control_System;
struct Control_System {
    int model_pid;
    float factor_p;
    float factor_i;
    float factor_d;
    float factor_encode;
    int offset_max;
};

/********************************
 * @brief Value to control motion
 * @param speed
 * @param direction
 */
typedef struct Control_Motion Control_Motion;
struct Control_Motion {
    float speed;
    float direction;
};

/*******************************
 * @brief Value to control light
 * @param status
 */
typedef struct Control_Light Control_Light;
struct Control_Light {
    int status;
    int task;
};

typedef struct Control_Led Control_Led;
struct Control_Led {
    int light[4];
};

enum Status_Light {
    LIGHT_ALL,
    DARK_ALL,
    LIGHT_LEFT,
    LIGHT_RIGHT,
    LIGHT_HEAD,
    LIGHT_TAIL,
    FLASH_ALL,
};

// The Model and function
enum Model_Task {
    TASK_A,
    TASK_B,
    TASK_C,
    MODEL_DEBUG,
    PREPARE,
};