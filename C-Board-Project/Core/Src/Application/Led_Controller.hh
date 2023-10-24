#pragma once

#include "main.h"
#include "../Basic/Message_Type.hh"

class RGB_Controller
{
private:
    LED_Struct LED_R_;
    LED_Struct LED_G_;
    LED_Struct LED_B_;

    float brightness_R;
    float brightness_G;
    float brightness_B;

public:
    RGB_Controller(LED_Struct LED_R, LED_Struct LED_G, LED_Struct LED_B)
        : LED_R_(LED_R),
          LED_G_(LED_G),
          LED_B_(LED_B)
    {
        brightness_R = 0;
        brightness_G = 0;
        brightness_B = 0;
    }

    void Set_LED(LED_Struct led, float brightness)
    {
        uint32_t value = (float)led.htim->Init.Period * brightness;
        __HAL_TIM_SET_COMPARE(led.htim, led.Channel, value);
    }

    void Set_Color(float R, float G, float B)
    {
        Set_LED(LED_R_, R);
        Set_LED(LED_G_, G);
        Set_LED(LED_B_, B);
    }
};