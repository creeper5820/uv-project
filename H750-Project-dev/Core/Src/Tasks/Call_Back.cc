#include "main.h"
#include "cmsis_os.h"

#include "../Application/Serial_Transceiver.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
}

extern Serial_Transceiver lisii;
extern Serial_Transceiver attitude;
extern Serial_Transceiver tof;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{

    if (huart == lisii.Get_Uart_Type()) {
        lisii.Set_Size(Size);
        lisii.Set_Ok();
        lisii.Recevice_A();
    }

    if (huart == attitude.Get_Uart_Type()) {
        attitude.Set_Size(Size);
        attitude.Set_Ok();
        attitude.Recevice_A();
    }

    if (huart == tof.Get_Uart_Type()) {
        tof.Set_Size(Size);
        tof.Set_Ok();
        tof.Recevice_A();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == lisii.Get_Uart_Type()) {
        lisii.Set_Ok();
        lisii.Recevice_C();
    }

    if (huart == attitude.Get_Uart_Type()) {
        attitude.Set_Ok();
        attitude.Recevice_C();
    }

    if (huart == tof.Get_Uart_Type()) {
        tof.Set_Ok();
        tof.Recevice_C();
    }
}
