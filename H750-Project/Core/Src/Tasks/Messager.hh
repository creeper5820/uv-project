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
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
}

static int model = TASK_A;

// Queue to use
auto Queue_Motion = xQueueCreate(5, sizeof(Control_Motion));
auto Queue_Light  = xQueueCreate(5, sizeof(Control_Light));
auto Queue_System = xQueueCreate(5, sizeof(Control_System));
auto Queue_Tof    = xQueueCreate(5, sizeof(Data_Tof));
auto Queue_Opencv = xQueueCreate(5, sizeof(Data_OpenCV));

// Message caches
Data_OpenCV data_opencv_temp{0, 0, 0, 0};
Data_Tof data_tof_temp{20};

Control_Light control_light_temp{DARK_ALL, PREPARE};
Control_Motion control_motion_temp{0, 0};

extern Serial_Transceiver lisii;

void Begin();
void Task_A();
void Task_B();
void Task_C();
void Model_Debug();