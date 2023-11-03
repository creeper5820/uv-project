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
};

/*****************
 * @param distance
 */
typedef struct Data_Tof Data_Tof;
struct Data_Tof {
    int distance;
};

/*********************
 * @brief Side marigin
 * @param margin_a
 * @param margin_b
*/
typedef struct Data_Margin Data_Margin;
struct Data_Margin {
    int margin_a;
    int margin_b;
};

/*****************
 * @param factor_k
 * @param factor_i
 * @param factor_p
 * @param factor_encode
 */
typedef struct Data_System Data_System;
struct Data_System {
    float factor_p_m;
    float factor_i_m;
    float factor_d_m;
    float factor_p_s;
    float factor_i_s;
    float factor_d_s;
    float encode_max;
    int offset_max;
    int model_pid;
};

/********************************
 * @brief Value to control motion
 * @param speed
 * @param direction
 */
typedef struct Data_Motion Data_Motion;
struct Data_Motion {
    float speed;
    float distance;
};

/*******************************
 * @brief Value to control light
 * @param status
 */
typedef struct Data_Light Data_Light;
struct Data_Light {
    int status;
};

typedef struct Data_Led Data_Led;
struct Data_Led {
    int light[4];
};

// The Model and function
enum Model {
    MODEL_DEBUG = 0,
    TASK_A,
    TASK_B,
    TASK_C,
    PREPARED,

    LIGHT_ALL,
    DARK_ALL,
    LIGHT_LEFT,
    LIGHT_RIGHT,
    LIGHT_HEAD,
    LIGHT_TAIL,
    FLASH_ALL,
    FLASH_THREE,
};