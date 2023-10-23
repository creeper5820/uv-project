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

uint8_t status_breath = 0;

LED_Struct LED_1 = {&htim5, TIM_CHANNEL_1};
LED_Struct LED_2 = {&htim5, TIM_CHANNEL_2};
LED_Struct LED_3 = {&htim5, TIM_CHANNEL_3};

auto rgb = RGB_Controller(LED_1, LED_2, LED_3);
extern QueueHandle_t Breath_Queue;

void Breath_LED_Loop()
{
    for (;;) {

        xQueueReceive(Breath_Queue, &status_breath, 0);

        switch (status_breath) {
            case 1: {
                rgb.Set_Color(1.0, 1.0, 1.0);
                break;
            }

            case 0: {
                rgb.Set_Color(0.3, 0.7, 0.1);
                break;
            }

            default: {
                break;
            }
        }
    }
}
