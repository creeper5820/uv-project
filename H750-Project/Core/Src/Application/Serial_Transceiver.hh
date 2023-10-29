#pragma once

#include "dma.h"
#include "usart.h"
#include "cmsis_os.h"

enum Status {
    WAIT,
    OK,
};

class Serial_Transceiver
{
private:
    UART_HandleTypeDef *huart_ptr_;
    char data_[30];
    int size_;
    int status_ = WAIT;

public:
    Serial_Transceiver(UART_HandleTypeDef *huart_ptr)
        : huart_ptr_(huart_ptr)
    {
    }

    void Send(char *pdata, uint16_t size)
    {
        HAL_UART_Transmit_DMA(huart_ptr_, (uint8_t *)pdata, size);
        osDelay(5);
    }

    void Recevice_A()
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart_ptr_, (uint8_t *)data_, sizeof(data_));
    }

    void Recevice_B()
    {
        HAL_UART_Receive_DMA(huart_ptr_, (uint8_t *)data_, sizeof(data_));
    }

    /**
     * @brief Some function to get
     */

    int Get_Status()
    {
        return status_;
    }

    UART_HandleTypeDef *Get_Uart_Type()
    {
        return huart_ptr_;
    }

    char *Get_Data()
    {
        return data_;
    }

    int Get_Data_Size()
    {
        return size_;
    }

    char Get_Data_Type()
    {
        return data_[0];
    }

    /**
     * @brief Some function to set value of this
     */

    void Set_Wait()
    {
        status_ = WAIT;
    }

    void Set_Ok()
    {
        status_ = OK;
    }

    void Set_Size(int size)
    {
        size_ = size;
    }

    void Hello_World()
    {
        char str[] = "Hello World!";
        HAL_UART_Transmit_DMA(huart_ptr_, (uint8_t *)str, sizeof(str));
    }
};
