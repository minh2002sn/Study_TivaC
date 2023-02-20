#ifndef SYSTICK_MS_H
#define SYSTICK_MS_H

#include <stdint.h>
#include <stdbool.h>

void SysTickMs_Init();
uint32_t SysTickMs_GetTick();

#endif
