#ifndef LCD1602_CLASS

#define LCD1602_CLASS

#define LCD1602_DATA PORTC
#define LCD1602_RS PORTD7
#define LCD1602_RW PORTG0
#define LCD1602_E PORTG1
#define LCD1602_BACKLIGHT PORTG2
#include <util/delay.h>
#include "fifo.h"

class LCD1602 {
	public:
	
		static void init(void){
			DDRG |= ((1 << LCD1602_RW) | (1 << LCD1602_E) | (1 << LCD1602_BACKLIGHT));
			DDRD |= (1 << LCD1602_RS);
			DDRC = 0xFF;
			
			
			
			writeInstruction(0x38); //Function Set: 8bit 2-line 5x8
			_delay_us(40);
			writeInstruction(0x38); 
			_delay_us(40);
			
			writeInstruction(0x06); //address increment
			_delay_us(40);

			writeInstruction(0x0C); //dispaly on
			
			_delay_us(40);
			
			clear();
			backlightOn();

			
		}
		
		static void clear() {
			wait();
			writeInstruction(0x01); 
		}
		
		static void backlightOn() {
			PORTG |= (1 << LCD1602_BACKLIGHT);
			printf("on %x\r\n", PING);
		}
		
		static void backlightOff() {
			PORTG &= ~(1 << LCD1602_BACKLIGHT);
		}
		
		static void moveTo(unsigned char line, unsigned char pos){
			wait();
			unsigned char addr = (line==1) ? 0x00 : 0x40;
			addr += (pos-1);
			writeInstruction(addr | 0x80);
		}
		
		static int _getchar(FILE *file) {
			return 0;
		}
		
		
		static int _putchar(char c, FILE *file) {			
			writeData(c);
			return 0;
		}

		
		
		static FILE *getStream(void){
			if (!stream){
				stream = fdevopen(&LCD1602::_putchar, &LCD1602::_getchar);
			}
			return stream;
		}
		
		
	protected:
	
		
		static FILE * stream;
	
	
		static void wait() {
			PORTG |= (1<<LCD1602_RW);
			DDRC = 0x00;
			LCD1602_DATA = 0xFF;
			char volatile bf = 1;
			PORTD &= ~(1<<LCD1602_RS);
			while(bf) {
				PORTG |= (1<<LCD1602_E);
				_delay_ms(1);
				bf = PINC & 0x80;
				PORTG &= ~(1<<LCD1602_E);
				_delay_ms(1);
			}
			DDRC = 0xFF;
		}
	
		static void _write(unsigned char data) {
			PORTG &= ~(1<<LCD1602_RW);
			LCD1602_DATA = data;			
			PORTG |= (1<<LCD1602_E);
			_delay_ms(1);
			PORTG &= ~(1<<LCD1602_E);
			_delay_ms(1);			
		}
	
		static void writeInstruction(unsigned char instruction_code) {
			PORTD &= ~(1<<LCD1602_RS);
			_write(instruction_code);			
		}
	
		static void writeData(unsigned char data) {
			PORTD |= 1<<LCD1602_RS;
			_write(data);
		}
		
		
	
	
};

FILE * LCD1602::stream = NULL;


#endif