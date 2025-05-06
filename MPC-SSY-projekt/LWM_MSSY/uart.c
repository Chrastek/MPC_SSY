#include <avr/io.h>
#include <stdio.h>

#define F_CPU 8000000UL  // Pokud není definováno jinde

void UART_init(uint16_t Baudrate){
	uint16_t ubrr = (F_CPU / 16 / Baudrate) - 1;

	// Nastavení baudrate
	UBRR1H = (uint8_t)(ubrr >> 8);
	UBRR1L = (uint8_t)ubrr;

	// Povolení p?íjmu a vysílání
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);

	// Rámcový formát: 8 datových bit?, žádná parita, 1 stop bit
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); // Async, 8N1
}

void UART_SendChar(uint8_t data){
	// ?ekání na prázdný buffer
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

void UART_SendString(char *text){
	while (*text != '\0') {
		UART_SendChar(*text++);
	}
}

uint8_t UART_GetChar(void){
	while (!(UCSR1A & (1 << RXC1)));
	return UDR1;
}


// Pro printf() p?es stdout
int UART_SendChar_stdio(char c, FILE *stream){
	if (c == '\n') {
		UART_SendChar('\r'); // P?idej \r p?ed \n pro Windows terminály
	}
	UART_SendChar(c);
	return 0;
}
