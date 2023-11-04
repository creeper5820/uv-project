#pragma once

#include "cmsis_os.h"
#include "stdio.h"
#include "math.h"

#include "../Basic/Motor.h"
#include "../Basic/Servo.h"
#include "../Basic/Message_Type.hh"

#define SPEED_RECORD_NUM 10
#define DISTANCE_RECORD_NUM 5

class Motion_Controller {
public:
    Data_Motion motion_;
    Data_Attitude attitude_;

    // 速度环的过程量
    float bias_left_ = 0;
    float bias_right_ = 0;

    float bias_left_last_ = 0;
    float bias_right_last_ = 0;

    float bias_left_integral_ = 0;
    float bias_right_integral_ = 0;

    float speed_left_now_ = 0;
    float speed_right_now_ = 0;

    // 距离环的过程量
    float bias_distance_ = 0;
    float bias_distance_last = 0;
    float bias_distance_integral_ = 0;

    int flag_pid_ = 1;

    // 滤波用数据组
    float speeds_left_[SPEED_RECORD_NUM];
    float speeds_right_[SPEED_RECORD_NUM];

    float distances_[DISTANCE_RECORD_NUM];

    // 右偏 <-(值越小)- PWM -(值越大)-> 左偏
    int pwm_mid_ = 775;

public:
    Motion_Controller()
    {
        motion_ = { 0, 0 };
    }

    // 电机和舵机的初始化
    void Init()
    {
        Motor_Init();
        Servo_Init();
    }

    /********************************
     * @brief 一些数据的载入和加载函数
    */

    // 加载运动目标
    void Load_Motion(Data_Motion* data)
    {
        motion_ = *data;
    }

    // 加载姿态信息
    void Load_Attitude(Data_Attitude* data)
    {
        if (data->margin_a != 0 && data->margin_a != 8888)
            attitude_.margin_a = data->margin_a;

        if (data->margin_b != 0 && data->margin_b != 8888)
            attitude_.margin_b = data->margin_b;

        attitude_.speed_left = data->speed_left;

        attitude_.speed_right = data->speed_right;
    }

    // 进行运动控制
    void Load()
    {
        Set_Direction(motion_.distance);
        Set_Speed(motion_.speed);
    }

    /**
     * @brief 进行速度的平均滤波
     * @param speed_new 输入新采样到的速度
     * @param speeds 存放速度的数组
     * @return 返回滤波后的速度, 注意修改返回值的除数来归一速度
    */
    float Filter_Speed(float speed_new, float* speeds)
    {
        float sum = 0.0f;

        //将现有数据后移一位
        for (uint8_t i = SPEED_RECORD_NUM - 1; i > 0; i--) {
            speeds[i] = speeds[i - 1];
            sum += speeds[i - 1];
        }

        //第一位是新的数据
        if (speed_new > 10000)
            speed_new = speed_new - 20000;

        speeds[0] = speed_new;

        sum += speed_new;

        //返回均值
        return sum / (SPEED_RECORD_NUM * 2430);
    }

    float Filter_Distance(float distance_new, float* distances)
    {
        float sum = 0.0f;

        //将现有数据后移一位
        for (uint8_t i = DISTANCE_RECORD_NUM - 1; i > 0; i--) {
            distances[i] = distances[i - 1];
            sum += distances[i - 1];
        }

        distances[0] = distance_new;

        sum += distance_new;

        //返回均值
        return sum / DISTANCE_RECORD_NUM;
    }

    /*************************
     * 速度环和距离环的闭环控制
    */

    /**
     * @brief 死调方向的函数
     * @param direction 方向,0 往左, 1往右
     * @param pwm_anlge 拐弯的角度pwm值,最大值为50, 最小值为0
     * @param time_ms 拐弯的毫秒数
    */
    void Turn(bool direction, int pwm_anlge, int time_ms)
    {
        if (direction == 0) {
            flag_pid_ = 0;
            Servo_Control(pwm_mid_ + pwm_anlge);
            osDelay(time_ms);
            flag_pid_ = 1;
        }
        else {
            flag_pid_ = 0;
            Servo_Control(pwm_mid_ - pwm_anlge);
            osDelay(time_ms);
            flag_pid_ = 1;
        }
    }

    // 设置舵机角度
    void Set_Servo_Duty(uint16_t duty)
    {
        if (flag_pid_ == 1)
            Servo_Control(duty);
        else
            return;
    }

    // 电机闭环控制
    void Set_Speed(float speed)
    {
        int pwm_speed_left = 0;
        int pwm_speed_right = 0;

        // 修改if的判断表达式就可以修改开环和闭环
        // 姿态任务中的速度测量值可能会有一部分为0,这个时候需要去除,这是由于串口收发不同步导致的很多噪声数据
        if (0) {
            pwm_speed_left = speed * PWM_limit;
            pwm_speed_right = speed * PWM_limit;
        }
        else {
            // 调用滤波函数获取速度值,滤波算法的实现在Get_Speed()里面
            speed_left_now_ = -Filter_Speed(attitude_.speed_left, speeds_left_);
            speed_right_now_ = -Filter_Speed(attitude_.speed_right, speeds_right_);

            // 获取目标速度和实际速度的差值
            bias_left_ = speed - speed_left_now_;
            bias_right_ = speed - speed_right_now_;

            // 计算输出的pwm量
            pwm_speed_left
                = speed * PWM_limit
                + (float)100 * bias_left_
                + (float)0 * bias_left_integral_
                + (float)30 * (bias_left_ - bias_left_last_);
            pwm_speed_right
                = speed * PWM_limit
                + (float)10 * bias_right_
                + (float)0 * bias_right_integral_
                + (float)15 * (bias_right_ - bias_right_last_);

            // 防止电机堵住积分无限积累
            if (abs(bias_left_ - bias_left_last_) > 0.01)
                bias_left_integral_ += bias_left_;
            if (abs(bias_right_ - bias_right_last_) > 0.01)
                bias_right_integral_ += bias_right_;

            // 差值记录
            bias_left_last_ = bias_left_;
            bias_right_last_ = bias_right_;
        }

        Set_Pwm(pwm_speed_left, pwm_speed_right);

        // Set_Pwm(150, 150);
    }

    // 舵机闭环控制
    void Set_Direction(int distance)
    {
        if (attitude_.margin_a == 0 || attitude_.margin_b == 0)
            return;

        int pwm_output = 0;

        // 获取实际的测量值
        // float distance_true = Filter_Distance(Get_Distance(), distances_);
        float distance_true = Get_Distance();

        // 对测量值进行限幅
        if (distance_true > 80)
            distance_true = 80;

        // 计算距离偏差值
        bias_distance_ = distance_true - distance;

        // 设置pwm输出限幅
        int limit_pwm_distance = 50;

        // 计算pwm输出量
        float pwm_distance_add
            = (float)0.25 * bias_distance_
            + (float)0 * bias_distance_integral_
            + (float)2 * (bias_distance_ - bias_distance_last);

        // 对pwm输出量进行限幅
        if (pwm_distance_add > limit_pwm_distance)
            pwm_distance_add = limit_pwm_distance;
        if (pwm_distance_add < -limit_pwm_distance)
            pwm_distance_add = -limit_pwm_distance;

        // 输出pwm结果
        pwm_output = pwm_mid_ + pwm_distance_add;

        Set_Servo_Duty(pwm_output);

        bias_distance_last = bias_distance_;
    }

    float Get_Distance()
    {
        if (attitude_.margin_a > 80)
            attitude_.margin_a = 80;
        if (attitude_.margin_b > 80)
            attitude_.margin_b = 80;

        return (9 * (attitude_.margin_a + attitude_.margin_b))
            / (2 * sqrt(pow(9, 2) + pow(abs(attitude_.margin_a - attitude_.margin_b), 2)));
    }
};