#pragma once

#include "hal/base/gpio.hh"
#include "hal/base/pwm.hh"

#include "utility/math.hh"
#include "utility/pid.hh"

namespace dev {

template <typename T>
concept gpio_concept = hal::gpio::gpio_concept<T>;
template <typename T>
concept pwm_concept = hal::pwm::pwm_concept<T>;

class Motor {
public:
    explicit Motor() { }

    template <gpio_concept _gpioa, gpio_concept _gpiob,
        pwm_concept _pwm>
    void setup() {
        using namespace hal;
        rotate_ = [](float speed) -> void {
            auto speed_ = util::limit(speed, -1.0f, +1.0f);
            if (speed_ > 0) {
                _gpioa::set();
                _gpiob::reset();
                _pwm::set_ratio(speed_);
            } else {
                _gpioa::reset();
                _gpiob::set();
                _pwm::set_ratio(-speed_);
            }
        };
        _pwm::template start<pwm::normal>();
        _gpioa::reset();
        _gpiob::reset();
    }

    void reset_pid() { pid_.reset(); }

    void set_pid(float kp, float ki, float kd) {
        pid_.kp = kp;
        pid_.ki = ki;
        pid_.kd = kd;
    }

    void update_speed(float speed) { speed_scan_ = speed; }

    float speed() const { return speed_scan_; }

    void rotate_open_loop(float speed) {
        rotate_(speed);
    }

    void rotate_closed_loop(float speed) {
        rotate_(pid_.update(speed_scan_, speed));
    }

private:
    void (*rotate_)(float);

    util::PID<float, float> pid_;

    float speed_scan_ = 0;
};

}