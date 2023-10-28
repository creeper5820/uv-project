#pragma once

#include "main.h"
#include "../Basic/Message_Type.hh"

/************************************************************
 * @brief Class to control a led by setting gpio high and low
 */
class Led_Controller
{
private:
    GPIO_TypeDef *type_;
    uint16_t pin_;

public:
    Led_Controller()
    {
    }

    Led_Controller(GPIO_TypeDef *type, uint16_t pin)
        : type_(type),
          pin_(pin)
    {
    }

    void Light()
    {
        HAL_GPIO_WritePin(type_, pin_, GPIO_PIN_RESET);
    }

    void Dark()
    {
        HAL_GPIO_WritePin(type_, pin_, GPIO_PIN_SET);
    }
};

/***********************
 * @brief A group of led
 */
class Led_Group
{
private:
    Led_Controller leds_[4];
    int size_;

public:
    Led_Group()
        : size_(0)
    {
    }

    void Add(Led_Controller led)
    {
        if (size_ > 3)
            return;

        leds_[size_] = led;
        size_++;
    }

    void Light_Single(int number)
    {
        if (number > size_)
            return;

        for (int i = 0; i < size_; i++)
            leds_[i].Dark();

        leds_[number].Light();
    }

    void Set(Control_Led control_led)
    {
        for (int i = 0; i < 4; i++) {

            if (control_led.light[i])
                leds_[i].Light();
            else
                leds_[i].Dark();
        }
    }
};