#include "Messager.hh"

/******************************************
 * @brief Loop function of the task
 */
void Messager_Loop()
{

    for (;;) {

        Begin();

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
}

void Begin()
{
    // Flash all
    control_light_temp.status = FLASH_ALL;
    control_light_temp.task   = PREPARE;
    xQueueSend(Queue_Light, &control_light_temp, 100);

    // Wait OrangePi to send message
    lisii.Send((char *)"Wait OrangePi to send message\n",
               sizeof("Wait OrangePi to send message"));

    if (0)
        while (xQueueReceive(Queue_Opencv, &data_opencv_temp, 0) != pdPASS) {
            // osDelay avoid block other threads
            osDelay(50);
        }

    lisii.Send((char *)"Done!\n",
               sizeof("Done!"));

    // Light all
    control_light_temp.status = LIGHT_ALL;
    xQueueSend(Queue_Light, &control_light_temp, 100);

    // Wait tof to be covered by hand
    lisii.Send((char *)"Wait tof to be covered by hand\n",
               sizeof("Wait tof to be covered by hand"));

    if (1)
        do {
            xQueueReceive(Queue_Tof, &data_tof_temp, 10);
            osDelay(50);
        } while (data_tof_temp.distance > 15);

    lisii.Send((char *)"Done!\n",
               sizeof("Done!"));

    // Flash all
    control_light_temp.status = FLASH_ALL;
    xQueueSend(Queue_Light, &control_light_temp, 100);

    // Wait hand removed
    lisii.Send((char *)"Wait hand removed\n",
               sizeof("Wait hand removed"));
    if (1)
        do {
            xQueueReceive(Queue_Tof, &data_tof_temp, 10);
            osDelay(50);
        } while (data_tof_temp.distance < 15);

    // Dark all
    control_light_temp.status = DARK_ALL;
    xQueueSend(Queue_Light, &control_light_temp, 100);

    lisii.Send((char *)"Done!\n",
               sizeof("Done!"));

    osDelay(50);
}

/******************************************
 * @brief Function of the task a
 */
void Task_A()
{
    control_light_temp.task = TASK_A;
    xQueueSend(Queue_Light, &control_light_temp, 100);

    control_motion_temp = {0.4, 0};
    xQueueSend(Queue_Motion, &control_motion_temp, 100);

    if (0) {
        while (1)
            osDelay(50);
    }

    osDelay(5000);

    control_motion_temp = {0, 0};
    xQueueSend(Queue_Motion, &control_motion_temp, 50);

    osDelay(1000);
}

/******************************************
 * @brief Function of the task b
 */
void Task_B()
{
    control_light_temp.task = TASK_B;
    xQueueSend(Queue_Light, &control_light_temp, 100);
}

/******************************************
 * @brief Function of the task b
 */
void Task_C()
{
    control_light_temp.task = TASK_C;
    xQueueSend(Queue_Light, &control_light_temp, 100);
}

/******************************************
 * @brief Function of the debug
 */
void Model_Debug()
{
    control_light_temp.task = MODEL_DEBUG;
    xQueueSend(Queue_Light, &control_light_temp, 100);

    int flag[] = {1, 1};

    for (;;) {

        if (xQueueReceive(Queue_Opencv, &data_opencv_temp, 0) == pdPASS) {
            if (flag[0]) Show_Data_OpenCV(data_opencv_temp, lisii);
        }

        if (xQueueReceive(Queue_Tof, &data_tof_temp, 0) == pdPASS) {
            if (flag[1]) Show_Data_Tof(data_tof_temp, lisii);
        }

        osDelay(100);
    }
}
