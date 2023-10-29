#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Tof_Loop();
}

extern QueueHandle_t Queue_Tof;

auto tof = Serial_Transceiver(&huart5);
static Data_Tof data_tof;

void Tof_Loop()
{
    for (;;) {
        tof.Recevice_A();
        data_tof.distance = atoi(tof.Get_Data());

        if (data_tof.distance < 250 + 1 && data_tof.distance != 0) {
            xQueueSend(Queue_Tof, &data_tof, 0);
        }

        osDelay(100);
    }
}