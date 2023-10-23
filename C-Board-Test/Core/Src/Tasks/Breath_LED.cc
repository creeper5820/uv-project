#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"

#include "../Controller/LED_Controller.hh"

extern "C" {
void Breath_LED_Loop();
}

int status_breath = 1;

LED_Struct LED_1 = {&htim5, TIM_CHANNEL_1};
LED_Struct LED_2 = {&htim5, TIM_CHANNEL_2};
LED_Struct LED_3 = {&htim5, TIM_CHANNEL_3};

void Breath_LED_Loop()
{
    /* USER CODE BEGIN Breath_LED_Fun */
    int time_delay = 20;
    /* Infinite loop */
    for (;;) {
        if (status_breath == 0)
            break;

        for (float count = 0; count < 1; count += 0.01) {
            osDelay(time_delay);
            Set_LED(LED_1, count);
            Set_LED(LED_2, count);
            Set_LED(LED_3, count);
        }

        for (float count = 1; count > 0; count -= 0.01) {
            osDelay(time_delay);
            Set_LED(LED_1, count);
            Set_LED(LED_2, count);
            Set_LED(LED_3, count);
        }
    }
    /* USER CODE END Breath_LED_Fun */
}