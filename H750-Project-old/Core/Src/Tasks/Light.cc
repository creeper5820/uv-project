#include "Light.hh"

static Data_Light data_light;

void Light_Loop()
{
    leds.Add(Led_Controller(LED_1_GPIO_Port, LED_1_Pin));
    leds.Add(Led_Controller(LED_2_GPIO_Port, LED_2_Pin));
    leds.Add(Led_Controller(LED_3_GPIO_Port, LED_3_Pin));
    leds.Add(Led_Controller(LED_4_GPIO_Port, LED_4_Pin));

    light.Add(Led_Controller(Light_1_GPIO_Port, Light_1_Pin));
    light.Add(Led_Controller(Light_2_GPIO_Port, Light_2_Pin));
    light.Add(Led_Controller(Light_3_GPIO_Port, Light_3_Pin));
    light.Add(Led_Controller(Light_4_GPIO_Port, Light_4_Pin));

    if (1)
        for (;;) {
            xQueueReceive(Queue_Light, &data_light, portMAX_DELAY);

            switch (data_light.status) {

            case LIGHT_ALL:
                Light_All();
                light.Set({ 1, 1, 1, 1 });
                break;

            case DARK_ALL:
                Dark_All();
                light.Set({ 0, 0, 0, 0 });
                break;

            case LIGHT_LEFT:
                Light_Left();
                light.Set({ 0, 0, 0, 1 });
                break;

            case LIGHT_RIGHT:
                Light_Right();
                light.Set({ 0, 0, 1, 0 });
                break;

            case LIGHT_HEAD:
                Light_Head();
                light.Set({ 0, 1, 0, 0 });
                break;

            case LIGHT_TAIL:
                Light_Tail();
                light.Set({ 1, 0, 0, 0 });
                break;

            case FLASH_ALL:
                Flash_All();
                break;

            case FLASH_THREE:
                Light_All();
                osDelay(200);
                Dark_All();
                osDelay(200);
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
    if (0)
        for (;;) {
            light.Set({ 1, 1, 1, 1 });
            osDelay(2000);
            light.Set({ 0, 0, 0, 0 });
            osDelay(2000);
            light.Set({ 1, 0, 0, 0 });
            osDelay(2000);
            light.Set({ 0, 1, 0, 0 });
            osDelay(2000);
            light.Set({ 0, 0, 1, 0 });
            osDelay(2000);
            light.Set({ 0, 0, 0, 1 });
            osDelay(2000);
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
        light.Light_Single(i);
        osDelay(100);
    }

    for (int i = 0; i < 4; i++) {
        leds.Light_Single(3 - i);
        light.Light_Single(3 - i);
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