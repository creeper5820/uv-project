#include "Messager.hh"

/******************************************
 * @brief Loop function of the task
 */
void Messager_Loop()
{
    Begin();

    for (;;) {

        switch (model) {
            case MODEL_DEBUG:
                Model_Debug();
                break;

            case TASK_A:
                Task_A();
                break;

            case TASK_B:
                Task_B();
                break;

            case TASK_C:
                Task_C();
                break;
        }
    }

    while (1)
        ;
}

void Begin()
{
    Control_Light control_light = {FLASH_ALL};
    xQueueSend(Queue_Light, &control_light, 50);

    while (lisii.Get_Status() == OK)
        ;
}

/******************************************
 * @brief Function of the task a
 */
void Task_A()
{
}

/******************************************
 * @brief Function of the task b
 */
void Task_B()
{
}

/******************************************
 * @brief Function of the task b
 */
void Task_C()
{
}

/******************************************
 * @brief Function of the debug
 */
void Model_Debug()
{
    lisii.Recevice_A((uint8_t *)data, sizeof(data));

    if (lisii.Get_Status() == OK) {
        lisii.Set_Ok();
        lisii.Send((char *)"OK\n", 3);
        osDelay(50);
    }

    osDelay(50);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == lisii.Get_Uart_Type()) {
        lisii.Recevice_A((uint8_t *)data, Size);
        lisii.Send((char *)"Lisii OK\n", 9);
    }
}
