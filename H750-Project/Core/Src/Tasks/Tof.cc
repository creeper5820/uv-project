#include "main.h"
#include "cmsis_os.h"

extern "C" {
void Tof_Loop(void *argument);
}

void Tof_Loop(void *argument)
{
    for (;;) {
        osDelay(100);
    }
}
