#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
}

void ADC_Pin_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	GPIOA->MODER |= GPIO_MODER_MODE1;
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	
	GPIOA->ASCR |= GPIO_ASCR_ASC1;
}

void ADC_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;
	RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;

  	// Other ADC Initialization
  	ADC_Pin_Init();
  	ADC_Common_Configuration();
	ADC_Wakeup();

	ADC1->CR |= ADC_CR_ADEN;
	ADC1->CR |= ADC_CR_ADDIS;
	
	ADC1->CFGR &= ~ADC_CFGR_RES;
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	
	ADC1->SQR1 &= ~ADC_SQR1_L;
	ADC1->SQR1 |= (ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2);
	
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;
	ADC1->SMPR1 |= (ADC_SMPR1_SMP6_0 | ADC_SMPR1_SMP6_1);
	
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	
	ADC1->CR |= ADC_CR_ADEN;
	
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}
