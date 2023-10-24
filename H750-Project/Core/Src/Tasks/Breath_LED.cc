#include "cmsis_os.h"
#include "../../Middlewares/Third_Party/FreeRTOS/Source/include/queue.h"

#include "main.h"
#include "tim.h"
#include "gpio.h"

#include "../Application/Led_Controller.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Breath_LED_Loop();
}

extern QueueHandle_t Queue_Breath;

void Breath_LED_Loop()
{
    // Control_Led led;
    // auto rgb = RGB_Controller(
    //     {&htim5, TIM_CHANNEL_3},
    //     {&htim5, TIM_CHANNEL_2},
    //     {&htim5, TIM_CHANNEL_1});

    for (;;) {

        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_RESET);
        osDelay(1000);

        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, GPIO_PIN_SET);
        osDelay(1000);

        // xQueueReceive(Queue_Breath, &led, portMAX_DELAY);

        // rgb.Set_Color(
        //     led.R,
        //     led.G,
        //     led.B);
    }
}
