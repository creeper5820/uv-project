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
extern QueueHandle_t Queue_System;
extern QueueHandle_t Queue_Offset;
extern QueueHandle_t Queue_Margin;

extern Serial_Transceiver lisii;

auto motion = Motion_Controller();

void Motion_Loop()
{
    static Data_System data_system{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static Data_Motion data_motion{ 0, 0 };
    static Data_Margin data_margin{ 0, 0 };

    // S_0_0_0_0_0_0_2430_300_0

    data_system.factor_p_m = 800;
    data_system.factor_i_m = 180;
    data_system.factor_d_m = 40;

    data_system.factor_p_s = 20;
    data_system.factor_i_s = 0;
    data_system.factor_d_s = 0;

    data_system.encode_max = 5000;
    data_system.offset_max = 300;
    data_system.model_pid = 1;

    // M_0_0_E

    data_motion.speed = 0;
    data_motion.distance = 40;

    motion.Load_System(&data_system);

    motion.Init();

    for (;;) {
        if (xQueueReceive(Queue_System, &data_system, 0) == pdTRUE)
            motion.Load_System(&data_system);

        if (xQueueReceive(Queue_Motion, &data_motion, 0) == pdTRUE)
            motion.Load_Motion(&data_motion);

        if (xQueueReceive(Queue_Margin, &data_margin, 0) == pdTRUE) {
            motion.Load_Margin(&data_margin);
        }

        motion.Load();

        if (0) {
            char send[40];
            int size = sprintf(send, "speed: %.2f,%.2f\n",
                motion.Read_Speed(0), motion.Read_Speed(1));

            lisii.Send(send, size);
        }

        if (0)
            Show_Data_System(motion.system_, lisii);

        if (0)
            Show_Data_Margin(data_margin, lisii);

        if (0) {
            char send_distance[20];
            int size_distance = sprintf(send_distance,
                "distance: %.2f\n", motion.Get_Distance());
            lisii.Send(send_distance, size_distance);
        }

        osDelay(5);
    }
}