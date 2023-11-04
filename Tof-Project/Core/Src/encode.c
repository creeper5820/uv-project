// #include "encode.h"

// #define SPEED_RECORD_NUM 20 //自己测试多少次
// float test_Speed = 0;

// Motor motor1;
// Motor motor2;

// void Encoder_Init(void)
// {
//     HAL_TIM_Encoder_Start(&ENCODER_TIM_1, TIM_CHANNEL_ALL); //开启编码器定时器
//     HAL_TIM_Encoder_Start(&ENCODER_TIM_2, TIM_CHANNEL_ALL);

//     __HAL_TIM_ENABLE_IT(&ENCODER_TIM_1, TIM_IT_UPDATE); //开启编码器定时器更新中断,防溢出处理
//     __HAL_TIM_ENABLE_IT(&ENCODER_TIM_2, TIM_IT_UPDATE);

//     HAL_TIM_Base_Start_IT(&GAP_TIM); //开启100ms定时器中断
//     __HAL_TIM_SET_COUNTER(&ENCODER_TIM_1, 10000); //编码器定时器初始值设定为10000
//     __HAL_TIM_SET_COUNTER(&ENCODER_TIM_2, 10000);

//     motor1.lastCount = 0; //结构体内容初始化
//     motor1.totalCount = 0;
//     motor1.overflowNum = 0;
//     motor1.speed = 0;
//     motor1.direct = 0;

//     motor2.lastCount = 0;
//     motor2.totalCount = 0;
//     motor2.overflowNum = 0;
//     motor2.speed = 0;
//     motor2.direct = 0;
// }

// float speed_Record_1[SPEED_RECORD_NUM] = { 0 };
// float speed_Record_2[SPEED_RECORD_NUM] = { 0 };

// /*
//  * 进行速度的平均滤波
//  * 输入新采样到的速度，存放速度的数组，
//  * 返回滤波后的速度
//  */
// float Speed_Low_Filter(float new_Spe, float* speed_Record)
// {
//     float sum = 0.0f;
//     test_Speed = new_Spe;
//     for (uint8_t i = SPEED_RECORD_NUM - 1; i > 0; i--) //将现有数据后移一位
//     {
//         speed_Record[i] = speed_Record[i - 1];
//         sum += speed_Record[i - 1];
//     }
//     speed_Record[0] = new_Spe; //第一位是新的数据
//     sum += new_Spe;
//     test_Speed = sum / SPEED_RECORD_NUM;
//     return sum / SPEED_RECORD_NUM; //返回均值
// }

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) //定时器回调函数，用于计算速度
// {

//     if (htim->Instance == GAP_TIM.Instance) //间隔定时器中断，是时候计算速度了
//     {
//         motor1.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER_TIM_1); //如果向上计数（正转），返回值为0，否则返回值为1
//         motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; //一个周期内的总计数值等于目前计数值加上溢出的计数值

//         motor2.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER_TIM_2);
//         motor2.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_2;

//         if (motor1.lastCount - motor1.totalCount > 19000) // 在计数值溢出时进行防溢出处理
//         {
//             motor1.overflowNum++;
//             motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; //一个周期内的总计数值等于目前计数值加上溢出的计数值
//         }
//         else if (motor1.totalCount - motor1.lastCount > 19000) // 在计数值溢出时进行防溢出处理
//         {
//             motor1.overflowNum--;
//             motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; //一个周期内的总计数值等于目前计数值加上溢出的计数值
//         }

//         if (motor2.lastCount - motor2.totalCount > 19000) // 在计数值溢出时进行防溢出处理
//         {
//             motor2.overflowNum++;
//             motor2.totalCount = COUNTERNUM_2 + motor1.overflowNum * RELOADVALUE_2; //一个周期内的总计数值等于目前计数值加上溢出的计数值
//         }
//         else if (motor2.totalCount - motor2.lastCount > 19000) // 在计数值溢出时进行防溢出处理
//         {
//             motor2.overflowNum--;
//             motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; //一个周期内的总计数值等于目前计数值加上溢出的计数值
//         }

//         motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / PULSE_PRE_ROUND; //算得每秒多少转,除以4是因为4倍频
//         motor2.speed = (float)(motor2.totalCount - motor2.lastCount) / PULSE_PRE_ROUND; //算得每秒多少转,除以4是因为4倍频

//         /*******************在这里添加滤波函数************************/
//         motor1.speed = Speed_Low_Filter(motor1.speed, speed_Record_1);
//         motor2.speed = Speed_Low_Filter(motor2.speed, speed_Record_2);

//         /**********************************************************/
//         motor1.lastCount = motor1.totalCount; //记录这一次的计数值
//         motor2.lastCount = motor2.totalCount; //记录这一次的计数值
//     }
// }

#include "Encode.h"

/**************************************************
 * @brief 编码器初始化
 */
void Encoder_Init()
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

/**************************************************
 * @brief 读编码器
 * @param TIMX  3,4,代表定时器3和定时器4
 */
int Read_Encoder(uint8_t TIMX)
{
    int Encoder_TIM;

    switch (TIMX) {

    case 0: {
        Encoder_TIM = (short)TIM2->CNT;
        TIM2->CNT = 0;
        break;
    }

    case 1: {
        Encoder_TIM = (short)TIM3->CNT;
        TIM3->CNT = 0;
        break;
    }

    default: {
        Encoder_TIM = 0;
    }
    }

    return Encoder_TIM;
}