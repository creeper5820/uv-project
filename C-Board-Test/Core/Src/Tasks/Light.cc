#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"

extern "C" {
void Light_Loop();
}

void Light_Loop()
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}