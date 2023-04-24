/*
 * ECE 153B - Winter 2023
 *
 * Name(s):
 * Section:
 * Lab: 5A
 */


#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

int data;
volatile int pmw_data;
volatile float voltage;

int main(void) {
  // Initialization
  System_Clock_Init(); // Switch System Clock = 16 MHz
  ADC_Init();
	PWM_Init();

	data = 0;
	voltage = 0;
			
  while (1) {
		// Start Regular Conversion
		ADC1->CR |= ADC_CR_ADSTART;
			
		// Wait for ADC conversion
		while((ADC1->ISR & ADC_ISR_EOC) == 0);
			
		// Ready ADC data register
		data = ADC1->DR;
			
		//convert raw data to actual voltage
		voltage = 3.3*((4096-((float)data))/4096);
			
		//change the CCR value to match the ADC value
		if (data > 4000) {
			TIM2->CCR1 |= 4096;
		} else {
			TIM2->CCR1 &= ~TIM_CCR1_CCR1;
			TIM2->CCR1 |= data;
		}
			for(int i=0; i<1000; ++i); // Some Delay
    }
}