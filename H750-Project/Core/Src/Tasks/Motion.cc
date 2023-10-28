#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"
#include "queue.h"

#include "../Basic/Message_Type.hh"
#include "../Basic/Servo.h"
#include "../Basic/Motor.h"
#include "../Application/Motion_Controller.hh"
#include "../Application/Serial_Transceiver.hh"

extern "C" {
void Motion_Loop();
}

extern QueueHandle_t Queue_Motion;
extern Serial_Transceiver lisii;

static Control_System contorl_system{850, 0.0, 1, 2430, 1};
static Control_Motion control_motion{0, 0};

static auto motion_controller = Motion_Controller(&contorl_system);

void Motion_Loop()
{
    motion_controller.Init();

    for (;;) {
        xQueueReceive(Queue_Motion, &control_motion, 50);

        motion_controller.Load_Target(&control_motion);

        osDelay(50);
    }
}