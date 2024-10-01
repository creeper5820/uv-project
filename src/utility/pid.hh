#pragma once

namespace util {

template <typename _input, typename _output>
struct PID {
    _output update(_input input, _input target) {
        _input error_current = target - input;
        _input error_diff = error_current - error_last;

        _output output
            = kp * error_current
            + ki * error_cumulative
            + kd * error_diff;

        error_cumulative += error_current;
        error_last = error_current;

        return output;
    }
    void reset() {
        error_cumulative = 0;
        error_last = 0;
    }
    float kp;
    float ki;
    float kd;
    _input error_cumulative;
    _input error_last;
};
}