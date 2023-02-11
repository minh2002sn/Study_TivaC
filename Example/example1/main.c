#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c1230h6pm.h"
uint8_t test = 2;
void    delay(void);
void    delay(void)
{
    int volatile count;
    count = 0;
    while (count<=1000000) ++count;
}
int main(void)
{
	uint8_t ui8LED = 2;
	volatile int i;
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	while(1)
	{
		// Turn on the LED
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
//	    GPIO_PORTF_DATA_R |= 1<<1;
	    (*((volatile uint32_t *)(0x40025000 +(1<<3) ))) = 1<<1;
		// Delay for a bit
//		i = 0;
//		while (i<=1000000) ++i;
	    delay();
		// Cycle through Red, Green and Blue LEDs
		//if (ui8LED == 8) {ui8LED = 2;} else {ui8LED = ui8LED*2;}
		//GPIO_PORTF_DATA_R &= ~(1<<1);
		(*((volatile uint32_t *)(0x40025000 +(1<<3) ))) = 0;
        i = 0;
        while (i<=1000000) ++i;
	}
}
