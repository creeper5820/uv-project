#pragma once

#include "cmsis_os.h"
#include "stdio.h"

#include "../Basic/Motor.h"
#include "../Basic/Servo.h"
#include "../Basic/Encode.h"
#include "../Basic/Message_Type.hh"

class Motion_Controller {
public:
    Data_System system_;
    Data_Motion motion_;

    int offset_;

    // PID params of speed
    float bias_ = 0;
    float bias_last_ = 0;

    // PID params if direction
    float speed_current_ = 0;
    float direction_current_ = 0;

    int pwm_speed_ = 0;
    int pwm_direction_ = 0;

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
        Set_Direction(motion_.direction);
        Set_Speed(motion_.speed);
    }

    void Load_Motion(Data_Motion* data)
    {
        if (abs(data->speed - 114) > 0.001)
            motion_.speed = data->speed;

        if (abs(data->direction - 114) > 0.001)
            motion_.direction = data->direction;
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

    void Load_Offset(int offset)
    {
        offset_ = offset;
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
        Set_Motor_Pwm(Pwm_Speed_Pid(speed));
    }

    void Set_Direction(float direction)
    {
        if (direction == 0) {
            direction += (float)offset_ / system_.offset_max;
        }

        if (direction > 1.0)
            direction = 1;

        if (direction < -1.0)
            direction = -1;

        int pwm_direction = 780 - direction * 50;

        Set_Servo_Duty(pwm_direction);
    }

    /********************
     * @brief PID control
     */
    float Pwm_Speed_Pid(float speed_target)
    {
        if (system_.model_pid) {

            bias_ = speed_target - Read_Speed(1);

            float pwm_add
                = system_.factor_p_m * bias_
                + system_.factor_i_m * bias_ * 0.01
                + system_.factor_d_m * (bias_ - bias_last_);

            pwm_speed_ += pwm_add;

            bias_last_ = bias_;
        }
        else {
            pwm_speed_ = speed_target * PWM_limit;
        }

        return pwm_speed_;
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

        return (float)(count) / (-system_.encode_max);
    }

    void Print_Speed(Serial_Transceiver serial)
    {
        char send[20];
        int size = sprintf(send, "speed:%.2f,%.2f\n", speed_current_, motion_.speed);
        serial.Send_Block(send, size);
    }
};