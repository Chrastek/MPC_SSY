#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "uart.h"
#include "adc.h"

#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"


int uart_putchar(char c, FILE *stream) {
	UART_SendChar(c);
	return 0;
}

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_RW);
char buffer[64];

static NWK_DataReq_t appDataReq;
static bool appDataReqBusy = false;
static uint8_t appDataReqBuffer[NWK_MAX_PAYLOAD_SIZE];

static void appDataConf(NWK_DataReq_t *req) {
	appDataReqBusy = false;
	(void)req;
}

void appInit(void) {
	NWK_SetAddr(APP_ADDR);
	NWK_SetPanId(APP_PANID);
	PHY_SetChannel(APP_CHANNEL);
	#ifdef PHY_AT86RF212
	PHY_SetBand(APP_BAND);
	PHY_SetModulation(APP_MODULATION);
	#endif
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, NULL);
}

void board_init() {
	UART_init(38400);
	stdout = &uart_str;
}

void print_menu(){
	UART_SendString("\n\r\n\rMENU:\n\r");
	UART_SendString("1 = Read value from Moisture sensor\n\r");
	UART_SendString("2 = Battery status\n\r");
	UART_SendString("3 = Clear terminal\n\r");
	UART_SendString("0 = Exit\n\r\n\r");
}

void sendLwmMessage(const char *msg) {
	while (appDataReqBusy) {
		SYS_TaskHandler();
	}

	
	snprintf((char*)appDataReqBuffer, sizeof(appDataReqBuffer), "%s\r\n", msg);

	appDataReq.dstAddr = 0x0001;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.options = NWK_OPT_ACK_REQUEST;
	appDataReq.size = strlen((char*)appDataReqBuffer) + 1;
	appDataReq.data = appDataReqBuffer;
	appDataReq.confirm = appDataConf;

	NWK_DataReq(&appDataReq);
	appDataReqBusy = true;
}


int main(void)
{
	SYS_Init();
	board_init();
	appInit();

	int run = 1;
	char choice;

	while(run){
		SYS_TaskHandler();

		print_menu();
		choice = UART_GetChar();

		switch (choice){
			case '0':
			run = 0;
			UART_SendString("Quiting\r\n");
			sendLwmMessage("Ukonceni programu");
			break;

			case '1': {
				ADC_Init(128, 1);
				uint16_t moisture = ADC_get(0);
				ADC_stop();

				sprintf(buffer, "Moisture (ADC0): %u\r\n", moisture);
				UART_SendString(buffer);

				if (moisture >= 290 && moisture <= 311) {
					UART_SendString("Puda je sucha.\r\n");
					sendLwmMessage("Puda je sucha");
					} else if (moisture >= 220 && moisture < 290) {
					UART_SendString("Puda je vlhka.\r\n");
					sendLwmMessage("Puda je vlhka");
					} else if (moisture >= 170 && moisture < 220) {
					UART_SendString("Puda je mokra.\r\n");
					sendLwmMessage("Puda je mokra");
					} else {
					UART_SendString("Hodnota mimo rozsah - zkontroluj senzor.\r\n");
					sendLwmMessage("Vlhkost mimo rozsah");
				}

				_delay_ms(1000);
				while (appDataReqBusy) {
        SYS_TaskHandler();
    }
				break;
			}

	case '2': {
    ADC_Init(128, 1);
    uint16_t adc_val = ADC_get(1);
    ADC_stop();

    float vcc = ((float)adc_val / 1023.0) * 3.3 * 2.0;
    char napeti_str[10];
    dtostrf(vcc, 4, 2, napeti_str);

    sprintf(buffer, "Napeti baterie: %s V\r\n", napeti_str);
    UART_SendString(buffer);

    if (vcc > 3.0) {
        sendLwmMessage("Baterie OK");
    } else if (vcc >= 2.8) {
        sendLwmMessage("Baterie SLABA");
    } else {
        sendLwmMessage("Baterie KRITICKA");
    }
	
	while (appDataReqBusy) {
        SYS_TaskHandler();
    }
    break;
}

		case '3':
    UART_SendString("Terminal vymazan...\r\n");
    sendLwmMessage("Terminal vymazan");
    while (appDataReqBusy) {
        SYS_TaskHandler();
    }
    _delay_ms(100);  
    UART_SendString("\x1b[2J");
    break;

		}
	}

	return 0;
}
