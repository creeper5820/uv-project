#include "cmsis_os.h"
#include "queue.h"
#include "../Application/Led_Controller.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Light_Loop();
}

extern QueueHandle_t Queue_Light;

auto leds          = Led_Group();
auto control_light = Control_Light{};

void Wait_Start();
void Wait_Tof();

void Light_Loop()
{
    leds.Add(Led_Controller(LED_1_GPIO_Port, LED_1_Pin));
    leds.Add(Led_Controller(LED_2_GPIO_Port, LED_2_Pin));
    leds.Add(Led_Controller(LED_3_GPIO_Port, LED_3_Pin));
    leds.Add(Led_Controller(LED_4_GPIO_Port, LED_4_Pin));

    for (;;) {
        xQueueReceive(Queue_Light, &control_light, 0);

        switch (control_light.status) {
            case LIGHT_ALL:
                break;

            case DARK_ALL:
                break;

            case LIGHT_LEFT:
                break;

            case LIGHT_RIGHT:
                break;

            case LIGHT_HEAD:
                break;

            case LIGHT_TAIL:
                break;

            case FLASH_ALL:
                break;

            default:
                break;
        }
    }
}

void Wait_Start()
{
    for (int i = 0; i < 4; i++) {
        leds.Light_Single(i);
        osDelay(300);
    }

    for (int i = 3; i > -1; i--) {
        leds.Light_Single(i);
        osDelay(50);
    }
}

void Wait_Tof()
{
    leds.Set({1, 1, 1, 1});
    osDelay(300);
    leds.Set({0, 0, 0, 0});
    osDelay(300);
}
