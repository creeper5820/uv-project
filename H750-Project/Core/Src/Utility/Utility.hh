#pragma once

#include "stdio.h"
#include "string.h"
#include "cmsis_os.h"
#include "../Basic/Message_Type.hh"
#include "../Application/Serial_Transceiver.hh"

int Utility_Get_Integer(char *head_ptr);
void Utility_Next(char **position_ptr);
void Utility_Get_Data_OpenCV(char *data, Data_OpenCV *data_opencv);
void Utility_Get_Data_System(char *data, Control_System *system);

// the utility to show data_opencv
void Show_Data_OpenCV(Data_OpenCV data, Serial_Transceiver lisii);
void Show_Data_Tof(Data_Tof data, Serial_Transceiver serial);
