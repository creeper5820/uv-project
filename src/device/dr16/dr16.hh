#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace dev {

struct DR16 {
    enum class Switch : uint8_t {
        UNKNOWN = 0,
        UP = 1,
        DOWN = 2,
        MIDDLE = 3
    };

    struct __attribute__((packed)) Keyboard {
        constexpr static inline Keyboard zero() {
            constexpr uint16_t zero = 0;
            return static_cast<Keyboard>(zero);
        }
        bool w : 1;
        bool s : 1;
        bool a : 1;
        bool d : 1;
        bool shift : 1;
        bool ctrl : 1;
        bool q : 1;
        bool e : 1;
        bool r : 1;
        bool f : 1;
        bool g : 1;
        bool z : 1;
        bool x : 1;
        bool c : 1;
        bool v : 1;
        bool b : 1;
    };

    struct __attribute__((packed, aligned(8))) Dr16DataPart1 {
        uint16_t joystick_channel0 : 11;
        uint16_t joystick_channel1 : 11;
        uint16_t joystick_channel2 : 11;
        uint16_t joystick_channel3 : 11;

        Switch switch_right : 2;
        Switch switch_left : 2;
    };

    struct __attribute__((packed, aligned(8))) Dr16DataPart2 {
        int16_t mouse_velocity_x;
        int16_t mouse_velocity_y;
        int16_t mouse_velocity_z;

        bool mouse_left;
        bool mouse_right;
    };

    struct __attribute__((packed, aligned(4))) Dr16DataPart3 {
        Keyboard keyboard;
        uint16_t unused;
    };

    void update() {

        auto channel_to_double = [](uint16_t value) {
            return (static_cast<int32_t>(value) - 1024) / 660.0;
        };

        auto part1 = data_part1_;
        joystick_right_y_ = -channel_to_double(part1.joystick_channel0);
        joystick_right_x_ = channel_to_double(part1.joystick_channel1);
        joystick_left_y_ = -channel_to_double(part1.joystick_channel2);
        joystick_left_x_ = channel_to_double(part1.joystick_channel3);
        switch_right_ = part1.switch_right;
        switch_left_ = part1.switch_left;

        auto part2 = data_part2_;
        mouse_velocity_x_ = -part2.mouse_velocity_y / 32768.0;
        mouse_velocity_y_ = -part2.mouse_velocity_x / 32768.0;
        mouse_left_ = part2.mouse_left;
        mouse_right_ = part2.mouse_right;

        auto part3 = data_part3_;
        keyboard_ = part3.keyboard;
    }

    void store_status(const uint8_t* data, size_t length) {
        if (length != 6 + 8 + 4)
            return;

        Dr16DataPart1 part1;
        std::memcpy(&part1, data, 6);
        data += 6;
        data_part1_ = part1;

        Dr16DataPart2 part2;
        std::memcpy(&part2, data, 8);
        data += 8;
        data_part2_ = part2;

        Dr16DataPart3 part3;
        std::memcpy(&part3, data, 4);
        data += 4;
        data_part3_ = part3;
    }

    Dr16DataPart1 data_part1_;
    Dr16DataPart2 data_part2_;
    Dr16DataPart3 data_part3_;

    double joystick_right_y_;
    double joystick_right_x_;
    double joystick_left_y_;
    double joystick_left_x_;

    Switch switch_right_;
    Switch switch_left_;

    double mouse_velocity_x_;
    double mouse_velocity_y_;
    double mouse_left_;
    double mouse_right_;

    Keyboard keyboard_;
};

}