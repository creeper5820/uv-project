#include "cmsis_os.h"
#include "queue.h"
#include "string.h"
#include "stdio.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Application/Motion_Controller.hh"
#include "../Utility/Utility.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Attitude_Loop();
}

extern QueueHandle_t Queue_Attitude;
extern Motion_Controller motion;
extern Serial_Transceiver lisii;

auto attitude = Serial_Transceiver(&huart5);

static Data_Attitude data_attitude;

void Attitude_Loop()
{
    // attitude.Recevice_A();

    for (;;) {
        attitude.Recevice_B();

        osDelay(20);

        // if (attitude.Get_Status() == OK) {
        //     attitude.Set_Wait();

        Utility_Get_Data_Attitude(attitude.Get_Data(), &data_attitude);
        motion.Load_Attitude(&data_attitude);

        // xQueueOverwrite(Queue_Margin, &data_margin);
        // }

        if (0)
            Show_Data_Attitude(data_attitude, lisii);

        osDelay(20);

        attitude.Set_Empty();
    }
}