#include "EXTI.h"

#include "DAC.h"

void EXTI_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	GPIOC->MODER &= ~GPIO_MODER_MODE13;
	RCC->APB2ENR = RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

static uint32_t dac_value = 64;
static enum {
    DOWN,
    UP,
} direction = UP;


void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;
	if (!(GPIOC->IDR & GPIO_IDR_ID13)) {
		if (direction == UP) {
			if (dac_value < DAC_MAX) {
				dac_value = dac_value+DAC_INCREMENT;
			} else {
				direction = DOWN;
				dac_value = dac_value-DAC_INCREMENT;
			}
		} else {
			if (dac_value > DAC_INCREMENT) {
				dac_value = dac_value-DAC_INCREMENT;
			} else {
				direction = UP;
				dac_value = dac_value+DAC_INCREMENT;
			}
		}
	}
	DAC_Write_Value(dac_value);
}
