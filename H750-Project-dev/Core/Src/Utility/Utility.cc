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
    motion->distance = atof(position);
}

bool Utility_Get_Data_Attitude(char* data, Data_Attitude* attitude)
{
    // 使用标识符检测会导致丢包率异常高

    // if (data[0] != 'A')
    //     return false;

    char* position = data;

    Utility_Next(&position);
    attitude->margin_a = atoi(position);

    Utility_Next(&position);
    attitude->margin_b = atoi(position);

    Utility_Next(&position);
    attitude->speed_right = atoi(position);

    Utility_Next(&position);
    attitude->speed_left = atoi(position);

    return true;
}

// the utility to show data_opencv
void Show_Data_OpenCV(Data_OpenCV data, Serial_Transceiver serial)
{
    char send[50];

    sprintf(send, "%d, %d, %d\n",
        data.flag_turn,
        data.flag_slow,
        data.flag_stop);

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

void Show_Data_Attitude(Data_Attitude data, Serial_Transceiver serial)
{
    char send[50];

    int size = sprintf(
        send,
        "margin: %6d %6d, speed: %6d %6d \n",
        data.margin_a,
        data.margin_b,
        data.speed_left,
        data.speed_right);

    serial.Send(send, size);

    osDelay(50);
}

int Button_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) {
        osDelay(20);
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
            return 1;
    }

    return 0;
}
