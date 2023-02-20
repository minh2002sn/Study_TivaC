#include "main.h"
#include "delay.h"
#include "systick_ms.h"
#include "button.h"
#include "Command_Line.h"

uint32_t blink_timer = 0;
BUTTON_HandleTypedef button1;

enum
{
    LED_COLOR_RED = GPIO_PIN_1,
    LED_COLOR_BLUE = GPIO_PIN_2,
    LED_COLOR_GREEN = GPIO_PIN_3,
};

uint8_t led_pin = LED_COLOR_RED;

void button_short_pressing_callback(BUTTON_HandleTypedef *button)
{
    if(button == &button1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, led_pin);
    }
}

void button_long_pressing_callback(BUTTON_HandleTypedef *button)
{
    if(button == &button1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    }
}

void UART0_ISR()
{
    uint8_t rx_data;
    uint32_t ui32Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ui32Status);

    if(UARTCharsAvail(UART0_BASE))
    {
        rx_data = UARTCharGetNonBlocking(UART0_BASE);
        UARTCharPutNonBlocking(UART0_BASE, rx_data);
        COMMAND_LINE_Receive(rx_data);
    }

    while(UARTCharsAvail(UART0_BASE))
    {
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE)); //echo character
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTIntRegister(UART0_BASE, UART0_ISR);
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_TX | UART_INT_RX);
    IntMasterEnable();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);

    SysTickMs_Init();
    BUTTON_Init(&button1, GPIO_PORTF_BASE, GPIO_PIN_0);
    BUTTON_Set_Callback_Function(NULL, NULL, button_short_pressing_callback, button_long_pressing_callback);

    while(1)
    {
        BUTTON_Handle(&button1);

        COMMAND_LINE_Handle();

    }

}
