#pragma once

#include <cstdint>

namespace pro {
enum class Event : uint8_t {
    None,
    Run,
    Slow,
    Stop,
    TurnLeft,
    TurnRight
};

struct __attribute__((packed)) Command {
    uint8_t header = 0xa5;
    float x;
    float y;
    float w;
    Event event;
    uint8_t end = 0xb6;
};
}