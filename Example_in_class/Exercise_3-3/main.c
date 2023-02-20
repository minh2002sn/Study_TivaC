#include "main.h"
#include "systick_ms.h"
#include "button.h"

BUTTON_HandleTypedef button1;
BUTTON_HandleTypedef button2;

typedef enum
{
    STATE_IDLE,
    STATE_BLINK,
    STATE_STOP_BLINK,
    STATE_ALL_ON,
} STATUS_t;

static STATUS_t status = STATE_IDLE;
uint8_t blink_counter = 0;
uint32_t blink_timer = 0;

void button_short_pressing_callback(BUTTON_HandleTypedef *button);
void STATE_MACHINE_Handle();


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysTickMs_Init();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);

    BUTTON_Init(&button1, GPIO_PORTF_BASE, GPIO_PIN_0);
    BUTTON_Init(&button2, GPIO_PORTF_BASE, GPIO_PIN_4);
    BUTTON_Set_Callback_Function(NULL, NULL, button_short_pressing_callback, NULL);

    while(1)
    {
        BUTTON_Handle(&button1);
        BUTTON_Handle(&button2);
        STATE_MACHINE_Handle();
    }

}

void button_short_pressing_callback(BUTTON_HandleTypedef *button)
{
    if(button == &button1)
    {
        switch(status)
        {
            case STATE_IDLE:
                status = STATE_BLINK;
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
                blink_timer = SysTickMs_GetTick();
                blink_counter = 0;
                break;
            case STATE_BLINK:

                break;
            case STATE_ALL_ON:
                status = STATE_IDLE;
                break;
            default:
                break;
        }
    }

    if(button == &button2)
    {
        switch(status)
        {
            case STATE_IDLE:
                break;
            case STATE_BLINK:
                status = STATE_STOP_BLINK;
                break;
            case STATE_STOP_BLINK:
                status = STATE_BLINK;
                break;
            case STATE_ALL_ON:
                break;
            default:
                break;
        }
    }
}

void STATE_MACHINE_Handle()
{
    switch(status)
    {
        case STATE_IDLE:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
            break;
        case STATE_BLINK:
            if((SysTickMs_GetTick() - blink_timer) > 1000)
            {
                uint8_t led_state = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, (led_state) ? (~GPIO_PIN_2) : GPIO_PIN_2);
                blink_timer = SysTickMs_GetTick();
                blink_counter++;
            }

            if(blink_counter == 3)
            {
                status = STATE_ALL_ON;
            }

            break;
        case STATE_STOP_BLINK:

            break;
        case STATE_ALL_ON:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
            break;
        default:
            break;
    }
}


