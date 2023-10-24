#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"

extern "C" {
void Light_Loop();
}

void Light_Loop()
{
    for (;;) {
        osDelay(1);
    }
}