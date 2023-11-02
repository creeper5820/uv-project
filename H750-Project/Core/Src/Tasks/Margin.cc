#include "cmsis_os.h"
#include "queue.h"
#include "string.h"
#include "stdio.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Utility/Utility.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Margin_Loop();
}

extern QueueHandle_t Queue_Margin;

auto margin = Serial_Transceiver(&huart1);

extern Serial_Transceiver lisii;

static Data_Margin data_margin;

void Margin_Loop()
{
    margin.Recevice_A();

    for (;;) {

        if (margin.Get_Status() == OK) {
            margin.Set_Wait();
            Utility_Get_Data_Margin(margin.Get_Data(), &data_margin);
            xQueueOverwrite(Queue_Margin, &data_margin);
        }

        if (0)
            Show_Data_Margin(data_margin, lisii);

        osDelay(5);
    }
}