#include "Messager.hh"

// Message caches
static Data_OpenCV data_opencv_temp;
static Data_Motion data_motion_temp;
static Data_Tof data_tof_temp;
static Data_Light data_light_temp;

extern Serial_Transceiver lisii;

// Queue to use
auto Queue_Motion = xQueueCreate(1, sizeof(Data_Motion));
auto Queue_System = xQueueCreate(1, sizeof(Data_System));
auto Queue_Opencv = xQueueCreate(1, sizeof(Data_OpenCV));
auto Queue_Margin = xQueueCreate(1, sizeof(Data_Margin));
auto Queue_Light = xQueueCreate(1, sizeof(Data_Light));
auto Queue_Tof = xQueueCreate(1, sizeof(Data_Tof));

/******************************************
 * @brief Loop function of the task
 */
void Messager_Loop()
{

    for (;;) {

        if (flag_begin)
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
    Light_Mode(model);

    // 按键消抖
    osDelay(200);

    // 使用按键调节模式
    while (Button_Scan(Button_1_GPIO_Port, Button_1_Pin)) {

        if (!Button_Scan(Button_5_GPIO_Port, Button_5_Pin) && model > 0) {
            osDelay(20);
            if (!Button_Scan(Button_5_GPIO_Port, Button_5_Pin)) {
                model--;
                Light_Mode(model);
            }
        }

        if (!Button_Scan(Button_3_GPIO_Port, Button_3_Pin) && model < 3) {
            osDelay(20);
            if (!Button_Scan(Button_3_GPIO_Port, Button_3_Pin)) {
                model++;
                Light_Mode(model);
            }
        }

        osDelay(100);
    }

    Light_Mode(FLASH_ALL);
    if (flag_debug)
        lisii.Send((char*)"Wait OrangePi to send message\n", sizeof("Wait OrangePi to send message"));

    // 等待香橙派发送信息
    if (flag_wait_pi)
        while (xQueueReceive(Queue_Opencv, &data_opencv_temp, 100) != pdTRUE) {
            // 避免阻塞优先级低的线程，有必要加一个延时
            osDelay(50);
        }

    if (flag_debug)
        lisii.Send((char*)"Done!\n", sizeof("Done!"));

    Light_Mode(LIGHT_ALL);

    if (flag_debug)
        lisii.Send((char*)"Wait tof to be covered by hand\n", sizeof("Wait tof to be covered by hand"));

    // 清空消息队列
    xQueueReset(Queue_Tof);
    data_tof_temp.distance = 8888;

    // TOF有遮挡时下一步
    Scan_Block_Next(10);

    if (flag_debug)
        lisii.Send((char*)"Done!\n", sizeof("Done!"));

    // 灯光全部亮起
    data_light_temp.status = FLASH_ALL;
    xQueueOverwrite(Queue_Light, &data_light_temp);

    if (flag_debug)
        lisii.Send((char*)"Wait hand removed\n", sizeof("Wait hand removed"));

    // 清空消息队列
    xQueueReset(Queue_Tof);

    // TOF没有遮挡时下一步
    Scan_Block_Stop(20);

    // 灯光全部暗下
    Light_Mode(DARK_ALL);

    if (flag_debug)
        lisii.Send((char*)"Done!\n", sizeof("Done!"));

    data_motion_temp = { 0, distance_right };
    xQueueOverwrite(Queue_Motion, &data_motion_temp);

    osDelay(1000);
}

/******************************************
 * @brief Function of the task a
 */
void Task_A()
{
    float speed_task_a = 0.20;

    Set_Speed(0, distance_right);

    Light_Mode(TASK_A);

    Set_Speed(speed_task_a, distance_right);

    // 无限阻塞以调试
    if (0)
        while (1)
            osDelay(50);

    // 防止抖动
    osDelay(1000);

    // 检测到前方的墙后进行下一步
    Scan_Block_Next(150);

    if (flag_debug)
        lisii.Send((char*)"stop\n", sizeof("stop"));

    // 刹车
    Light_Mode(LIGHT_TAIL);

    Set_Speed(-0.1, distance_right);

    osDelay(500);

    Set_Speed(0, distance_right);

    Light_Mode(DARK_ALL);
    Light_Mode(PREPARED);

    while (Button_Scan(Button_1_GPIO_Port, Button_1_Pin))
        ;
}

/******************************************
 * @brief Function of the task b
 */
void Task_B()
{
    int time_turn = 1000;

    float speed_run = 0.20;
    float direction_turn = 0.5;

    // 直行
    Straight(speed_run);

    // 设置任务灯
    data_light_temp.status = TASK_B;
    xQueueOverwrite(Queue_Light, &data_light_temp);

    // 等待香橙派发送转向信息
    do {
        xQueueReceive(Queue_Opencv, &data_opencv_temp, 100);
        osDelay(50);
    } while (data_opencv_temp.flag_turn == 0);

    // 左转
    Turn_Left(speed_run, direction_turn);

    osDelay(time_turn);

    // 右转
    Turn_Right(speed_run, direction_turn);

    osDelay(time_turn);

    // 直行
    Straight(speed_run);

    // 等待一段时间，或者超过同向行驶的车辆
    osDelay(2000);

    // 右转
    Turn_Right(speed_run, direction_turn);

    osDelay(time_turn);

    // 左转
    Turn_Left(speed_run, direction_turn);

    osDelay(time_turn);

    // 直行
    Straight(speed_run);

    // 闪三次
    Light_Mode(FLASH_THREE);

    // 等待会车结束
    int flag_pass = 0;
    do {
        // TOF 距离突变来检测旁边的车擦身而过
        if (1)
            flag_pass++;
    } while (flag_pass > 10);

    // 左转
    Turn_Left(speed_run, direction_turn);

    osDelay(time_turn);

    // 右转
    Turn_Right(speed_run, direction_turn);

    osDelay(time_turn);

    // 直行
    Straight(speed_run);

    // 检测到墙后进行下一步
    Scan_Block_Next(150);

    // 刹车
    Brake();

    while (Button_Scan(Button_1_GPIO_Port, Button_1_Pin))
        ;
}

/******************************************
 * @brief Function of the task b
 */
void Task_C()
{
    data_light_temp.status = TASK_C;
    xQueueOverwrite(Queue_Light, &data_light_temp);
}

/******************************************
 * @brief Function of the debug
 */
void Model_Debug()
{
    data_light_temp.status = MODEL_DEBUG;
    xQueueOverwrite(Queue_Light, &data_light_temp);

    for (;;) {

        if (0)
            if (xQueueReceive(Queue_Opencv, &data_opencv_temp, 0) == pdTRUE) {
                Show_Data_OpenCV(data_opencv_temp, lisii);
            }

        if (1)
            if (xQueueReceive(Queue_Tof, &data_tof_temp, 0) == pdTRUE) {
                Show_Data_Tof(data_tof_temp, lisii);
            }

        if (0) {

            /**
             * 面向车头 
             * 右边 1
             * 后面 3
             * 前面 5
             */

            char send_button[10];
            int size_button = sprintf(send_button, "B- %d\n",
                Button_Scan(Button_1_GPIO_Port, Button_1_Pin));

            lisii.Send(send_button, size_button);
        }

        Set_Speed(0, 20);

        osDelay(5);

        if (!Button_Scan(Button_1_GPIO_Port, Button_1_Pin)) {
            Light_Mode(PREPARED);
            osDelay(300);
            break;
        }
    }
}

void Set_Speed(float speed, float distance)
{
    data_motion_temp = { speed, distance };
    xQueueOverwrite(Queue_Motion, &data_motion_temp);
}

void Light_Mode(int mode)
{
    data_light_temp.status = mode;
    xQueueOverwrite(Queue_Light, &data_light_temp);
}

void Turn_Left(float speed, float direction)
{
    Light_Mode(LIGHT_LEFT);
    Set_Speed(speed, -direction);
}

void Turn_Right(float speed, float direction)
{
    Light_Mode(LIGHT_RIGHT);
    Set_Speed(speed, direction);
}

void Straight(float speed)
{
    Light_Mode(DARK_ALL);
    Set_Speed(speed, distance_right);
}

// 检测到前方的障碍物 有障碍物时进行下一步
void Scan_Block_Next(int distance)
{
    int flag_block = 0;
    xQueueReset(Queue_Tof);

    while (flag_block < 3) {

        xQueueReceive(Queue_Tof, &data_tof_temp, 10);

        if (data_tof_temp.distance < distance)
            flag_block++;

        osDelay(50);
    };
}

// 检测到前方的障碍物 没有障碍物时才能进行下一步
void Scan_Block_Stop(int distance)
{
    int flag_block = 0;
    xQueueReset(Queue_Tof);

    while (flag_block < 3) {

        xQueueReceive(Queue_Tof, &data_tof_temp, 10);

        if (data_tof_temp.distance > distance)
            flag_block++;

        osDelay(50);
    };
}

void Brake()
{
    Light_Mode(LIGHT_TAIL);

    Set_Speed(-0.4, distance_right);

    osDelay(800);

    Set_Speed(0, distance_right);

    Light_Mode(DARK_ALL);
    Light_Mode(PREPARED);
}
