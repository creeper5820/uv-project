#include "system.hh"

#ifdef HAL_SPI_MODULE_ENABLED

#include "spi.h"

#include "hal/base/gpio.hh"
#include "hal/base/timer.hh"

static auto pin_reset = hal::gpio::PB<14> {};
static auto pin_dc = hal::gpio::PB<12> {};

void sys::delay(uint32_t ms) {
    hal::time::delay(ms);
}

void sys::reset() {
    pin_reset.reset();
    delay(10);
    pin_reset.set();
    delay(10);
}

void sys::command(uint8_t byte) {
    pin_dc.reset();
    HAL_SPI_Transmit(&hspi1, (uint8_t*)&byte, 1, UINT32_MAX);
}

void sys::data(uint8_t* buffer, size_t size) {
    pin_dc.set();
    HAL_SPI_Transmit(&hspi1, buffer, size, UINT32_MAX);
}

#else

void sys::delay(uint32_t ms) { }
void sys::reset() { }
void sys::command(uint8_t byte) { }
void sys::data(uint8_t* buffer, size_t size) { }

#endif
