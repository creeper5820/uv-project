#include "singleton.hh"

using namespace hal;
using namespace single;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    timer17.callback(htim);
}