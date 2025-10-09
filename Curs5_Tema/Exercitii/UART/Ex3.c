#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 115200
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

volatile char received_char = 0;
volatile uint8_t data_ready = 0;

void uart_init(void) {
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)(UBRR_VALUE);
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	sei();
}

void uart_putc(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_puts(const char *s) {
	while (*s) {
		uart_putc(*s++);
	}
}

ISR(USART0_RX_vect) {
	received_char = UDR0;
	data_ready = 1;
}

int main(void) {
	uart_init();
	uart_puts("Curs SIEMENS, trimite caractere:\r\n");
	
	while (1) {
		if (data_ready) {
			uart_putc(received_char);
			data_ready = 0;
		}
	}

}