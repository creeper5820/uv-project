#include "Servo.h"
#include "tim.h"

/**************************************************
 * @brief 舵机初始化
 * 50Hz
 */
void Servo_Init()
{
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
}

/**************************************************
 * @brief 使舵机旋转某角度
 * @param duty：旋转角度-78-  上下限未测，可能要改
 */
void Servo_Control(uint16_t duty)
{
    htim15.Instance->CCR1 = duty;
}
