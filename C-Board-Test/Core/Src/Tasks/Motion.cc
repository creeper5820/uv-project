#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"

extern "C" {
void Motion_Loop();
}

void Motion_Loop()
{

    /**
     * @brief The goal this controller would touch
     */

    float speed     = 0; // -1.0 < speed < 1.0
    float direction = 0; // -1.0 < direcrion < 1.0

    for (;;) {
        // receive message from queue

        // handle the message and modify the goal

        // use controller and pid to achieve this goal

        osDelay(1000);
    }
}