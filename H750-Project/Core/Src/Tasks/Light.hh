#pragma once

#include "cmsis_os.h"
#include "queue.h"
#include "../Application/Led_Controller.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Light_Loop();
}

extern QueueHandle_t Queue_Light;

auto leds          = Led_Group();
auto control_light = Control_Light{};

void Wait_Start();
void Wait_Tof();
void Light_All();
void Dark_All();
void Flash_All();
void Light_Left();
void Light_Right();
void Light_Head();
void Light_Tail();
