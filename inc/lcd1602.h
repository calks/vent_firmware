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
			if (c >= 0xA0) {
				switch (c) {
					case 0xC0: c = 0x41; break;//À
					case 0xC1: c = 0xA0; break;//Á
					case 0xC2: c = 0x42; break;//Â
					case 0xC3: c = 0xA1; break;//Ã
					case 0xC4: c = 0xE0; break;//Ä
					case 0xC5: c = 0x45; break;//Å
					case 0xA8: c = 0xA2; break;//¨
					case 0xC6: c = 0xA3; break;//Æ
					case 0xC7: c = 0xA4; break;//Ç
					case 0xC8: c = 0xA5; break;//È
					case 0xC9: c = 0xA6; break;//É
					case 0xCA: c = 0x4B; break;//Ê
					case 0xCB: c = 0xA7; break;//Ë
					case 0xCC: c = 0x4D; break;//Ì
					case 0xCD: c = 0x48; break;//H
					case 0xCE: c = 0x4F; break;//O
					case 0xCF: c = 0xA8; break;//Ï
					case 0xD0: c = 0x50; break;//P
					case 0xD1: c = 0x43; break;//C
					case 0xD2: c = 0x54; break;//T
					case 0xD3: c = 0xA9; break;//Ó
					case 0xD4: c = 0xAA; break;//Ô
					case 0xD5: c = 0x58; break;//X
					case 0xD6: c = 0xE1; break;//Ö
					case 0xD7: c = 0xAB; break;//×
					case 0xD8: c = 0xAC; break;//Ø
					case 0xD9: c = 0xE2; break;//Ù
					case 0xDA: c = 0xAD; break;//Ú
					case 0xDB: c = 0xAE; break;//Û
					case 0xDC: c = 0x62; break;//Ü
					case 0xDD: c = 0xAF; break;//Ý
					case 0xDE: c = 0xB0; break;//Þ
					case 0xDF: c = 0xB1; break;//ß
					case 0xE0: c = 0x61; break;//à
					case 0xE1: c = 0xB2; break;//á
					case 0xE2: c = 0xB3; break;//â
					case 0xE3: c = 0xB4; break;//ã
					case 0xE4: c = 0xE3; break;//ä
					case 0xE5: c = 0x65; break;//å
					case 0xB8: c = 0xB5; break;//¸
					case 0xE6: c = 0xB6; break;//æ
					case 0xE7: c = 0xB7; break;//ç
					case 0xE8: c = 0xB8; break;//è
					case 0xE9: c = 0xB9; break;//é
					case 0xEA: c = 0xBA; break;//ê
					case 0xEB: c = 0xBB; break;//ë
					case 0xEC: c = 0xBC; break;//ì
					case 0xED: c = 0xBD; break;//í
					case 0xEE: c = 0x6F; break;//î
					case 0xEF: c = 0xBE; break;//ï
					case 0xF0: c = 0x70; break;//ð
					case 0xF1: c = 0x63; break;//c
					case 0xF2: c = 0xBF; break;//ò
					case 0xF3: c = 0x79; break;//ó
					case 0xF4: c = 0xE4; break;//ô
					case 0xF5: c = 0x78; break;//õ
					case 0xF6: c = 0xE5; break;//ö
					case 0xF7: c = 0xC0; break;//÷
					case 0xF8: c = 0xC1; break;//ø
					case 0xF9: c = 0xE6; break;//ù
					case 0xFA: c = 0xC2; break;//ú
					case 0xFB: c = 0xC3; break;//û
					case 0xFC: c = 0xC4; break;//ü
					case 0xFD: c = 0xC5; break;//ý
					case 0xFE: c = 0xC6; break;//þ
					case 0xFF: c = 0xC7; break;//ÿ
				}
			}
			
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