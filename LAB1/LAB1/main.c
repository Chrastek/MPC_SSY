/*
 * LAB1.c
 *
 * Created: 02.02.2020 9:01:38
 * Author : Ondra
 */ 

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "libs/libprintfuart.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

#define konst 2
#define ODECET
#define UPPER_CASE 1
#define NORMAL_CASE 0
#define DIRECTION_UP 1
#define DIRECTION_DOWN 0
#define ALPHABET_SIZE 52
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

//musime vytvorit soubor pro STDOUT
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void board_init(){
	UART_init(38400); //nastaveni rychlosti UARTu, 38400b/s
	stdout = &uart_str; //presmerovani STDOUT
}

int main(void)
{ 	
	board_init();
	_delay_ms(1000);
	printf("Hello world!\n");
    _delay_ms(1000);
	//4
	int promenna = 10; 
	#ifdef ODECET
		promenna -= konst;
	#endif

	printf("Hodnota promenne po odecteni: %d\n", promenna);
	
	//5
	unsigned char a1 = 255;
	unsigned char b1 = 255;
	unsigned int c1;
	c1 = (unsigned int)a1 + (unsigned int)b1;
	printf(" %u/n", c1);
	
	//6
	int a6 = 24;
	a6 = 24 >> 3;
	a6 = a6 - 1;
	a6 = a6 &0x2;
	printf("%d/n" , a6);
	
	//7
	int value = 200;
	char txt [80];
	char temp [20];
	sprintf(temp, "%d", value);
	strcpy(txt, "Hodnota=");
	strcat(txt, temp);
	printf("string.h a stdlib.h: %s\n", txt);
	sprintf(txt, "Hodnota=%d", value);
	printf("sprintf: %s\n", txt);
	
	//8
	// Globální pole pro uložení abecedy
	char alphabet[ALPHABET_SIZE];

	// Funkce pro generování abecedy
	void generateField(int letter_case) {
		if (letter_case != UPPER_CASE && letter_case != NORMAL_CASE) {
			printf("PROGRAM ERROR\n");
			return;
		}

		if (letter_case == UPPER_CASE) {
			for (int i = 0; i < 26; i++) {
				alphabet[i] = 'A' + i;         // Velká písmena A-Z
				alphabet[26 + i] = 'a' + i;    // Malá písmena a-z
			}
			} else if (letter_case == NORMAL_CASE) {
			for (int i = 0; i < 26; i++) {
				alphabet[i] = 'a' + i;         // Malá písmena a-z
				alphabet[26 + i] = 'A' + i;    // Velká písmena A-Z
			}
		}
	}

	// Funkce pro zm?nu velikosti písmen
	void capsLetters(int letter_case) {
		if (letter_case != UPPER_CASE && letter_case != NORMAL_CASE) {
			printf("PROGRAM ERROR\n");
			return;
		}

		for (int i = 0; i < ALPHABET_SIZE; i++) {
			if (letter_case == UPPER_CASE) {
				alphabet[i] = toupper(alphabet[i]);  // Zm?na na velká písmena
				} else {
				alphabet[i] = tolower(alphabet[i]);  // Zm?na na malá písmena
			}
		}
	}

	// Funkce pro výpis pole
	void printField(int direction) {
		if (direction != DIRECTION_UP && direction != DIRECTION_DOWN) {
			printf("PROGRAM ERROR\n");
			return;
		}

		if (direction == DIRECTION_UP) {
			for (int i = 0; i < ALPHABET_SIZE; i++) {
				printf("%c ", alphabet[i]);  // Výpis vzestupn?
			}
			} else if (direction == DIRECTION_DOWN) {
			for (int i = ALPHABET_SIZE - 1; i >= 0; i--) {
				printf("%c ", alphabet[i]);  // Výpis sestupn?
			}
		}
		printf("\n");
	}

	int main() {
		int letter_case = UPPER_CASE;   // Nastavení na velká písmena
		int direction = DIRECTION_UP;   // Nastavení výpisu vzestupn?

		// Generování abecedy
		generateField(letter_case);

		// Zm?na velikosti písmen na malá písmena
		capsLetters(NORMAL_CASE);

		// Výpis pole
		printField(direction);

		return 0;
	}
	

	int i = 0;
    while (1) 
    {
		_delay_ms(10000);
		i++;
		printf("Test x = %d \n\r", i);
    }
}
