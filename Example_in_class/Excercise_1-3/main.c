#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

void delay(uint32_t time_ms);
uint8_t  is_button_pressed();

uint8_t last_state = 1;
uint8_t current_state = 1;



int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;

    while(1)
    {
        if(is_button_pressed())
        {
            if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) == 0)
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            else
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        }
    }

}

void delay(uint32_t time_ms)
{
    uint32_t i;
    for(i = 0; i < time_ms; i++)
    {
        SysCtlDelay(40000 / 3);
    }
}

uint8_t is_button_pressed()
{
    current_state = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
    if(current_state != last_state)
    {
        delay(100);
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            return 1;
        }
        last_state = current_state;
    }
    return 0;
}
