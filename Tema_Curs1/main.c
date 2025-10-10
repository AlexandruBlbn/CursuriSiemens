

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	for(uint8_t i = 0; i < 8; i++){
		DDRA |= (1<<i);
	}
	PORTA |= (1<<PA0) | (1<<PA1);
	

    while (1) 
    {
		_delay_ms(1000);
		for(int i=0; i<=5; i++){
			PORTA = PORTA << 1;
			_delay_ms(1000);
		}
		for(int i=0; i<=5; i++){
			PORTA = PORTA >> 1;
			_delay_ms(1000);
		}
		
	
    }
}

