#include "cmsis_os.h"
#include "queue.h"
#include "../Application/Led_Controller.hh"
#include "../Basic/Message_Type.hh"

extern "C" {
void Breath_LED_Loop();
}

extern QueueHandle_t Queue_Breath;

void Breath_LED_Loop()
{
    auto leds = Led_Group();
    leds.Add(Led_Controller(LED_1_GPIO_Port, LED_1_Pin));
    leds.Add(Led_Controller(LED_2_GPIO_Port, LED_2_Pin));
    leds.Add(Led_Controller(LED_3_GPIO_Port, LED_3_Pin));
    leds.Add(Led_Controller(LED_4_GPIO_Port, LED_4_Pin));

    auto control_led = Control_Led{0, 0, 0, 0};

    for (;;) {

        xQueueReceive(Queue_Breath, &control_led, 0);

        leds.Set(control_led);
    }
}
