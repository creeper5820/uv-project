#pragma once

#include "main.h"
#include <cstddef>
#include <cstdint>

namespace sys {
// @brief delay the number of milliseconds
void delay(uint32_t ms);

// @brief to reset the screen
void reset();

// @brief write the command byte to screen
void command(uint8_t byte);

// @brief write the data bytes to screen
void data(uint8_t* buffer, size_t size);
}