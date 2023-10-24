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

extern QueueHandle_t Breath_Queue;

void Breath_LED_Loop()
{
    Control_Led led;
    auto rgb = RGB_Controller(
        {&htim5, TIM_CHANNEL_3},
        {&htim5, TIM_CHANNEL_2},
        {&htim5, TIM_CHANNEL_1});

    for (;;) {
        xQueueReceive(Breath_Queue, &led, portMAX_DELAY);

        rgb.Set_Color(
            led.R,
            led.G,
            led.B);
    }
}
