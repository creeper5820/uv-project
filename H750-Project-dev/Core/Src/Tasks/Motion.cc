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
extern QueueHandle_t Queue_Offset;
extern Serial_Transceiver lisii;

auto motion = Motion_Controller();

static Data_Motion data_motion{ 0, 0 };

void Motion_Loop()
{

    // M_0_0_E

    data_motion.speed = 0;
    data_motion.distance = 40;

    motion.Init();

    for (;;) {
        if (xQueueReceive(Queue_Motion, &data_motion, 0) == pdTRUE)
            motion.Load_Motion(&data_motion);

        motion.Load();

        if (0) {
            char send_distance[20];
            int size_distance = sprintf(send_distance,
                "distance: %.2f\n", motion.Get_Distance());
            lisii.Send(send_distance, size_distance);
        }

        if (0) {
            char send_speed[30];
            int size_speed = sprintf(send_speed,
                "speed:%.2f,%.2f,%.2f\n", motion.motion_.speed, motion.speed_left_now_, motion.speed_right_now_);
            lisii.Send(send_speed, size_speed);
        }

        if (0)
            Show_Data_Attitude(motion.attitude_, lisii);

        osDelay(50);
    }
}