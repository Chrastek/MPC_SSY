#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "my_lib/uart.h"
#include "my_lib/adc.h"

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);
char buffer[30];

void board_init() {
	UART_init(38400);
	stdout = &uart_str;
}

int main(void) {
	board_init();

	while (1) {
		ADC_Init(128, 1); //inicializace ADC -> prescaler 128 ; uref = AVCC

		uint16_t moisture = ADC_get(0); // ADC0 = PF0
		ADC_stop();

		// Vypsání intenzity p?es UART
		sprintf(buffer, "Moisture: (ADC0): %u\r\n", moisture);
		UART_SendString(buffer);

		_delay_ms(1000); // ?ekání 1s mezi m??eními
	}

	return 0;
}
