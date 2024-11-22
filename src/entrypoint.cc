#include "entrypoint.hh"
#include "protocol.hh"
#include "singleton.hh"

#include "device/motor/motion.hh"
#include "hal/base/serial.hh"
#include "hal/base/timer.hh"

#include <cassert>
#include <cstdint>

using namespace hal;
using namespace single;

namespace global {
float rotation { 0 };
float speed[4] { 0, 0, 0, 0 };
util::Vector2f velocity { 0, 0 };

uint8_t receive[20];
pro::Command command;

uint32_t watchdog_tick = 0;
}

GENERATE_UART_RX_EVENT_CALLBACK(remote)
GENERATE_UART_RX_COMPLETE_CALLBACK(remote)
GENERATE_TIM_PERIOD_ELAPSED_CALLBACK(timer17)

namespace mission {
void toggle_led() { led::toggle(); }

void apply_motion() {
    if (global::watchdog_tick > 100) {
        global::velocity = { 0, 0 };
        global::rotation = 0;
    }

    motion.move(global::velocity, global::rotation);
}

void update_status() {
    const int32_t interval[4] { encoder0.interval() * 500 / 13, encoder1.interval() * 500 / 13,
        encoder2.interval(), encoder3.interval() };

    motor0.update_speed(static_cast<float>(interval[0]) / encoder_count_limit);
    motor1.update_speed(static_cast<float>(interval[1]) / encoder_count_limit);
    motor2.update_speed(static_cast<float>(interval[2]) / encoder_count_limit);
    motor3.update_speed(static_cast<float>(interval[3]) / encoder_count_limit);

    global::speed[0] = motor0.speed();
    global::speed[1] = motor1.speed();
    global::speed[2] = motor2.speed();
    global::speed[3] = motor3.speed();
}

void serial_callback(UART_HandleTypeDef*, uint16_t) {
    remote.receive_idle<Mode::It>(global::receive, sizeof(global::receive));

    global::watchdog_tick = 0;
    global::velocity = { 0, 0 };
    global::rotation = 0;

    const auto command = reinterpret_cast<pro::Command*>(global::receive);
    global::command = *command;
    if (command->header == 0xa5 && command->end == 0xb6) {

        global::velocity.x = command->x;
        global::velocity.y = command->y;
        global::rotation = command->w;

        switch (command->event) {
        case pro::Event::None:
            break;
        case pro::Event::Run:
            break;
        case pro::Event::Slow:
            break;
        case pro::Event::Stop:
            global::velocity = { 0, 0 };
            global::rotation = 0;
            break;
        case pro::Event::TurnLeft:
            break;
        case pro::Event::TurnRight:
            break;
        }
    }
}

}

void entrypoint() {
    motor0.setup<pin_a0, pin_a1, pwm_a>();
    motor1.setup<pin_b0, pin_b1, pwm_b>();
    motor2.setup<pin_c0, pin_c1, pwm_c>();
    motor3.setup<pin_d0, pin_d1, pwm_d>();

    // set the direction
    encoder0.setup(false);
    encoder1.setup(true);
    encoder2.setup(true);
    encoder3.setup(true);

    motor0.set_pid(0.7, 0.1, 0);
    motor1.set_pid(0.7, 0.1, 0);
    motor2.set_pid(0.7, 0.1, 0);
    motor3.set_pid(0.7, 0.1, 0);

    auto& receive = global::receive;
    remote.receive_idle<Mode::It>(receive);
    remote.set_callback(mission::serial_callback);

    timer17.register_activity(1000, mission::toggle_led);
    timer17.register_activity(10, mission::update_status);
    timer17.register_activity(10, mission::apply_motion);
    timer17.start();

    while (true) {
        time::delay(1);
        global::watchdog_tick++;
    }
}
