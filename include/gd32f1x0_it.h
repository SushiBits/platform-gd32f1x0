/*
 * stm32f1xx_it.h
 *
 *  Created on: Jan 8, 2018
 *      Author: technix
 */

#ifndef PLATFORM_INCLUDE_STM32F1XX_IT_H_
#define PLATFORM_INCLUDE_STM32F1XX_IT_H_

#include <stm32f1xx.h>

extern void (*ISR_Vector[])(void);

#define IRQN_HANDLER(irqn, name) void name ##_IRQHandler(void);
#define SKIP_HANDLER(irqn)
#include <stm32f1xx_handler.h>
#undef IRQN_HANDLER
#undef SKIP_HANDLER

#endif /* PLATFORM_INCLUDE_STM32F1XX_IT_H_ */
