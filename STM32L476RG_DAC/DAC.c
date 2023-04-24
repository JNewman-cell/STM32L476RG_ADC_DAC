#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE4;
	GPIOA->MODER |= GPIO_MODER_MODE4;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;
}

void DAC_Write_Value(uint32_t value) {
	// DAC1Ch1 right-aligned 12-bit value
	if (value > 4095) {
		DAC->DHR12R1 = 0x0FFF;
	} else if (value < 1){
		DAC->DHR12R1 = 1;
	} else {
		DAC->DHR12R1 = value; 
	}
}

void DAC_Init(void) {
	DAC_Pin_Init();

	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	DAC->CR  &= ~DAC_CR_EN1;
	
	DAC->CR |= DAC_CR_TEN1;
	DAC->CR &= ~DAC_CR_TSEL1;
	DAC->CR |= DAC_CR_TSEL1;
	
	DAC->CR &= ~DAC_CR_TEN1;

	DAC->MCR &= ~DAC_MCR_MODE1;

	DAC->CR |= DAC_CR_EN1;
	
	DAC_Write_Value(0);
}
