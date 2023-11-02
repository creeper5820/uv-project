#pragma once

#include "cmsis_os.h"
#include "stdio.h"
#include "math.h"

#include "../Basic/Motor.h"
#include "../Basic/Servo.h"
#include "../Basic/Encode.h"
#include "../Basic/Message_Type.hh"

class Motion_Controller {
public:
    Data_System system_;
    Data_Motion motion_;
    Data_Margin margin_;

    int offset_;

    // PID params of speed
    float bias_ = 0;
    float bias_last_ = 0;
    float bias_integral_ = 0;

    // PID params if distance
    int bias_distance_a_ = 0;
    int bias_distance_last_a_ = 0;
    int bias_distance_integral_a_ = 0;

    int bias_distance_b_ = 0;
    int bias_distance_last_b_ = 0;
    int bias_distance_integral_b_ = 0;

    int bias_distance_ = 0;
    int bias_distance_last_ = 0;
    int bias_distance_integral_ = 0;

    int model_direction = 1;

    int pwm_speed_ = 0;
    int pwm_distance_ = 0;

    float speed_current_ = 0;
    float direction_current_ = 0;

public:
    Motion_Controller()
        : system_()
    {
        motion_ = { 0, 0 };
        offset_ = 0;
    }

    Motion_Controller(Data_System* data)
        : system_(*data)
    {
        motion_ = { 0, 0 };
        offset_ = 0;
    }

    void Init()
    {
        Motor_Init();
        Servo_Init();
        Encoder_Init();
    }

    void Load()
    {
        Set_Direction(motion_.distance);
        Set_Speed(motion_.speed);
    }

    void Load_Motion(Data_Motion* data)
    {
        if (abs(data->speed - 114) > 0.001)
            motion_.speed = data->speed;

        if (abs(data->distance - 114) > 0.001)
            motion_.distance = data->distance;
    }

    void Load_System(Data_System* data)
    {
        if (abs(data->factor_p_m - 114) > 0.001)
            system_.factor_p_m = data->factor_p_m;

        if (abs(data->factor_i_m - 114) > 0.001)
            system_.factor_i_m = data->factor_i_m;

        if (abs(data->factor_d_m - 114) > 0.001)
            system_.factor_d_m = data->factor_d_m;

        if (abs(data->factor_p_s - 114) > 0.001)
            system_.factor_p_s = data->factor_p_s;

        if (abs(data->factor_i_s - 114) > 0.001)
            system_.factor_i_s = data->factor_i_s;

        if (abs(data->factor_d_s - 114) > 0.001)
            system_.factor_d_s = data->factor_d_s;

        if (abs(data->encode_max - 114) > 0.001)
            system_.encode_max = data->encode_max;

        if (abs(data->offset_max - 114) > 0.001)
            system_.offset_max = data->offset_max;

        if (data->model_pid == 1 || data->model_pid == 0)
            system_.model_pid = data->model_pid;
    }

    void Load_Margin(Data_Margin* data)
    {
        margin_ = *data;
    }

    /******************************************
     * @brief Some function about setting value
     */

    // ensure pwm is limited under 500, or your car will fly
    void Set_Motor_Pwm(int pwm)
    {
        Set_Pwm(pwm, pwm);
    }

    // 780 is mid
    void Set_Servo_Duty(uint16_t duty)
    {
        Servo_Control(duty);
    }

    void Set_Speed(float speed)
    {
        if (!system_.model_pid) {

            pwm_speed_ = speed * PWM_limit;
        }
        else {
            speed_current_ = Read_Speed(1);
            bias_ = speed - speed_current_;

            pwm_speed_
                = (float)system_.factor_p_m * bias_
                + (float)system_.factor_i_m * bias_integral_
                + (float)system_.factor_d_m * (bias_ - bias_last_);

            if (abs(bias_ - bias_last_) > 0.01)
                bias_integral_ += bias_;

            bias_last_ = bias_;
        }

        Set_Motor_Pwm(pwm_speed_);
    }

    void Set_Direction_base(float distance)
    {
        // 读数据进行筛选
        {
            if (margin_.margin_a == 0 || margin_.margin_b == 0 || margin_.margin_a == 8888 || margin_.margin_b == 8888)
                return;
        }

        // 保持距离的函数
        int pwm_add_distance = 0;
        int factor_distance = 1;
        int limit_distance = 20;

        bias_distance_ = Get_Distance() - motion_.distance;

        pwm_add_distance = bias_distance_ * factor_distance;

        if (pwm_add_distance > limit_distance)
            pwm_add_distance = limit_distance;

        if (pwm_add_distance < -limit_distance)
            pwm_add_distance = -limit_distance;

        // 保持平衡的函数
        int pwm_add_balance = 0;
        int factor_balance = 3;
        int limit_balance = 30;

        pwm_add_balance
            = (margin_.margin_a - margin_.margin_b) * factor_balance;

        if (pwm_add_balance > limit_balance)
            pwm_add_balance = limit_balance;

        if (pwm_add_balance < -limit_balance)
            pwm_add_balance = -limit_balance;

        pwm_distance_ = 780 + pwm_add_balance + pwm_add_distance;

        Set_Servo_Duty(pwm_distance_);
    }

    float Get_Distance()
    {
        if (margin_.margin_a > 100)
            margin_.margin_a = 80;
        if (margin_.margin_b > 100)
            margin_.margin_b = 80;

        return (9 * (margin_.margin_a + margin_.margin_b))
            / (2 * sqrt(pow(9, 2) + pow(abs(margin_.margin_a - margin_.margin_b), 2)));
    }

    void Set_Direction(int distance)
    {
        // margin_.margin_a;  测量值a
        // margin_.margin_b;  测量值b

        int distance_a = margin_.margin_a;
        int distance_b = margin_.margin_b;

        int limit_pwm_distance = 50;

        if (distance_a > 80)
            distance_a = 80;
        if (distance_b > 80)
            distance_b = 80;

        bias_distance_ = distance_a - distance_b;

        pwm_distance_
            = 780
            + (float)system_.factor_p_s * bias_distance_
            + (float)system_.factor_i_s * bias_distance_ * 0.01
            + (float)system_.factor_d_s * (bias_distance_ - bias_distance_last_);

        pwm_distance_ += (distance - Get_Distance()) / 2;

        if (pwm_distance_ > 780 + limit_pwm_distance)
            pwm_distance_ = 780 + limit_pwm_distance;
        if (pwm_distance_ < 780 - limit_pwm_distance)
            pwm_distance_ = 780 - limit_pwm_distance;

        bias_distance_last_ = bias_distance_;

        Set_Servo_Duty(pwm_distance_);
    }

    float Read_Speed(bool encoder)
    {
        int count = 0;
        char send[10];
        int size = 0;

        if (encoder == 0) {
            Read_Encoder(2);

            osDelay(50);
            count = Read_Encoder(2);

            if (0) {
                size = sprintf(send, "2-%d\n", count);
                HAL_UART_Transmit_DMA(&huart5, (uint8_t*)send, size);
            }
        }
        else {
            Read_Encoder(4);

            osDelay(50);
            count = Read_Encoder(4);

            if (0) {
                size = sprintf(send, "4-%d\n", count);
                HAL_UART_Transmit_DMA(&huart5, (uint8_t*)send, size);
            }
        }

        return -(float)(count) / (-system_.encode_max);
    }

    void Print_Speed(Serial_Transceiver serial)
    {
        char send[20];
        int size = sprintf(send, "speed:%.2f,%.2f\n", speed_current_, motion_.speed);
        serial.Send_Block(send, size);
    }
};