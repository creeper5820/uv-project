#pragma once

#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"
#include "../Utility/Utility.hh"

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

void Turn_Left(float speed, float direction);
void Turn_Right(float speed, float direction);
void Straight(float speed);
void Light_Mode(int mode);
