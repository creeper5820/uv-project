#include "singleton.hh"

using namespace hal;
using namespace single;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    timer17.callback(htim);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    (void)huart;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size) {
    remote.callback(huart, size);
}