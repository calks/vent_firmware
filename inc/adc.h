#ifndef ADC_CLASS
#define ADC_CLASS

#include "task_manager.h"

#define ADC_REFERENCE_INTERNAL 1
#define ADC_REFERENCE_VCC 2

struct ADCClannel {	
	unsigned char number;
	unsigned char reference_voltage;
	unsigned int reading;
	ADCClannel * next_channel;
};

class ADCModule {
	public: 
		static void addChannel(unsigned char channel_number, unsigned char reference_voltage){
			if (channel_number > 8) return;
			ADCClannel * channel = new ADCClannel;
			channel->number = channel_number;
			channel->reference_voltage = reference_voltage;
			channel->reading = 0;

			if (!current_channel) {				
				channel->next_channel = channel;
				current_channel = channel;
				setMultiplexer(channel_number, reference_voltage);
				ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
				TaskManager::addTask(&updateReading, 10);
			}
			else {
				channel->next_channel = current_channel->next_channel;
				current_channel->next_channel = channel;
			}
		}
		
		static void updateReading() {
			if (!current_channel) return;
			current_channel->reading = ADCL;
			current_channel->reading |= (ADCH << 8);
			//printf("%i     \r", current_channel->reading);
			current_channel = current_channel->next_channel;
			setMultiplexer(current_channel->number, current_channel->reference_voltage);			
			TaskManager::addTask(&updateReading, 10);
		}
		
		static unsigned int getValue(unsigned char channel_number) {
			if (!current_channel) return 0;
			ADCClannel * ch = current_channel->next_channel;
			while (ch->number != channel_number && ch != current_channel) {
				ch = ch->next_channel;
			}
			return ch->number == channel_number ? ch->reading : 0;
		}
	
	protected:
		static ADCClannel * current_channel;
		
		static void setMultiplexer(unsigned char channel_number, unsigned char reference_voltage) {
			ADMUX = (reference_voltage==ADC_REFERENCE_INTERNAL ? 0xC0 : 0x40) | channel_number;
			//printf("ADC update ADMUX=%x\r\n", ADMUX);
		}
	
};

ADCClannel * ADCModule::current_channel = NULL;


#endif