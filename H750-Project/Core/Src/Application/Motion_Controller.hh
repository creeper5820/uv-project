#pragma once

#include "cmsis_os.h"

#include "../Basic/Motor.h"
#include "../Basic/Servo.h"
#include "../Basic/Encode.h"
#include "../Basic/Message_Type.hh"

class Motion_Controller
{
public:
    Control_System system_;

    // PID params of speed
    float speed_last_ = 0;
    float bias_       = 0;
    float bias_last_  = 0;
    float pwm_        = 0;

    // PID params if direction
    float direction_last_ = 0;

    float speed_current_ = 0;

public:
    Motion_Controller()
        : system_()
    {
    }

    Motion_Controller(Control_System *system)
        : system_(*system)
    {
    }

    void Init()
    {
        Motor_Init();
        Servo_Init();
        Encoder_Init();
    }

    void Load_Target(Control_Motion *motion)
    {
        Set_Direction(motion->direction);
        Set_Speed(motion->speed);
    }

    /******************************************
     * @brief Some function about setting value
     */
    // ensure pwm is limited under 500, or your car will fly
    void Set_Motor_Pwm(int pwm_left, int pwm_right)
    {
        Set_Pwm(pwm_left, pwm_right);
    }

    // 780 is mid
    void Set_Servo_Duty(uint16_t duty)
    {
        Servo_Control(duty);
    }

    void Set_Speed(float speed)
    {
        Pwm_Speed_Pid(speed);
        Set_Motor_Pwm(pwm_, pwm_);
    }

    void Set_Direction(float direction)
    {
        Pwm_Direction_Pid();
        Set_Servo_Duty(780 + -direction * 50);
    }

    /********************
     * @brief PID control
     */
    void Pwm_Speed_Pid(float speed_target)
    {
        if (speed_target == 0) {
            pwm_ = 0;
            return;
        }

        if (system_.model_pid) {

            // float increment_val =
            // pid.Kp*(pid.err - pid.err_next) +
            // pid.Ki*pid.err +
            // pid.Kd*(pid.err - 2 * pid.err_next + pid.err_last);

            bias_ = speed_target - speed_last_;

            int pwm_add =
                system_.factor_p * (bias_ - bias_last_) +
                system_.factor_i * bias_ +
                system_.factor_d * (bias_ - bias_last_);

            pwm_ += pwm_add;

            speed_last_ = Read_Speed();
            bias_last_  = bias_;

        } else {

            pwm_ = speed_target * PWM_limit;
            Read_Speed();
        }
    }

    void Pwm_Direction_Pid()
    {
    }

    float Read_Speed()
    {
        Read_Encoder(3);
        Read_Encoder(4);

        osDelay(50);

        int count_left  = Read_Encoder(3);
        int count_right = Read_Encoder(4);

        speed_current_ = (float)(count_left + count_right) / (float)(-2 * system_.factor_encode);

        return speed_current_;
    }

    float Get_Speed()
    {
        return speed_current_;
    }
};