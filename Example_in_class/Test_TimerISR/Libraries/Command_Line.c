#include "Command_Line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

enum
{
    LED_COLOR_RED = GPIO_PIN_1,
    LED_COLOR_BLUE = GPIO_PIN_2,
    LED_COLOR_GREEN = GPIO_PIN_3,
};

extern uint8_t led_pin;

uint8_t cl_buf[255];
uint8_t cl_pointer = 0;
uint8_t cl_flag = 0;

void COMMAND_LINE_Init()
{

}

void COMMAND_LINE_Handle(){
	if(cl_flag)
	{
		char *arg_list[10];
		uint8_t arg_num = 0;

		char *temp_token = strtok((char *)cl_buf, " ");
		while(temp_token != NULL)
		{
			arg_list[arg_num++] = temp_token;
			temp_token = strtok(NULL, " ");
		}

		if(strstr(arg_list[0], "COLOR") != NULL)
		{
	        if(strstr(arg_list[0], "RED") != NULL)
	        {
	            led_pin = LED_COLOR_RED;
	        }
	        else if(strstr(arg_list[0], "BLUE") != NULL)
	        {
	            led_pin = LED_COLOR_BLUE;
	        }
	        else if(strstr(arg_list[0], "BLUE") != NULL)
            {
	            led_pin = LED_COLOR_GREEN;
            }
		}

		cl_pointer = 0;
		cl_flag = 0;
	}
}

void COMMAND_LINE_Receive(uint8_t rx_data)
{
	if(rx_data != '\n')
	{
		cl_buf[cl_pointer++] = rx_data;
	}
	else
	{
		cl_buf[cl_pointer] = '\0';
		cl_flag = 1;
	}
}
