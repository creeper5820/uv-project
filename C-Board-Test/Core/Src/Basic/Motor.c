#include "Motor.h"

/**************************************************
 * 函数功能：电机初始化
 30000Hz  arr 1000
 */
void Motor_Init()
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
}

/**************************************************
 * 函数功能：电机控制
 * 参数：motor_left：PWM的值，还可继续封装
 30000Hz  arr 1000
 */
void Set_Pwm(int motor_left, int motor_right) // 输出PWM大小
{
    // PWM限幅	800
    if (motor_left > PWM_limit)
        motor_left = PWM_limit; // 电机输出限幅
    if (motor_left < -PWM_limit)
        motor_left = -PWM_limit; // 电机输出限幅

    if (motor_right > PWM_limit)
        motor_right = PWM_limit; // 电机输出限幅
    if (motor_right < -PWM_limit)
        motor_right = -PWM_limit; // 电机输出限幅
    //电机方向+正转，-反转

    /*********************左电机************************/ // 接左电机
    if (motor_left >= 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, motor_left);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, -motor_left);
    }
    /*********************右电机************************/ // 接右电机
    if (motor_right >= 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, motor_right);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, -motor_right);
    }
}
