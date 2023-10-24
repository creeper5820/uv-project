#include "Servo.h"
#include "tim.h"

/**************************************************
 * 函数功能：舵机初始化
 50Hz
 */
void Servo_Init(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1); // arr:0-2000;
}

/**************************************************
 * 函数功能：使舵机旋转某角度
 * 参数：duty：旋转角度-78-  上下限未测，可能要改
 */
void Servo_Control(TIM_HandleTypeDef *htim, uint16_t duty)
{
    htim->Instance->CCR1 = duty;
}
