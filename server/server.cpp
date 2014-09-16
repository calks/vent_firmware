/*
 * server.cpp
 *
 * Created: 31.08.2014 0:18:28
 *  Author: Марина
 */ 

#include "../inc/m16compat.h"
#include "../inc/common.h"


/*#include "../inc/motor.h"
#include "../inc/adc.h"*/
#include "../inc/lcd1602.h"
//#include "../inc/rtl8019.h"


#define ROOM_TEMP 0x01




void printState(void);
void printState(void) {
	
	LCD1602::clear();
	fprintf(LCD1602::getStream(), "%lu Q:%i", TaskManager::getSystemTime(), TaskManager::getQueueSize());	

	LCD1602::moveTo(2,1);
	int temp = tempSensors::getTemperature(ROOM_TEMP);
	int temp_units = temp/16;
	int temp_decimals = (temp*10/16)%10;
	int derive = tempSensors::getTemperature(ROOM_TEMP, 1);
	int derive_units = derive/16;
	int derive_decimals = abs(derive*10/16)%10;

	fprintf(LCD1602::getStream(), "%i.%i (%i.%i)", temp_units, temp_decimals, derive_units, derive_decimals);
	
	
	TaskManager::addTask(&printState, 10);
	
	
}


void processCommands(void);
void processCommands(void) {
	
	
	if (Console::lineEntered()) {
		char *line;
		char *command;
		char *param_str;
		
		line = Console::getLine();
		command = line;
		param_str = strstr(line, " ");
		
		if (param_str) {
			*param_str = 0;
			param_str++;
		}
		else {
			char command_length = strlen(command);
			if (command_length) command[command_length-1] = 0;
		}
		
		//printf("command: %s\r\nparam: %s\r\n", command, param_str);
		
		/*if (strcmp(command, "read") == 0) {
			unsigned char addr;
			if (sscanf(param_str, "%i", &addr)) {
				unsigned char data = RTL8019::read(addr);
				printf("%x = %x\r\n", addr, data);
			}
			else {
				printf("no address\r\n");
			}
			
		}
		
		else if (strcmp(command, "write") == 0) {
			unsigned char addr;
			unsigned char data;
			if (sscanf(param_str, "%i", &data)) {
				printf("%x\r\n", data);
				RTL8019::write(CR, data);
				delay_ms(10);
				unsigned char rdata = RTL8019::read(CR);
				printf("%x = %x (%x)\r\n", CR, rdata, data);
			}
			else {
				printf("no address or data\r\n");
			}
			
		}*/
		
		
		if (strcmp(command, "temp") == 0) {
			int temp = tempSensors::getTemperature(ROOM_TEMP);
			int derive = tempSensors::getTemperature(ROOM_TEMP, 1);
			printf("%i (%i)\r\n", temp, derive);
		}

		else {
			printf("unknown command <%s>\r\n", command);
		}

	}
	
	TaskManager::addTask(&processCommands, 10);
}


int main(void)
{
	
	Console::init(14745600, 9600, 128);
	TaskManager::init();
	
	/*DDRG |= (1 << PORTG2);
	PORTG |= (1 << PORTG2);*/
	
	LCD1602::init();
	
	stdout = Console::getStream();

	tempSensors::add(ROOM_TEMP, &PORTD, &DDRD, &PIND, PORTD5);

	TaskManager::addTask(&processCommands, 10);	
	printState();

	while(1) {
	}
}

