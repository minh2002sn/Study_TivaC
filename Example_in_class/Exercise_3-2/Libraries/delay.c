#include "delay.h"

void delay_ms(uint32_t time_ms)
{
    uint32_t i ,freq = SysCtlClockGet() / 1000;
    for(i = 0; i < time_ms; i++)
    {
        SysCtlDelay(freq / 3);
    }
}

//void delay_us(uint32_t time_us)
//{
//    uint32_t i ,freq = SysCtlClockGet() / 1000000;
//    for(i = 0; i < time_us; i++)
//    {
//        SysCtlDelay(freq / 3);
//    }
//}
