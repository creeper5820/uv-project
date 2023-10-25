#pragma once

#include "../Basic/Message_Type.hh"
#include "../Utility/Utility.h"

class Data_Handler
{
public:
    Data_OpenCV data_opencv_;
    Data_Tof data_tof_;
    Control_System system_;

public:
    Data_Handler()
        : data_opencv_(Data_OpenCV{}),
          data_tof_(Data_Tof{})
    {
    }

    void Load_Serial(char *data)
    {
        Utility_Get_Data_OpenCV(data, &data_opencv_);
        Utility_Get_Data_System(data, &system_);
    }

    void Load_Tof(float value)
    {
    }

    void Motion_Handle(Control_Motion *motion)
    {
    }

    void Light_Handle(Control_Light *light)
    {
    }

    void Led_Handle(Control_Led *led)
    {
        auto control = Control_Led{1, 0, 0, 0};

        if (data_opencv_.flag_turn == 1)
            control.light[1] = 1;

        if (data_opencv_.flag_slow == 1)
            control.light[2] = 1;

        if (data_opencv_.flag_stop == 1)
            control.light[3] = 1;

        *led = control;
    }

    void System_Handle(Control_System *system)
    {
    }
};
