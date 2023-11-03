#pragma once

#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"
#include "../Utility/Utility.hh"

#define flag_debug 1
#define flag_wait_pi 0
#define flag_begin 1

#define distance_right 30
#define distance_left 18

static int model = TASK_A;

// Override the function
extern "C" {
void Messager_Loop();
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size);
}

void Begin();
void Task_A();
void Task_B();
void Task_C();
void Model_Debug();

void Set_Speed(float speed, float direction);
void Light_Mode(int mode);
void Light_Task(int task);

void Turn_Left(float speed, float direction);
void Turn_Right(float speed, float direction);
void Straight(float speed);
void Brake();
void Scan_Block_Next(int distance);
void Scan_Block_Stop(int distance);
