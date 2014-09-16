/*
 * power_module.cpp
 *
 * Created: 25.08.2014 15:45:38
 *  Author: Марина
 */ 

#include "../inc/common.h"
#include "../inc/motor.h"
#include "../inc/adc.h"


#define LED PC2

void LEDoff(void);
char led_state = 0;

void LEDon(void) {
	PORTC &= ~(1<<LED);
	led_state = 1;	
	TaskManager::addTask(&LEDoff, 50);	
}

void LEDoff(void) {	
	PORTC |= (1<<LED);
	led_state = 0;
	TaskManager::addTask(&LEDon, 50);	
}

//PA1 (ADC1)

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
		
		if (strcmp(command, "led") == 0) {
			int state;
			if (sscanf(param_str, "%d", &state)) {
				if (state == 1) LEDon();
				else if (state == 0) LEDoff();
				else printf("usage: led [1|0]\r\n");
			}
			else {
				printf("led state: %i\r\n", led_state);
			}
			
		}
		else if (strcmp(command, "motor") == 0) {
			int power;
			if (sscanf(param_str, "%d", &power)) {
				if (power >= 0 && power <= 255) Motor::setPower(power);				
				else printf("usage: motor [power 0-255]\r\n");
			}
			else {
				printf("motor power: %i\r\n", Motor::getPower());
			}			
		}
		else if (strcmp(command, "adc") == 0) {			
			printf("ADC value: %i\r\n", ADCModule::getValue(0));
		}


		else {
			printf("unknown command <%s>\r\n", command);			
		}	

	}
	
	TaskManager::addTask(&processCommands, 10);
}


int main(void)
{
	
	Console::init(14745600, 9600);
	TaskManager::init();
	Motor::init(125);
	ADCModule::addChannel(0, ADC_REFERENCE_VCC);
	
	stdout = Console::getStream();
	
	PORTC = 0xFF;
	DDRC = 0xFF;

	TaskManager::addTask(&processCommands, 10);
	TaskManager::addTask(&LEDon, 10);

	while(1) {}
}

