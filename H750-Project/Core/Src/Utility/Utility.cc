#include "Utility.hh"

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
    data_opencv->flag_offset = Utility_Get_Integer(position);
}

void Utility_Get_Data_System(char *data, Control_System *system)
{
}

// the utility to show data_opencv
void Show_Data_OpenCV(Data_OpenCV data, Serial_Transceiver serial)
{
    char send[50];

    sprintf(send, "%d, %d, %d, %d\n",
            data.flag_turn,
            data.flag_slow,
            data.flag_stop,
            data.flag_offset);

    serial.Send((char *)send, strlen(send));

    osDelay(5);
}

void Show_Data_Tof(Data_Tof data, Serial_Transceiver serial)
{
    char send[5];

    sprintf(send, "%d\n", data.distance);

    serial.Send(send, strlen(send));

    osDelay(5);
}
