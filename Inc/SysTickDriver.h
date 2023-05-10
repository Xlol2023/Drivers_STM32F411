/*
 * SysTickDriver.h
 *
 *  Created on: Apr 28, 2023
 *      Author: samuel
 */

#ifndef INC_SYSTICKDRIVER_H_
#define INC_SYSTICKDRIVER_H_

#include <stm32f4xx.h>

#define SYSTICK_LOAD_VALUE_16MHZ_1ms		16000 //Numeros de ciclos en 1ms
#define SYSTICK_LIAD_VALIE_100MHZ_1ms		100000 //Numero de ciclos en 1ms

void config_SysTick_ms(uint8_t systemClock);
uint64_t getTicks_ms(void);
void delay_ms(uint32_t wait_time_ms);

#endif /* INC_SYSTICKDRIVER_H_ */
