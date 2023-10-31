#include "Utility.hh"

void Utility_Next(char** position_ptr)
{
    for (; **position_ptr != '_'; (*position_ptr)++)
        ;

    (*position_ptr)++;
}

void Utility_Get_Data_OpenCV(char* data, Data_OpenCV* opencv)
{
    char* position = data;

    Utility_Next(&position);
    opencv->flag_turn = atoi(position);

    Utility_Next(&position);
    opencv->flag_slow = atoi(position);

    Utility_Next(&position);
    opencv->flag_stop = atoi(position);

    Utility_Next(&position);
    opencv->flag_offset = atoi(position);
}

void Utility_Get_Data_System(char* data, Data_System* system)
{
    char* position = data;

    Utility_Next(&position);
    system->factor_p_m = atof(position);

    Utility_Next(&position);
    system->factor_i_m = atof(position);

    Utility_Next(&position);
    system->factor_d_m = atof(position);

    Utility_Next(&position);
    system->factor_p_s = atof(position);

    Utility_Next(&position);
    system->factor_i_s = atof(position);

    Utility_Next(&position);
    system->factor_d_s = atof(position);

    Utility_Next(&position);
    system->encode_max = atof(position);

    Utility_Next(&position);
    system->offset_max = atoi(position);

    Utility_Next(&position);
    system->model_pid = atoi(position);
}

void Utility_Get_Data_Motion(char* data, Data_Motion* motion)
{
    char* position = data;

    Utility_Next(&position);
    motion->speed = atof(position);

    Utility_Next(&position);
    motion->direction = atof(position);
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

    serial.Send((char*)send, strlen(send));

    osDelay(5);
}

void Show_Data_Tof(Data_Tof data, Serial_Transceiver serial)
{
    char send[5];

    sprintf(send, "%d\n", data.distance);

    serial.Send(send, strlen(send));

    osDelay(5);
}

void Show_Data_System(Data_System data, Serial_Transceiver serial)
{
    char send[50];

    sprintf(send,
        "%.2f-%.2f-%.2f-%.2f-%.2f-%.2f-%.2f-%d-%d\n",
        data.factor_p_m,
        data.factor_i_m,
        data.factor_d_m,
        data.factor_p_s,
        data.factor_i_s,
        data.factor_d_s,
        data.encode_max,
        data.offset_max,
        data.model_pid);

    serial.Send(send, strlen(send));

    osDelay(50);
}