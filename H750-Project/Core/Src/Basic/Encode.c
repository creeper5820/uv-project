#include "Encode.h"

/**************************************************
 * @brief 编码器初始化
 */
void Encoder_Init()
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

/**************************************************
 * @brief 读编码器
 * @param TIMX  3,4,代表定时器3和定时器4
 */
int Read_Encoder(uint8_t TIMX)
{
    int Encoder_TIM;
    switch (TIMX) {

        case 4: {
            Encoder_TIM = (short)TIM4->CNT;
            TIM4->CNT   = 0;
            break;
        }

        case 3: {
            Encoder_TIM = (short)TIM3->CNT;
            TIM3->CNT   = 0;
            break;
        }

        default: {
            Encoder_TIM = 0;
        }
    }

    return Encoder_TIM;
}
