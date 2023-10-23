#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"
#include "main.h"

#include "../Controller/Serial_Transceiver.hh"
#include "../Utility/Utility.h"

/**
 * @brief Override the function
 */
extern "C" {
void Messager_Loop();
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
}

/**
 * @brief Task
 */
auto Lisii = Serial_Transceiver(&huart1);

void Messager_Loop()
{
    char data[50];
    Lisii.Recevice((uint8_t *)data, sizeof(data));

    for (;;) {
        if (Lisii.Get_Status() == OK) {
            Lisii.Send((uint8_t *)&"Receive OK\n", 11);

            Data_OpenCV data_opencv_temp;
            Utility_Get_Data_OpenCV(data, &data_opencv_temp);

            Lisii.Recevice((uint8_t *)data, sizeof(data));
            Lisii.Set_Wait();
        }

        osDelay(50);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1) {
        Lisii.Set_Ok();
    }
}
