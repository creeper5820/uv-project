#include "entrypoint.hh"
#include "singleton.hh"

#include "hal/base/timer.hh"

#include <cstdint>

using namespace hal;
using namespace single;

volatile float speed[4];
volatile float target[4];

void entrypoint() {
    motor0.setup<pin_a0, pin_a1, pwm_a>();
    encoder0.setup(false);

    motor1.setup<pin_b0, pin_b1, pwm_b>();
    encoder1.setup(true);

    motor2.setup<pin_c0, pin_c1, pwm_c>();
    encoder2.setup(true);

    motor3.setup<pin_d0, pin_d1, pwm_d>();
    encoder3.setup(true);

    timer17.register_activity(1000, []() {
        led::toggle();
    });
    timer17.register_activity(1, []() {
        motor0.rotate_open_loop(target[0]);
        motor1.rotate_open_loop(target[1]);
        motor2.rotate_open_loop(target[2]);
        motor3.rotate_open_loop(target[3]);
    });
    timer17.register_activity(1, []() {
        const int32_t interval[4] {
            encoder0.interval() * 500 / 13, encoder1.interval() * 500 / 13,
            encoder2.interval(), encoder3.interval()
        };
        motor0.update_speed(
            static_cast<float>(interval[0]) / encoder_count_limit);
        motor1.update_speed(
            static_cast<float>(interval[1]) / encoder_count_limit);
        motor2.update_speed(
            static_cast<float>(interval[2]) / encoder_count_limit);
        motor3.update_speed(
            static_cast<float>(interval[3]) / encoder_count_limit);
    });
    timer17.start();

    while (true) {
        time::delay(100);
    }
}
