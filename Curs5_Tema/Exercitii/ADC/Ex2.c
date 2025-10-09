#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define BAUD 115200
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void uart_init(void) {
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)(UBRR_VALUE);
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
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

void adc_init(void) {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | 0x07;
}

uint16_t adc_read(void) {
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

int main(void) {
	uart_init();
	adc_init();
	
	while (1) {
		uint16_t adc_value = adc_read();
		
		if (adc_value < 307) {
			uart_puts("Voltage is LOW\r\n");
		}
		else if (adc_value < 717) {
			uart_puts("Voltage is NORMAL\r\n");
		}
		else {
			uart_puts("Voltage is HIGH\r\n");
		}
		
		_delay_ms(1000);
	}
	
	return 0;
}