#include <stdio.h>
#include <avr/io.h>
#include "makra.h"

void ADC_Init(uint8_t prescale, uint8_t uref){
	ADCSRA = 0;
	ADCSRA |= (prescale << ADPS0);
	ADMUX &= 0x3F;
	ADMUX |= (uref << REFS0);
	sbi(ADCSRA, ADEN);
	while(!(ADCSRB & 0x80));
	while(!(ADCSRB & 0x20));
}

uint16_t ADC_get(uint8_t chan){
	uint16_t tmp = 0;

	ADMUX &= 0xF0;          
	ADCSRB &= ~(1 << MUX5); 

	if (chan < 8) {
		ADMUX |= chan;
	} else {
		chan -= 8;
		ADMUX |= chan;
		ADCSRB |= (1 << MUX5);
	}


	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)) {}

	tmp = ADC;
	ADCSRA |= (1 << ADIF);

	return tmp;
}

void ADC_stop(void){
	cbi(ADCSRA, ADEN);
}
