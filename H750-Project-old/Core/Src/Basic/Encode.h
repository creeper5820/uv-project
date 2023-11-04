#pragma once

#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

int Read_Encoder(uint8_t TIMX);
void Encoder_Init();
// void Until_Distance(int32_t distance);

#ifdef __cplusplus
}
#endif
