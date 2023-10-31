#include "cmsis_os.h"
#include "queue.h"
#include "string.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Application/Motion_Controller.hh"
#include "../Basic/Message_Type.hh"
#include "../Utility/Utility.hh"

extern "C" {
void Lisii_Loop();
}

extern QueueHandle_t Queue_Opencv;
extern QueueHandle_t Queue_System;
extern QueueHandle_t Queue_Tof;
extern QueueHandle_t Queue_Motion;
extern QueueHandle_t Queue_Offset;

// Serial port initiallize
auto lisii = Serial_Transceiver(&huart5);

void Lisii_Loop()
{
    // Data cache
    static Data_OpenCV data_opencv_temp;
    static Data_Motion data_motion_temp;
    static Data_System data_system_temp;
    static Data_Tof data_tof_temp;

    for (;;) {

        lisii.Recevice_A();

        if (lisii.Get_Status() == OK) {
            lisii.Set_Wait();

            switch (lisii.Get_Data_Type()) {
            case 'R':
                // Load the data from char to int * 4 in data_opencv
                Utility_Get_Data_OpenCV(lisii.Get_Data(), &data_opencv_temp);

                // send message to main handler "Messager"
                xQueueSend(Queue_Opencv, &data_opencv_temp, 5);

                xQueueSend(Queue_Offset, &(data_opencv_temp.flag_offset), 5);

                break;

            case 'D':
                // Repeat data from serial port
                lisii.Send(lisii.Get_Data(), lisii.Get_Data_Size());
                lisii.Send((char*)"\n", 1);
                break;

            case 'S':
                Utility_Get_Data_System(lisii.Get_Data(), &data_system_temp);
                xQueueSend(Queue_System, &data_system_temp, 5);
                break;

            case 'T':
                xQueueReceive(Queue_Tof, &data_tof_temp, 100);
                Show_Data_Tof(data_tof_temp, lisii);
                break;

            case 'M':
                Utility_Get_Data_Motion(lisii.Get_Data(), &data_motion_temp);
                xQueueSend(Queue_Motion, &data_motion_temp, 100);
                break;

            default:
                lisii.Send((char*)"Invalid data!\n", sizeof("Invalid data!"));
                break;
            }
        }

        osDelay(50);
    }
}
