#pragma once

#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Application/Motion_Controller.hh"
#include "../Basic/Message_Type.hh"
#include "../Utility/Utility.hh"

#define flag_debug 0
#define flag_wait_pi 0
#define flag_begin 0

#define distance_right 58
#define distance_left 23

static int model = MODEL_DEBUG;

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
void Turn_Right(int time_ms);
void Turn_Left(int time_ms);

void Scan_Block_Next(int distance);
void Scan_Block_Stop(int distance);
