#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

void adc_init(void){
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN) | 0x07;
	DDRA |= (1 << PA0) | (1 << PA1);
}

//V=IR, I~=50mA, R = Serie, V = 5v
// 

void ADC_verificare(uint16_t valoareADC){
	if(valoareADC < 307){

		PORTA |= (1<<PA0);
		PORTA &= ~(1<<PA1);
	}
	else {
		PORTA &= ~(1<<PA0);
		PORTA |= (1<<PA1);
	}
	
}


int main(void) {
	adc_init();

	

	while (1) {
		ADCSRA |= (1<<ADSC);
		while (ADCSRA & (1 << ADSC));
		uint16_t val = ADC;
		ADC_verificare(val);
	}
}

