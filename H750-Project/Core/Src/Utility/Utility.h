#pragma once

#include "string.h"
#include "../Basic/Message_Type.hh"

void Utility_Next(char **position_ptr)
{
    for (; **position_ptr != '_'; (*position_ptr)++)
        ;

    (*position_ptr)++;
}

int Utility_Get_Integer(char *head_ptr)
{
    return atoi(head_ptr);
}

void Utility_Get_Data_OpenCV(char *data, Data_OpenCV *data_opencv)
{
    // R_1_2_0_13_E
    char *position = data;

    Utility_Next(&position);
    data_opencv->flag_turn = Utility_Get_Integer(position);

    Utility_Next(&position);
    data_opencv->flag_slow = Utility_Get_Integer(position);

    Utility_Next(&position);
    data_opencv->flag_stop = Utility_Get_Integer(position);

    Utility_Next(&position);
    data_opencv->offset = Utility_Get_Integer(position);
}

void Utility_Get_Data_System(char *data, Control_System *system)
{
}
