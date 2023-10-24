#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"
#include "queue.h"
#include "../Basic/Message_Type.hh"

extern "C" {
void Motion_Loop();
}

extern QueueHandle_t Queue_Motion;

void Motion_Loop()
{
    float speed     = 0; // -1.0 < speed < 1.0
    float direction = 0; // -1.0 < direcrion < 1.0

    
    Control_System Contorl_Params
    {
        0, 0, 0
    };

    for (;;) {
        // receive message from queue


        // handle the message and modify the goal


        // use controller and pid to achieve this goal


        osDelay(1000);
    }
}