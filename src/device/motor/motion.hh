#pragma once

#include "device/motor/motor.hh"
#include "utility/math.hh"

class Motion {
public:
    Motion(dev::Motor& motor_left_front, dev::Motor& motor_right_front,
        dev::Motor& motor_left_back, dev::Motor& motor_right_back)
        : motor_left_front_(motor_left_front)
        , motor_right_front_(motor_right_front)
        , motor_left_back_(motor_left_back)
        , motor_right_back_(motor_right_back) {
    }

    // r: rad/s
    void move(const util::Vector2f& v, float w) {
        float output0 = v.x - v.y - w * (rx + ry); // left front
        float output1 = v.x + v.y + w * (rx + ry); // right front
        float output2 = v.x + v.y - w * (rx + ry); // left back
        float output3 = v.x - v.y + w * (rx + ry); // right back

        // motor_left_front_.rotate_closed_loop(output0);
        // motor_right_front_.rotate_closed_loop(output1);
        // motor_left_back_.rotate_closed_loop(output2);
        // motor_right_back_.rotate_closed_loop(output3);

        motor_left_front_.rotate_open_loop(output0);
        motor_right_front_.rotate_open_loop(output1);
        motor_left_back_.rotate_open_loop(output2);
        motor_right_back_.rotate_open_loop(output3);
    }

private:
    dev::Motor& motor_left_front_;
    dev::Motor& motor_right_front_;
    dev::Motor& motor_left_back_;
    dev::Motor& motor_right_back_;

    constexpr static inline float rx = 0.525f; // m
    constexpr static inline float ry = 0.950f; // m
};