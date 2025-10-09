
#include <stdint.h>
#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 115200
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

#define UCSR0A (*(volatile uint8_t*)0xC0) //status
#define UCSR0B (*(volatile uint8_t*)0xC1) //activare tx/rx
#define UCSR0C (*(volatile uint8_t*)0xC2) //Format
#define UBRR0L (*(volatile uint8_t*)0xC4) //baud low
#define UBRR0H (*(volatile uint8_t*)0xC5) //baud high
#define UDR0   (*(volatile uint8_t*)0xC6) //read/write


#define UDRE0  5 

#define TXEN0  3 

#define UCSZ01 2 
#define UCSZ00 1


void uart_init(void){
	UBRR0H = (uint8_t)(UBRR_VALUE >>8);
	UBRR0L = (uint8_t)(UBRR_VALUE);
	UCSR0B = (1<<TXEN0) | (1 <<RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uartPutC(char c){
	while(1(UCSR0A & (1 << UDRE0)));
	UDR0=c;
	
}

void uart_puts(const char *s){
	while(*s){
		uartPutC(*s++);
		
	}
}

char uartGetC(void){
	while(!(UCSR0A & (1 << RXC0)))
	return UDR0;
}

int main(void) {
	uart_init();
	uart_puts("Polling mode: send me chars\r\n");

	while (1) {
		char c = uartGetC();
		uartPutC(c);
	}
	
	return 0;
}