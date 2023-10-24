#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "tim.h"

#define PWM_limit 1800

extern int Motor_Left, Motor_Right; //电机PWM变量


extern int speedLeft, speedRight;       //  左右轮速度


void Set_Pwm(int motor_left, int motor_right); //   输入为PWM大小
void LMotor_PID(float Kp, float Ki, float Kd, float Err);       //  
void Motor_Init(void);

#endif
