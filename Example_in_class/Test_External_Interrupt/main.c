#include "main.h"
#include "systick_ms.h"

uint8_t led_state;

void GPIO_ISR()
{
    static uint32_t timer = 0;

    GPIOIntClear(GPIO_PORTF_BASE, GPIOIntStatus(GPIO_PORTF_BASE, true));

    if((SysTickMs_GetTick() - timer) < 20)
    {
        return;
    }
    timer = SysTickMs_GetTick();

    led_state = !led_state;

}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysTickMs_Init();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);
    GPIOIntRegister(GPIO_PORTF_BASE, &GPIO_ISR);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    IntMasterEnable();

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, (led_state) ? (GPIO_PIN_1) : (~GPIO_PIN_1));
    }

}
