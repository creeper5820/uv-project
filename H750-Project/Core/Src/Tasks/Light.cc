#include "Light.hh"

static Data_Light data_light;

void Light_Loop()
{
    leds.Add(Led_Controller(LED_1_GPIO_Port, LED_1_Pin));
    leds.Add(Led_Controller(LED_2_GPIO_Port, LED_2_Pin));
    leds.Add(Led_Controller(LED_3_GPIO_Port, LED_3_Pin));
    leds.Add(Led_Controller(LED_4_GPIO_Port, LED_4_Pin));

    for (;;) {
        xQueueReceive(Queue_Light, &data_light, 0);

        switch (data_light.status) {

        case LIGHT_ALL:
            Light_All();
            break;

        case DARK_ALL:
            Dark_All();
            break;

        case LIGHT_LEFT:
            Light_Left();
            break;

        case LIGHT_RIGHT:
            Light_Right();
            break;

        case LIGHT_HEAD:
            Light_Head();
            break;

        case LIGHT_TAIL:
            Light_Tail();
            break;

        case FLASH_ALL:
            Flash_All();
            break;

        case MODEL_DEBUG:
            leds.Light_Single(0);
            break;
        case TASK_A:
            leds.Light_Single(1);
            break;
        case TASK_B:
            leds.Light_Single(2);
            break;
        case TASK_C:
            leds.Light_Single(3);
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
    leds.Set({ 1, 1, 1, 1 });
    osDelay(300);
    leds.Set({ 0, 0, 0, 0 });
    osDelay(300);
}

void Light_All()
{
    leds.Set({ 1, 1, 1, 1 });
}

void Dark_All()
{
    leds.Set({ 0, 0, 0, 0 });
}

void Flash_All()
{
    for (int i = 0; i < 4; i++) {
        leds.Light_Single(i);
        osDelay(100);
    }

    for (int i = 0; i < 4; i++) {
        leds.Light_Single(3 - i);
        osDelay(100);
    }
}

void Light_Left()
{
}

void Light_Right()
{
}

void Light_Head()
{
}

void Light_Tail()
{
}