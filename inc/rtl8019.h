#ifndef RTL8019_H
#define RTL8019_H

#include "rtl8019regs.h"

#define RTL_ADDR PORTA
#define RTL_ADDR_DDR DDRA

#define RTL_ADDR0 PA3
#define RTL_ADDR1 PA4
#define RTL_ADDR2 PA5
#define RTL_ADDR3 PA6
#define RTL_ADDR4 PA7

#define RTL_ADDR_MASK 0xF8

#define RTL_ADDR_SHIFT 3

#define RTL_DATA_OUT PORTF
#define RTL_DATA_IN PINF
#define RTL_DATA_DDR DDRF

#define RTL_CONTROL_PORT PORTB
#define RTL_CONTROL_DDR DDRB
#define RTL_RESET_DRV PB6
#define RTL_IOWB PB5
#define RTL_IORB PB4


#define F_CPU 14745600
void delay_ms(uint8_t ms) {
	uint16_t delay_count = F_CPU / 17500;
	volatile uint16_t i;
	
	while (ms != 0) {
		for (i=0; i != delay_count; i++);
		ms--;
	}
}





void test(void);

class RTL8019 {
	
	public:
		static void init(void) {
			RTL_ADDR_DDR |= ((1<<RTL_ADDR0) | (1<<RTL_ADDR1) | (1<<RTL_ADDR2) | (1<<RTL_ADDR3) | (1<<RTL_ADDR4));
			RTL_CONTROL_DDR |= ((1<<RTL_RESET_DRV) | (1<<RTL_IOWB) | (1<<RTL_IORB));
			RTL_DATA_DDR = 0x00;
			RTL_DATA_OUT = 0xFF;
			hardReset();
return;


    // do soft reset
    write(_ISR, 0xFF ) ;
    delay_ms(50);
    
    // switch to page 3 to load config registers
    write(CR, 0xE1);
    
    // disable EEPROM write protect of config registers
    write(RTL_EECR, 0xC0);
    
    // set network type to 10 Base-T link test
    write(CONFIG2, 0x20);
    
    // disable powerdown and sleep
    write(CONFIG3, 0);
    delay_ms(255);
    
    // reenable EEPROM write protect
    write(RTL_EECR, 0);
    
    // go back to page 0
    write(CR, 0x21);
    
    write(CR,0x21);       // stop the NIC, abort DMA, page 0
    delay_ms(2);               // make sure nothing is coming in or going out
    write(DCR, DCR_INIT);    // 0x58
    write(RBCR0,0x00);
    write(RBCR1,0x00);
    write(RCR,0x04);
    write(TPSR, TXSTART_INIT);
    write(TCR,0x02);
    write(PSTART, RXSTART_INIT);
    write(BNRY, RXSTART_INIT);
    write(PSTOP, RXSTOP_INIT);
    write(CR, 0x61);
    delay_ms(2);
    write(CURR, RXSTART_INIT);
    
    write(PAR0+0, 0x20);
    write(PAR0+1, 0x6A);
    write(PAR0+2, 0x8a);
    write(PAR0+3, 0x1a);
    write(PAR0+4, 0x0b);
    write(PAR0+5, 0x75);
    
    write(CR,0x21);
    write(DCR, DCR_INIT);
    write(CR,0x22);
    write(_ISR,0xFF);
    write(IMR, IMR_INIT);
    write(TCR, TCR_INIT);
    
    write(CR, 0x22); // start the NIC





			//TaskManager::addTask(&test, 50);
			//		
			
			
			/*write(_ISR, read(_ISR)) ;
			delay_ms(50);*/
			
			
		}
	
	
	
	//protected:
	
		static void hardReset(void) {
			RTL_CONTROL_PORT |= (1 << RTL_RESET_DRV);
			delay_ms(10);
			RTL_CONTROL_PORT &= ~(1 << RTL_RESET_DRV);
		}
	
		static void write(unsigned char address, unsigned char data){
			RTL_ADDR = (address << RTL_ADDR_SHIFT) & RTL_ADDR_MASK;
			RTL_DATA_OUT = data;
			RTL_DATA_DDR = 0xFF;			
			RTL_CONTROL_PORT &= ~(1 << RTL_IOWB);
			__asm__ __volatile__("nop");
			RTL_CONTROL_PORT |= (1 << RTL_IOWB);
			RTL_DATA_DDR = 0x00;
			RTL_DATA_OUT = 0xFF;
		}
		
		static unsigned char read(unsigned char address){
			unsigned char byte;
			RTL_ADDR = (address << RTL_ADDR_SHIFT) & RTL_ADDR_MASK;
			RTL_CONTROL_PORT &= ~(1 << RTL_IORB);
			__asm__ __volatile__("nop");
			byte = RTL_DATA_IN;
			RTL_CONTROL_PORT |= (1 << RTL_IORB);
			return byte;
		}


		
	
	
};

void test(void){
	RTL8019::read(10);
	//RTL_CONTROL_PORT = 0x00;
//	TaskManager::addTask(&test, 50);
}


#endif