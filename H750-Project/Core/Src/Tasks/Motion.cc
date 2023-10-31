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
extern Serial_Transceiver lisii;

void Motion_Loop()
{
    static Data_System data_system{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static Data_Motion data_motion{ 0, 0 };
    static int offset = 0;

    // S_0_0_0_0_0_0_2430_300_0

    data_system.factor_p_m = 120;
    data_system.factor_i_m = 0.1;
    data_system.factor_d_m = 65;

    data_system.factor_p_s = 0;
    data_system.factor_i_s = 0;
    data_system.factor_d_s = 0;

    data_system.encode_max = 4860;
    data_system.offset_max = 300;
    data_system.model_pid = 0;

    // M_0_0_E

    data_motion.speed = 0;
    data_motion.direction = 0;

    auto motion = Motion_Controller(&data_system);

    motion.Init();

    for (;;) {

        if (xQueueReceive(Queue_System, &data_system, 0) == pdTRUE) {
            motion.Load_System(&data_system);

            if (false)
                Show_Data_System(data_system, lisii);
        }

        if (xQueueReceive(Queue_Motion, &data_motion, 0) == pdTRUE)
            motion.Load_Motion(&data_motion);

        if (xQueueReceive(Queue_Offset, &offset, 0) == pdTRUE) {
            motion.Load_Offset(offset);

            if (1) {
                char send_offset[20];
                int size_offset = sprintf(send_offset, "offset: %d", offset);
                lisii.Send(send_offset, size_offset);
            }
        }

        motion.Load();

        // 测速打印
        if (0) {
            char send[40];
            int size = sprintf(send, "speed*100: %.1f,%.1f\n",
                motion.Read_Speed(0) * 100, motion.Read_Speed(1) * 100);

            lisii.Send(send, size);
        }

        if (1) {
            int count = 0;
            TIM2->CNT = 0;

            osDelay(50);

            count = (short)TIM2->CNT;

            char send_count[20];
            int size_count = sprintf(send_count, "count: %d\n", count);
            lisii.Send(send_count, size_count);
        }

        osDelay(50);
    }
}