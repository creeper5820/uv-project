#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "tim.h"

#define PWM_limit 1800

extern int Motor_Left, Motor_Right; //电机PWM变量
extern int targetSpeed;
extern int Turn;
extern int Target_TurnAngle;
extern float Integral;
extern int speedLeft, speedRight;

void Set_Pwm(int motor_left, int motor_right); //输入为PWM大小
void Motor_Init(void);

#endif
