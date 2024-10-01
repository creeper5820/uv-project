#pragma once

namespace util {

template <typename T>
constexpr T limit(T value, T min, T max) {
    value = value > max ? max : value;
    value = value < min ? min : value;
    return value;
}

}