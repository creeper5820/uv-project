#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"
#include "../Utility/Utility.hh"

extern "C" {
void Lisii_Loop();
}

extern QueueHandle_t Queue_Opencv;
extern QueueHandle_t Queue_System;

// Serial port initiallize
auto lisii = Serial_Transceiver(&huart1);

// Data cache
static Data_OpenCV data_opencv;
static Control_System control_system_temp;

void Lisii_Loop()
{
    for (;;) {

        lisii.Recevice_A();

        if (lisii.Get_Status() == OK) {
            lisii.Set_Wait();

            switch (lisii.Get_Data_Type()) {
                case 'R':
                    // Load the data from char to int * 4 in data_opencv
                    Utility_Get_Data_OpenCV(lisii.Get_Data(), &data_opencv);

                    // send message to main handler "Messager"
                    xQueueSend(Queue_Opencv, &data_opencv, 5);

                    break;

                case 'D':
                    // Repeat data from serial port
                    lisii.Send(lisii.Get_Data(), lisii.Get_Data_Size());
                    lisii.Send((char *)"\n", 1);
                    break;

                case 'S':
                    Utility_Get_Data_System(lisii.Get_Data(), &control_system_temp);
                    xQueueSend(Queue_System, &control_system_temp, 5);
                    break;

                default:
                    lisii.Send((char *)"Invalid data!\n", sizeof("Invalid data!"));
                    break;
            }
        }

        osDelay(50);
    }
}
