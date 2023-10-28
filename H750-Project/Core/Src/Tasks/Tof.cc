#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Tof_Loop();
}

extern Serial_Transceiver tof;
extern Serial_Transceiver lisii;
extern char data_tof_temp[10];
extern QueueHandle_t Queue_Tof;

static Data_Tof data_tof;

void Tof_Loop()
{
    for (;;) {
        tof.Recevice_A((uint8_t *)data_tof_temp, sizeof(data_tof_temp));
        data_tof.distance = atoi(data_tof_temp);

        if (data_tof.distance < 200) {
            xQueueSend(Queue_Tof, &data_tof, 0);
        }

        osDelay(100);
    }
}