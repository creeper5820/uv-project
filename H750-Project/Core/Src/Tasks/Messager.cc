#include "Messager.hh"

static int model = MODEL_DEBUG;
uint8_t flag_debug = 1;

// Message caches
static Data_OpenCV data_opencv_temp;
static Data_Motion data_motion_temp;
static Data_Tof data_tof_temp;
static Data_Light data_light_temp;
static int flag_end;
extern Serial_Transceiver lisii;

// Queue to use
auto Queue_Motion = xQueueCreate(2, sizeof(Data_Motion));
auto Queue_Light = xQueueCreate(2, sizeof(Data_Light));
auto Queue_System = xQueueCreate(2, sizeof(Data_System));
auto Queue_Tof = xQueueCreate(2, sizeof(Data_Tof));
auto Queue_Opencv = xQueueCreate(2, sizeof(Data_OpenCV));
auto Queue_Offset = xQueueCreate(2, sizeof(int));

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
    flag_end = 0;

    Light_Mode(FLASH_ALL);

    data_light_temp.task = PREPARE;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    if (flag_debug)
        lisii.Send((char*)"Wait OrangePi to send message\n", sizeof("Wait OrangePi to send message"));

    // 等待香橙派发送信息
    if (1)
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

    // 等待TOF被遮挡，遮挡距离小于10cm
    if (1) {
        while (flag_end > 10) {

            xQueueReceive(Queue_Tof, &data_tof_temp, 1000);
            osDelay(50);

            if (data_tof_temp.distance < 10)
                flag_end++;
        }
        flag_end = 0;
    }

    if (flag_debug)
        lisii.Send((char*)"Done!\n", sizeof("Done!"));

    // 灯光全部亮起
    data_light_temp.status = FLASH_ALL;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    if (flag_debug)
        lisii.Send((char*)"Wait hand removed\n", sizeof("Wait hand removed"));

    // 清空消息队列
    xQueueReset(Queue_Tof);

    // 等待TOF遮挡消失
    if (1) {
        while (flag_end > 10) {

            xQueueReceive(Queue_Tof, &data_tof_temp, 1000);
            osDelay(50);

            if (data_tof_temp.distance > 20)
                flag_end++;
        }
        flag_end = 0;
    }

    // 灯光全部暗下
    data_light_temp.status = DARK_ALL;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    if (flag_debug)
        lisii.Send((char*)"Done!\n", sizeof("Done!"));

    data_motion_temp = { 0, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    osDelay(1000);
}

/******************************************
 * @brief Function of the task a
 */
void Task_A()
{
    xQueueReset(Queue_Tof);
    xQueueReset(Queue_Motion);

    data_motion_temp = { 0, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    data_tof_temp.distance = 250;

    data_light_temp.task = TASK_A;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    data_motion_temp = { 0.24, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 100);

    // 无限阻塞以调试
    if (0)
        while (1)
            osDelay(50);

    // 清空消息队列并防止抖动
    xQueueReset(Queue_Tof);
    osDelay(500);

    // 检测到前方的墙
    flag_end = 0;
    while (flag_end > 10) {

        xQueueReceive(Queue_Tof, &data_tof_temp, 0);
        osDelay(50);

        if (data_tof_temp.distance < 200)
            flag_end++;
    };

    if (flag_debug)
        lisii.Send((char*)"stop\n", sizeof("stop"));

    // 减速灯亮
    data_light_temp.status = LIGHT_TAIL;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    data_motion_temp = { -0.3, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    osDelay(100);

    data_motion_temp = { 0, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    data_light_temp.status = DARK_ALL;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    while (1)
        ;
}

/******************************************
 * @brief Function of the task b
 */
void Task_B()
{
    int time_turn = 0;

    float speed_run = 0.20;
    float direction_turn = 0.5;

    Straight(speed_run);

    data_light_temp.task = TASK_B;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    // 等待香橙派发送转向信息
    do {
        xQueueReceive(Queue_Opencv, &data_opencv_temp, 100);
        osDelay(50);
    } while (data_opencv_temp.flag_turn == 0);

    Turn_Left(speed_run, direction_turn);

    osDelay(time_turn);

    Turn_Right(speed_run, direction_turn);

    osDelay(time_turn);

    // 灯光暗
    data_light_temp.status = DARK_ALL;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    // 回到直行状态
    data_motion_temp = { speed_run, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    // 等待一段时间，或者超过同向行驶的车辆
    osDelay(2000);

    Turn_Right(speed_run, direction_turn);

    osDelay(time_turn);

    Turn_Left(speed_run, direction_turn);

    osDelay(time_turn);

    // 灯光暗
    data_light_temp.status = DARK_ALL;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    //回到直行状态
    data_motion_temp = { speed_run, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    // 闪三次
    data_light_temp.status = FLASH_THREE;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    // 等待会车结束
    flag_end = 0;
    do {
        // TOF 距离突变来检测旁边的车擦身而过
        if (1)
            flag_end++;
    } while (flag_end > 10);

    // 左转向灯
    data_light_temp.status = LIGHT_LEFT;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    // 左转
    data_motion_temp = { speed_run, -direction_turn };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    osDelay(time_turn);

    // 右转向灯
    data_light_temp.status = LIGHT_RIGHT;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    // 右转
    data_motion_temp = { speed_run, direction_turn };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);

    osDelay(time_turn);
}

/******************************************
 * @brief Function of the task b
 */
void Task_C()
{
    data_light_temp.task = TASK_C;
    xQueueSend(Queue_Light, &data_light_temp, 100);
}

/******************************************
 * @brief Function of the debug
 */
void Model_Debug()
{
    data_light_temp.task = MODEL_DEBUG;
    xQueueSend(Queue_Light, &data_light_temp, 100);

    for (;;) {

        if (1)
            if (xQueueReceive(Queue_Opencv, &data_opencv_temp, 0) == pdTRUE) {
                Show_Data_OpenCV(data_opencv_temp, lisii);
            }

        if (0)
            if (xQueueReceive(Queue_Tof, &data_tof_temp, 0) == pdTRUE) {
                Show_Data_Tof(data_tof_temp, lisii);
            }

        osDelay(100);
    }
}

void Turn_Left(float speed, float direction)
{
    // 左转向灯
    data_light_temp.status = LIGHT_LEFT;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    // 左转
    data_motion_temp = { speed, direction };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);
}

void Turn_Right(float speed, float direction)
{
    // 右转向灯
    data_light_temp.status = LIGHT_RIGHT;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    // 右转
    data_motion_temp = { speed, direction };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);
}

void Straight(float speed)
{
    // 灯光暗
    data_light_temp.status = DARK_ALL;
    xQueueSend(Queue_Light, &data_light_temp, 100);
    // 回到直行状态
    data_motion_temp = { speed, 0 };
    xQueueSend(Queue_Motion, &data_motion_temp, 50);
}

void Light_Mode(int mode)
{
    data_light_temp.status = mode;
    xQueueSend(Queue_Light, &data_light_temp, 100);
}