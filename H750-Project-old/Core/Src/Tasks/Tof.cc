#include "cmsis_os.h"
#include "queue.h"
#include "string.h"
#include "stdio.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Application/Motion_Controller.hh"
#include "../Utility/Utility.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Tof_Loop();
}

extern QueueHandle_t Queue_Tof;

auto tof = Serial_Transceiver(&hlpuart1);

extern Serial_Transceiver lisii;

static Data_Tof data_tof;

void Tof_Loop()
{
    // tof.Recevice_C();

    for (;;) {

        tof.Recevice_D();

        data_tof.distance = atoi(tof.Get_Data());

        xQueueOverwrite(Queue_Tof, &data_tof);

        osDelay(5);
    }
}
