/*
 * stm32f3xx_it.c
 *
 *  Created on: Dec 4, 2017
 *      Author: technix
 */

#include <stm32f1xx_it.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

void __stack(void);
void Reset_IRQHandler(void);

__attribute__((section(".vector"))) void (*ISR_Vector[])(void) =
{
		__stack,
		Reset_IRQHandler,
#define IRQN_HANDLER(irqn, name) name ##_IRQHandler,
#define SKIP_HANDLER(irqn) NULL,
#include <stm32f1xx_handler.h>
#undef IRQN_HANDLER
#undef SKIP_HANDLER
};

extern const struct copyitem
{
	const void *src;
	void *dst;
	uint32_t size;
} *__copy_addr;
extern uint32_t __copy_size;

extern const struct zeroitem
{
	void *dst;
	uint32_t size;
} *__zero_addr;
extern uint32_t __zero_size;

__attribute__((noreturn)) extern void _start(void);

__attribute__((noreturn)) void Reset_IRQHandler(void)
{
	SystemInit();

	SCB->VTOR = (uint32_t)ISR_Vector;

	uint32_t count = __copy_size / sizeof(struct copyitem);
	for (uint32_t idx = 0; idx < count; idx++)
	{
		const struct copyitem *item = &(__copy_addr[idx]);
		memcpy(item->dst, item->src, item->size);
	}

	count = __zero_size / sizeof(struct zeroitem);
	for (uint32_t idx = 0; idx < count; idx++)
	{
		const struct zeroitem *item = &(__zero_addr[idx]);
		memset(item->dst, 0, item->size);
	}

	SystemCoreClockUpdate();

	_start();
}

__attribute__((noreturn)) void Default_IRQHandler(void)
{
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
		__BKPT(0);

	for (;;)
		__WFE();
}

__attribute__((noreturn)) void _exit(int status)
{
	if (!status && (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk))
		__BKPT(0);

	SCB->AIRCR =
			(0x05fa << SCB_AIRCR_VECTKEY_Pos) |
			SCB_AIRCR_SYSRESETREQ_Msk;

	for (;;)
		__WFE();
}

#define IRQN_HANDLER(irqn, name) __attribute__((weak, alias("Default_IRQHandler"))) void name ##_IRQHandler(void);
#define SKIP_HANDLER(irqn)
#include <stm32f1xx_handler.h>
#undef IRQN_HANDLER
#undef SKIP_HANDLER
