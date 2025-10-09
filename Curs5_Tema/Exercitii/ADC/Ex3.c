#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 115200
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)
#define BUFFER_SIZE 64

volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t command_ready = 0;
volatile uint8_t adc_active = 0;

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

void adc_init(void) {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | 0x07;
}

uint16_t adc_read(void) {
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void process_command(void) {
	if (strcmp((char*)rx_buffer, "adc_on") == 0) {
		adc_active = 1;
		uart_puts("ADC started\r\n");
	}
	else if (strcmp((char*)rx_buffer, "adc_off") == 0) {
		adc_active = 0;
		uart_puts("ADC stopped\r\n");
	}
	else if (strcmp((char*)rx_buffer, "adc_status") == 0) {
		if (adc_active) {
			uart_puts("ADC is ON\r\n");
			} else {
			uart_puts("ADC is OFF\r\n");
		}
	}
	else if (strcmp((char*)rx_buffer, "help") == 0) {
		uart_puts("Available commands:\r\n");
		uart_puts("  adc_on     - Start ADC measurements\r\n");
		uart_puts("  adc_off    - Stop ADC measurements\r\n");
		uart_puts("  adc_status - Check ADC state\r\n");
		uart_puts("  help       - Show this message\r\n");
	}
	else {
		uart_puts("Unknown command. Type 'help' for list of commands\r\n");
	}
}

ISR(USART0_RX_vect) {
	char c = UDR0;
	
	if (c == '\r' || c == '\n') {
		if (rx_index > 0) {
			rx_buffer[rx_index] = '\0';
			command_ready = 1;
		}
	}
	else if (rx_index < BUFFER_SIZE - 1) {
		rx_buffer[rx_index++] = c;
	}
}

int main(void) {
	uart_init();
	adc_init();
	
	uart_puts("System ready. Type 'help' for commands\r\n");
	
	uint16_t counter = 0;
	
	while (1) {
		if (command_ready) {
			process_command();
			rx_index = 0;
			command_ready = 0;
		}
		
		if (adc_active) {
			if (counter >= 1000) {
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
				
				counter = 0;
			}
			counter++;
			_delay_ms(1);
		}
		else {
			_delay_ms(10);
		}
	}
	
	return 0;
}