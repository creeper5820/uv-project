#include "cmsis_os.h"
#include "../../Middlewares/Third_Party/FreeRTOS/Source/include/queue.h"

#include "tim.h"
#include "gpio.h"
#include "main.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Utility/Utility.h"
#include "../Basic/Message_Type.hh"

// Override the function
extern "C" {
void Messager_Loop();
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
}

// Use lisii to send and receive message from serial port
auto Lisii = Serial_Transceiver(&huart1);

// Queue to use
auto Breath_Queue = xQueueCreate(3, sizeof(int8_t));
auto Motion_Queue = xQueueCreate(3, sizeof(Data_OpenCV));

// Task
void Messager_Loop()
{
    char data[50];
    Lisii.Recevice((uint8_t *)data, sizeof(data));
    uint8_t flag_led;

    for (;;) {

        // Receive and handle the message from lisii
        if (Lisii.Get_Status() == OK) {
            Lisii.Send((uint8_t *)&"Receive OK\n", 11);

            switch (data[0]) {
                case 'R':
                    Data_OpenCV data_opencv_temp;
                    Utility_Get_Data_OpenCV(data, &data_opencv_temp);

                    flag_led = 0;
                    xQueueSend(Breath_Queue, &flag_led, 0);

                    break;

                case 'D':
                    Factor_System factor_system;
                    Utility_Get_Factor_System(data, &factor_system);

                    flag_led = 1;
                    xQueueSend(Breath_Queue, &flag_led, 0);

                    break;

                default:
                    break;
            }

            Lisii.Recevice((uint8_t *)data, sizeof(data));
            Lisii.Set_Wait();
        }

        // Receive and handle the message form sensor

        // Collect logs form queue and send them to lisii

        osDelay(500);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1) {
        Lisii.Set_Ok();
    }
}
