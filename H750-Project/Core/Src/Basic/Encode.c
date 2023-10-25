#include "Encode.h"

// Read_Encoder函数放入定时器一直读，让encoderSum一直加，当有需要距离时候，将encoderSum清零

int Encoder_Left  = 0;
int Encoder_Right = 0; //左右编码器的脉冲计数

int64_t encoderSum = 0;

/**************************************************
 * @brief 编码器初始化
 */
void Encoder_Init(TIM_HandleTypeDef *htim_1, TIM_HandleTypeDef *htim_2)
{
    HAL_TIM_Encoder_Start(htim_1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(htim_2, TIM_CHANNEL_ALL);
}

/**************************************************
 * @brief 读编码器
 * @param TIMX:3,4     代表定时器3和定时器4
 */
int Read_Encoder(uint8_t TIMX)
{
    int Encoder_TIM;
    switch (TIMX) {
        case 4:
            Encoder_TIM = (short)TIM4->CNT;
            TIM4->CNT   = 0;
            break;
        case 3:
            Encoder_TIM = (short)TIM3->CNT;
            TIM3->CNT   = 0;
            break;
        default:
            Encoder_TIM = 0;
    }
    return Encoder_TIM;
}
