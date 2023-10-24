#pragma once

#include "dma.h"
#include "usart.h"
#include "string"

enum Status {
    WAIT,
    OK,
};

class Serial_Transceiver
{
private:
    UART_HandleTypeDef *huart_ptr_;
    int status = WAIT;

public:
    Serial_Transceiver(UART_HandleTypeDef *huart_ptr)
        : huart_ptr_(huart_ptr)
    {
    }

    void Send(uint8_t *pdata, uint16_t size)
    {
        HAL_UART_Transmit_DMA(huart_ptr_, pdata, size);
    }

    void Recevice(uint8_t *pdata, uint16_t size)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart_ptr_, pdata, size);
    }

    /**
     * @brief Some function to get
     */

    int Get_Status()
    {
        return status;
    }

    /**
     * @brief Some function to set value of this
     */

    void Set_Wait()
    {
        status = WAIT;
    }

    void Set_Ok()
    {
        status = OK;
    }

    void Hello_World()
    {
        char str[] = "Hello World!";
        HAL_UART_Transmit_DMA(huart_ptr_, (uint8_t *)str, sizeof(str));
    }
};