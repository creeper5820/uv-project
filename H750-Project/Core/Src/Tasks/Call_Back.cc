#include "main.h"
#include "cmsis_os.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
    extern Serial_Transceiver lisii;

    if (huart == lisii.Get_Uart_Type()) {

        lisii.Set_Size(Size);
        lisii.Set_Ok();

        lisii.Recevice_A();
    }
}
