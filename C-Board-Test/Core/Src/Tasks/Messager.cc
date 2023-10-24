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

// The Model and function
enum Model {
    MODEL_NORMAL,
    MODEL_CRUISE,
    MODEL_DEBUG
};

uint8_t model = MODEL_NORMAL;

void Model_Normal();
void Model_Debug();
void Model_Cruise();

// Queue to use
auto Queue_Breath = xQueueCreate(3, sizeof(Control_Led));
auto Queue_Motion = xQueueCreate(3, sizeof(Control_Motion));
auto Queue_Light  = xQueueCreate(3, sizeof(Control_Light));
auto Queue_System = xQueueCreate(3, sizeof(Control_System));
auto Queue_Tof    = xQueueCreate(3, sizeof(float));

// Applications to use
static auto Lisii        = Serial_Transceiver(&huart1);
static auto data_handler = Data_Handler();

static char data[25];
static float value_tof;
static Control_Led control_led;
static Control_Motion control_motion;
static Control_Light control_light;
static Control_System control_system;

// Task
void Messager_Loop()
{
    for (;;) {

        switch (model) {
            case MODEL_NORMAL:
                Model_Normal();
                break;

            case MODEL_DEBUG:
                Model_Debug();
                break;

            case MODEL_CRUISE:
                Model_Cruise();
                break;
        }
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1) {
        Lisii.Set_Ok();
    }
}

// Loop function in the Normal Model
void Model_Normal()
{
    Lisii.Recevice((uint8_t *)data, sizeof(data));

    if (Lisii.Get_Status() == OK) {
        Lisii.Send((uint8_t *)&"Receive OK\n", 11);
        Lisii.Recevice((uint8_t *)data, 11);
        Lisii.Set_Wait();
        data_handler.Load_Serial(data);
    }
    if (xQueueReceive(Queue_Tof, &value_tof, 0) == pdPASS)
        data_handler.Load_Tof(value_tof);

    data_handler.Led_Handle(&control_led);
    data_handler.Motion_Handle(&control_motion);
    data_handler.Light_Handle(&control_light);
    data_handler.System_Handle(&control_system);

    xQueueSend(Queue_Breath, &control_led, 0);
    xQueueSend(Queue_Motion, &control_motion, 0);
    xQueueSend(Queue_Light, &control_light, 0);
    xQueueSend(Queue_System, &control_system, 0);

    osDelay(10);
}

// Loop function in the Debug Model
void Model_Debug()
{
    osDelay(10);
}

// Loop function in the Cruise Model
void Model_Cruise()
{
    osDelay(10);
}