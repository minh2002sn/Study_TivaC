#include "button.h"

#ifdef USING_TIVA_C
#include "systick_ms.h"
#endif

#define TIME_FOR_LONG_PRESSING	2000

//========== btn func ==========//

button_callback_function_t __BUTTON_Pressing_Callback = NULL;
button_callback_function_t __BUTTON_Releasing_Callback = NULL;
button_callback_function_t __BUTTON_Short_Pressing_Callback = NULL;
button_callback_function_t __BUTTON_Long_Pressing_Callback = NULL;

static uint32_t GetTick()
{
#ifdef USING_TIVA_C
    return SysTickMs_GetTick();
#else
    return HAL_GetTick();
#endif
}

static uint8_t GPIO_ReadPin(BUTTON_HandleTypedef *ButtonX)
{
    #ifdef USING_TIVA_C
        return (GPIOPinRead(ButtonX->GPIO_Port, ButtonX->GPIO_Pin) == 0) ? 0 : 1;
    #else
        return HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
    #endif
}

void BUTTON_Handle(BUTTON_HandleTypedef *ButtonX)
{
	uint8_t state = GPIO_ReadPin(ButtonX);
	if(state != ButtonX->BTN_Filter)
	{
		ButtonX->BTN_Filter = state;
		ButtonX->is_debouncing = 1;
		ButtonX->time_debounce = GetTick();
	}

	if(ButtonX->is_debouncing && (GetTick() - ButtonX->time_debounce >= 15))
	{
		ButtonX->BTN_Current = ButtonX->BTN_Filter;
		ButtonX->is_debouncing = 0;
	}

	if(ButtonX->BTN_Current != ButtonX->BTN_Last)
	{
		if(ButtonX->BTN_Current == 0)
		{
			if(__BUTTON_Pressing_Callback != NULL)
			{
				__BUTTON_Pressing_Callback(ButtonX);
			}
			ButtonX->is_press = 1;
			ButtonX->time_start_press = GetTick();
		}
		else{
			ButtonX->is_press = 0;
			if(GetTick() - ButtonX->time_start_press <= 1000)
			{
				if(__BUTTON_Short_Pressing_Callback != NULL)
				{
					__BUTTON_Short_Pressing_Callback(ButtonX);
				}
			}
			if(__BUTTON_Releasing_Callback != NULL)
			{
				__BUTTON_Releasing_Callback(ButtonX);
			}
		}
		ButtonX->BTN_Last = ButtonX->BTN_Current;
	}

	if(ButtonX->is_press && (GetTick() - ButtonX->time_start_press >= TIME_FOR_LONG_PRESSING))
	{
		if(__BUTTON_Long_Pressing_Callback != NULL)
		{
			__BUTTON_Long_Pressing_Callback(ButtonX);
		}
		ButtonX->is_press = 0;
	}
}

#ifdef USING_TIVA_C
void BUTTON_Init(BUTTON_HandleTypedef *ButtonX, uint32_t GPIO_Port, uint8_t GPIO_Pin)
#else
void BUTTON_Init(BUTTON_HandleTypedef *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
#endif
{
    ButtonX->GPIO_Port = GPIO_Port;
	ButtonX->GPIO_Pin = GPIO_Pin;
	ButtonX->BTN_Current = 1;
	ButtonX->BTN_Last = 1;
	ButtonX->BTN_Filter = 1;
	ButtonX->is_debouncing = 0;
	ButtonX->is_press = 0;
}

void BUTTON_Set_Callback_Function(button_callback_function_t p_pressing_callback_function,
								button_callback_function_t p_releasing_callback_function,
								button_callback_function_t p_short_pressing_callback_function,
								button_callback_function_t p_long_pressing_callback_function){
	__BUTTON_Pressing_Callback = p_pressing_callback_function;
	__BUTTON_Releasing_Callback = p_releasing_callback_function;
	__BUTTON_Short_Pressing_Callback = p_short_pressing_callback_function;
	__BUTTON_Long_Pressing_Callback = p_long_pressing_callback_function;
}

