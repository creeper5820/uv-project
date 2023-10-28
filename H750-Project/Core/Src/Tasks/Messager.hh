#pragma once

#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"

// Override the function
extern "C" {
void Messager_Loop();
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
}

// The Model and function
enum Model_Messager {
    TASK_A,
    TASK_B,
    TASK_C,
    MODEL_DEBUG
};

static int model = MODEL_DEBUG;

// Queue to use
auto Queue_Motion = xQueueCreate(2, sizeof(Control_Motion));
auto Queue_Light  = xQueueCreate(2, sizeof(Control_Light));
auto Queue_System = xQueueCreate(2, sizeof(Control_System));
auto Queue_Tof    = xQueueCreate(2, sizeof(Data_Tof));

// Applications to use
auto lisii = Serial_Transceiver(&huart1);
auto tof   = Serial_Transceiver(&huart5);

// Message caches
char data[30];
char data_tof_temp[30];

void Model_Debug();
void Task_A();
void Task_B();
void Task_C();