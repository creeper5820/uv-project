#pragma once

#include "main.h"

struct LED {
    TIM_HandleTypeDef *htim;
    uint32_t Channel;
};
typedef struct LED LED_Struct;

void Set_LED(LED_Struct led, float brightness)
{
    uint32_t value = (float)led.htim->Init.Period * brightness;
    __HAL_TIM_SET_COMPARE(led.htim, led.Channel, value);
}