#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "gpio.h"
#include "queue.h"

#include "../Basic/Message_Type.hh"
#include "../Basic/Servo.h"
#include "../Basic/Motor.h"
#include "../Utility/Utility.hh"
#include "../Application/Motion_Controller.hh"
#include "../Application/Serial_Transceiver.hh"

extern "C" {
void Motion_Loop();
}

extern QueueHandle_t Queue_Motion;
extern QueueHandle_t Queue_Opencv;
extern Serial_Transceiver lisii;

static Control_System contorl_system{0, 850, 0.0, 1, 2430, 300};
static Control_Motion control_motion{0, 0};
static Data_OpenCV data_opencv;

auto motion_controller = Motion_Controller(&contorl_system);

void Motion_Loop()
{
    contorl_system.offset_max    = 300;
    contorl_system.factor_encode = 2430;
    contorl_system.model_pid     = 0;

    motion_controller.Init();

    for (;;) {
        if (xQueueReceive(Queue_Motion, &control_motion, 100) == pdPASS)
            lisii.Send((char *)"Receive Motion\n", sizeof("Receive Motion"));

        if (xQueueReceive(Queue_Opencv, &data_opencv, 100) == pdPASS)
            Show_Data_OpenCV(data_opencv, lisii);

        control_motion.direction -= (float)data_opencv.flag_offset / contorl_system.offset_max;

        if (control_motion.direction > 1.0)
            control_motion.direction = 1.0;

        if (control_motion.direction < -1.0)
            control_motion.direction = -1.0;

        motion_controller.Load_Target(&control_motion);

        osDelay(50);
    }
}