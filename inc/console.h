#ifndef CONSOLE_CLASS
#define CONSOLE_CLASS

#include <stdio.h>
#include <avr/interrupt.h>
#include "fifo.h"
#include <stdlib.h>


class Console {
	public:
		static void init(unsigned long int clock_hz, unsigned int baudrate, unsigned char buffer_size = 64) {
			initHardware(clock_hz,baudrate);		
			rx_buf = new FifoBuffer(buffer_size);
			tx_buf = new FifoBuffer(buffer_size);
			line_buf = (char*)malloc(buffer_size);
			sei();
		}			
		
		static int _getchar(FILE *file) {
			int ret;
			cli(); 
			if(!rx_buf->isEmpty()) {				
				ret = rx_buf->pop();				
			}
			else {
				ret = _FDEV_EOF;
			}
			sei();
			return ret;
		}
		
		static int _putchar(char c, FILE *file) {
			int ret;
			cli(); 
			if(!tx_buf->isFull()) {				
				tx_buf->push(c);				
				UCSRB |= ( 1 << UDRIE );
				ret = 0;
			}
			else {
				ret = -1;
			}
			sei();
			return ret;			
		}
		
		static FILE *getStream(void){
			if (!stream){
				stream = fdevopen(&Console::_putchar, &Console::_getchar);
			} 			
			return stream;		
		}
		
		static void rx_isr(){
			unsigned char rxbyte = UDR;
			
			
			if (rxbyte == 13) {				
				line_available = true;
			}
			else {				
				if(line_available) {
					line_length = 0;
					line_available = false;
				}
				line_length++;
			}
			
			
			if (echo) {
				if (rxbyte == 13) _putchar(10,stream);
				_putchar(rxbyte,stream);
			}
			if(!rx_buf->isFull()) {
				rx_buf->push(rxbyte);
			}
		}	
		
		static void tx_isr(){
			if(tx_buf->isEmpty()) {				
				UCSRB &= ~(1 << UDRIE);
			}
			else {										
				UDR = tx_buf->pop();
			}
		}
		
		static void enableEcho() {
			echo = true;
		}
		
		static void disableEcho() {
			echo = false;
		}
		
		static bool lineEntered() {
			return line_available;
		}
		
		static char* getLine() {			
			unsigned char pos=0;			
			while(!rx_buf->isEmpty()) {
				line_buf[pos] = rx_buf->pop();
				pos++; 
			}
			line_buf[pos] = 0;
			line_available = false;			
			
			return line_buf;
		}
		
		
	protected:
		static FifoBuffer *rx_buf;
		static FifoBuffer *tx_buf;	
		static FILE *stream;
		static bool echo;
		static bool line_available;
		static char *line_buf;
		static unsigned char line_length;
		
		char test;
		
		static void initHardware(unsigned long int clock_hz, unsigned int baudrate) {
			unsigned int ubrr = clock_hz/16/baudrate-1;
			UBRRH = (unsigned char)(ubrr>>8);
			UBRRL = (unsigned char)ubrr;
			/* Enable receiver and transmitter and rx interrupt */
			UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
			/* Set frame format: 8data, 1stop bit */
			//UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
			UCSRC = (1<<URSEL)|(3<<UCSZ0);
		}
		
};


FILE *Console::stream = NULL;
FifoBuffer *Console::rx_buf = NULL;
FifoBuffer *Console::tx_buf = NULL;
bool Console::echo = true;
bool Console::line_available = false;
char *Console::line_buf = NULL;
unsigned char Console::line_length = 0;


ISR( USART_RXC_vect) {	
	Console::rx_isr();
}


ISR(USART_UDRE_vect) {
	Console::tx_isr();
}

#endif 