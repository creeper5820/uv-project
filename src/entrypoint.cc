#include "entrypoint.hh"
#include "hal/base/timer.hh"
#include "motion.hh"
#include "singleton.hh"

#include <cassert>
#include <cstdint>

using namespace hal;
using namespace single;

float speed[4];

util::Vector2f velocity;
float rotation;

uint8_t receive[20];

void mission0();

void entrypoint() {
    motor0.setup<pin_a0, pin_a1, pwm_a>();
    motor1.setup<pin_b0, pin_b1, pwm_b>();
    motor2.setup<pin_c0, pin_c1, pwm_c>();
    motor3.setup<pin_d0, pin_d1, pwm_d>();

    encoder0.setup(false);
    encoder1.setup(true);
    encoder2.setup(true);
    encoder3.setup(true);

    motor0.set_pid(0.7, 0.1, 0);
    motor1.set_pid(0.7, 0.1, 0);
    motor2.set_pid(0.7, 0.1, 0);
    motor3.set_pid(0.7, 0.1, 0);

    // remote.receive_idle<Serial::Mode::IT>(receive, sizeof(receive));

    // remote.set_callback([](UART_HandleTypeDef* huart, uint16_t size) {
    //     remote.receive_idle<Serial::Mode::IT>(receive, sizeof(receive));
    // });

    timer17.register_activity(1000, []() {
        led::toggle();
    });

    timer17.register_activity(10, []() {
        motion.move(velocity, rotation);
    });

    timer17.register_activity(10, []() {
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

        speed[0] = motor0.speed();
        speed[1] = motor1.speed();
        speed[2] = motor2.speed();
        speed[3] = motor3.speed();
    });

    timer17.start();

    while (true) {
        mission0();
    }
}

void mission0() {
    time::delay(2000);

    velocity = { 0.2, 0 };
    time::delay(2000);

    velocity = { 0.2, 0.2 };
    time::delay(2000);

    velocity = { 0, 0 };
    time::delay(2000);

    velocity = { 0, 0 };
    rotation = 0.1;
    time::delay(1000);

    velocity = { 0.1, -0.2 };
    rotation = 0;
    time::delay(2000);

    velocity = { 0, 0 };
    while (true)
        ;
}