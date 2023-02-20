#include "systick_ms.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

static uint32_t sys_tick = 0;

static void SysTick_ISR()
{
    sys_tick++;
}

void SysTickMs_Init()
{
    SysTickPeriodSet(SysCtlClockGet() / 1000);
    SysTickIntRegister(SysTick_ISR);
    SysTickIntEnable();
    SysTickEnable();
}

uint32_t SysTickMs_GetTick()
{
    return sys_tick;
}
