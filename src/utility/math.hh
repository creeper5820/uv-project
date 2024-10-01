#pragma once

namespace util {

struct Vector2f {
    float x;
    float y;
    Vector2f() = default;
    Vector2f(float x, float y)
        : x(x)
        , y(y) { }
    Vector2f(Vector2f& other) {
        *this = other;
    }
    Vector2f& operator=(const Vector2f& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    Vector2f operator+(const Vector2f& other) const {
        return { x + other.x, y + other.y };
    }
    Vector2f operator-(const Vector2f& other) const {
        return { x - other.x, y - other.y };
    }
    void operator+=(const Vector2f& other) {
        x += other.x;
        y += other.y;
    }
    void operator-=(const Vector2f& other) {
        x -= other.x;
        y -= other.y;
    }
};

template <typename T>
constexpr T limit(T value, T min, T max) {
    value = (value > max) ? max : value;
    value = (value < min) ? min : value;
    return value;
}

}