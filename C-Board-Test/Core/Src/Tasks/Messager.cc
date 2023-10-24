#include "cmsis_os.h"
#include "../../Middlewares/Third_Party/FreeRTOS/Source/include/queue.h"

#include "tim.h"
#include "gpio.h"
#include "main.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Application/Data_Handler.hh"
#include "../Basic/Message_Type.hh"

// Override the function
extern "C" {
void Messager_Loop();
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
}

// Queue to use
auto Breath_Queue = xQueueCreate(3, sizeof(Control_Led));
auto Motion_Queue = xQueueCreate(3, sizeof(Control_Motion));
auto Light_Queue  = xQueueCreate(3, sizeof(Control_Light));
auto System_Queue = xQueueCreate(3, sizeof(Control_System));
auto Tof_Queue    = xQueueCreate(3, sizeof(float));

// Applications to use
auto Lisii        = Serial_Transceiver(&huart1);
auto data_handler = Data_Handler();

// Task
void Messager_Loop()
{
    char data[25];
    float value_tof;
    Lisii.Recevice((uint8_t *)data, sizeof(data));

    Control_Led led;
    Control_Motion motion;
    Control_Light light;
    Control_System system;

    for (;;) {

        if (Lisii.Get_Status() == OK) {
            Lisii.Send((uint8_t *)&"Receive OK\n", 11);
            Lisii.Recevice((uint8_t *)data, sizeof(data));
            Lisii.Set_Wait();
            data_handler.Load_Serial(data);
        }
        if (xQueueReceive(Tof_Queue, &value_tof, 0) == pdPASS)
            data_handler.Load_Tof(value_tof);

        data_handler.Led_Handle(&led);
        data_handler.Motion_Handle(&motion);
        data_handler.Light_Handle(&light);
        data_handler.System_Handle(&system);

        xQueueSend(Breath_Queue, &led, 0);
        xQueueSend(Motion_Queue, &motion, 0);
        xQueueSend(Light_Queue, &light, 0);
        xQueueSend(System_Queue, &system, 0);

        osDelay(50);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1) {
        Lisii.Set_Ok();
    }
}
