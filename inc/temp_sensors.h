#ifndef TEMP_SENSORS
#define TEMP_SENSORS

//#define PORTA_ADDRESS 0x

#define sensor_pin PINB;


#define DS18B20_SKIP_ROM 0xCC
#define DS18B20_START_CONVERSION 0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

typedef volatile uint8_t *sfr_pointer;


#include "task_manager.h"
#include "samples_group.h"

class ds18B20 {
	public:	
		unsigned char id;
		int16_t current_value;
		int16_t derive;
		ds18B20 * next;
		uint64_t last_update_timestamp;
		
		volatile uint8_t * port_address;
		volatile uint8_t * ddr_address;
		volatile uint8_t * pin_address;
		char pin_number;		
		samplesGroup * derive_samples;

	
		ds18B20(unsigned char id, volatile uint8_t * port_address, volatile uint8_t * ddr_address, volatile uint8_t * pin_address, unsigned char pin_number) {
			this->id = id;
			this->port_address = port_address;
			this->ddr_address = ddr_address;
			this->pin_address = pin_address;
			this->pin_number = pin_number;

			this->current_value = 0;
			this->derive = 0;
			this->last_update_timestamp = 0;
			
			this->derive_samples = new samplesGroup(4);			
			next = NULL;		
			
		}
		
		
		/*void reset(void) {
			DDRD |= (1<<pin_number);
			*port_address &= ~(1<<pin_number);
			_delay_us(480);
			DDRD &= ~(1<<pin_number);
			*port_address |= (1<<pin_number);
			_delay_us(480);

		}
		
		bool dataReady(void) {
			return readBit()==1;
		}
		
		
		void writeBit(unsigned char bit){
			//Initiate write time slot
			DDRD |= (1<<pin_number);
			*port_address &= ~(1<<pin_number);
			_delay_us(1);
			
			if(bit) {
				*port_address |= (1<<pin_number);
			}
    
			_delay_us(59); //delay to meet write time slot req.
			*port_address |= (1<<pin_number);
			DDRD &= ~(1<<pin_number);
			_delay_us(1);
		}
		
		
		uint8_t readBit(void){
			uint8_t bit_value;
			
			//Initiate write time slot
			DDRD |= (1<<pin_number);
			*port_address &= ~(1<<pin_number);
			_delay_us(1);
 
			DDRD &= ~(1<<pin_number);
			*port_address |= (1<<pin_number);
			_delay_us(15); //wait for DS18B20 output to stablize
			
			bit_value = (PIND & (1<<pin_number)) == 0 ? 0 : 1;
			//wait to meet minimum read time slot of 60us plus 1us recovery time
			_delay_us(59);
			
			return bit_value;
		}
		
		
		void writeByte(unsigned char byte){
			uint8_t bit_pos;
			
			for(bit_pos = 0; bit_pos != 8; ++bit_pos) {				
				writeBit(byte & (1<<bit_pos));
			}			
		}
		
		void startConversion(void) {
			reset();
			writeByte(DS18B20_SKIP_ROM);
			writeByte(DS18B20_START_CONVERSION);			
		}
		
		int16_t readTemp(void){
			int16_t temp = 0;
			uint16_t bit_value;
			uint8_t bit_pos;
			
			reset();
			writeByte(DS18B20_SKIP_ROM);
			writeByte(DS18B20_READ_SCRATCHPAD);
			
			for(bit_pos = 0; bit_pos != 16; ++bit_pos) {
				 bit_value = (uint16_t)readBit();
				 temp |= bit_value << bit_pos;
			 }			

			reset();
			
			return temp;

		}*/
};

class tempSensors {
	
	public:
		static void add(unsigned char sensor_id, volatile uint8_t* port_address, volatile uint8_t * ddr_address, volatile uint8_t * pin_address, unsigned char pin_number) {
			ds18B20 * new_sensor = new ds18B20(sensor_id, port_address, ddr_address, pin_address, pin_number);
			if (!first) {				
				first = new_sensor;	
				first->next = first;
				TaskManager::addTask(&tempSensors::readSensors, 10);
				initTimeslotsGenerator();
			}
			else {
				ds18B20 * append_to = first;
				while (append_to->next != first) {
					append_to = append_to->next;
				}
				new_sensor->next = first;
				append_to->next = new_sensor;				
			}
			sensors_count++;
		}
		
		static int getTemperature(unsigned char sensor_id, char get_derive = 0) {
			if (!first) return 0;
			ds18B20 * s = first;
			do {
				if (s->id == sensor_id) return get_derive ? s->derive : s->current_value;
				s = s->next;
			}
			while(s != first);			
			return 0;
		}

			
		static void readSensors() {			
			if (!busy) readScratchpad();
			TaskManager::addTask(&tempSensors::readSensors, 10);
			return;
		}
		
		
		static void timeSlotISR(void) {	
			if (!period_counter) return;
			period_counter--;
			if (period_counter==0 && next_action) {
				void(*act)(void) = next_action;
				next_action = NULL;
				(*act)();				
			}
			
		}
	
	
	protected:	
		static ds18B20 * first;
		static unsigned char sensors_count;
		static unsigned char period_counter;		
		static void(*next_action)(void);
		static unsigned char pin_number;
		
				
		static char write_buf[2];
		static unsigned char write_buf_byte;
		static unsigned char write_buf_bit;
				
		static char read_buf[9];
		static unsigned char read_buf_byte;
		static unsigned char read_buf_bit;
		static unsigned char read_buf_size;
		
		static unsigned char CRC;
		static unsigned char busy;
		
		
		static void startConversion(void) {
			write_buf_bit = 0;
			write_buf_byte = 0;
			write_buf[0] = DS18B20_SKIP_ROM;
			write_buf[1] = DS18B20_START_CONVERSION;			
			resetLow();			
		}
		
		
		static void readScratchpad(void) {
			write_buf_bit = 0;
			write_buf_byte = 0;
			write_buf[0] = DS18B20_SKIP_ROM;
			write_buf[1] = DS18B20_READ_SCRATCHPAD;
			resetLow();			
		}
		
		
		static void resetLow(void) {
			*(first->ddr_address) |= (1<<pin_number);
			*(first->port_address) &= ~(1<<pin_number);
			period_counter = 33;
			next_action = &tempSensors::resetHigh;
		}
		
		static void resetHigh(void) {
			*(first->ddr_address) &= ~(1<<pin_number);
			*(first->port_address) |= (1<<pin_number);
			period_counter = 32;
			CRC = 0;
			for (int i=0; i<9; i++) read_buf[i] = 0;
			next_action = &tempSensors::slotStart;			
		}
		
		
		static void slotStart(void) {			
			//initiate slot
			*(first->ddr_address) |= (1<<pin_number);
			
			*(first->port_address) |= (1<<pin_number);
			_delay_us(1);
			*(first->port_address) &= ~(1<<pin_number);
			_delay_us(1);
			
			//writing
			if (write_buf[0]) {
				unsigned char bit_value = write_buf[write_buf_byte] & (1 << write_buf_bit);
				if (bit_value) {
					*(first->port_address) |= (1<<pin_number);	
				}
				write_buf_bit++;
				if (write_buf_bit == 8) {
					write_buf_bit = 0;
					write_buf_byte++;
					if (write_buf_byte == 2) {
						write_buf[0] = 0; // stop writing
						if (write_buf[1] == DS18B20_START_CONVERSION) {
							toNextSensor();
						}
					}
				}				
				
				period_counter = 4; // 60us
				next_action = &tempSensors::slotStart;
			}
			//reading
			else {
				*(first->ddr_address) &= ~(1<<pin_number);
				*(first->port_address) |= (1<<pin_number);
				period_counter = 1; // 15us
				next_action = &tempSensors::slotSample;
			}

		}
		
		static void slotSample(void) {
			char bit_value = (*(first->pin_address) & (1<<pin_number)) == 0 ? 0 : 1;
			
			if (bit_value && write_buf[1] == DS18B20_READ_SCRATCHPAD) {
				read_buf[read_buf_byte] |= (1 << read_buf_bit);
			}
			
			if (write_buf[1] != DS18B20_READ_SCRATCHPAD) {
				
				busy = bit_value ? 0 : 1;
				if (!busy) return;
				period_counter = 3; // 45us
				next_action = &tempSensors::slotStart;
				return;
			}
			
			
			unsigned char fb;  
			// exclusive or least sig bit of current shift reg with the data bit 
			fb = (CRC & 0x01) ^ bit_value;
			
			// shift one place to the right 
			CRC = CRC >> 1;

			if (fb==1) {
				CRC = CRC ^ 0x8C; /* CRC ^ binary 1000 1100 */
			}			
			
			read_buf_bit++;
			if (read_buf_bit == 8) {
				read_buf_bit = 0;
				read_buf_byte++;
				if (read_buf_byte == 9) {
					read_buf_byte = 0;
					period_counter = 3; // 45us
					int data = read_buf[0] | (read_buf[1] << 8);					
					updateCurrentSensor(data);
					startConversion();
					return;
				}
			}
			period_counter = 3; // 45us
			next_action = &tempSensors::slotStart;
		}
		
		
		static void initTimeslotsGenerator() {
			TCCR2 |= (1<<FOC2) | (1 << WGM21) | (1 << CS20); // CTC mode, no prescaling
			OCR2 = 220; // ~15us period
			TIMSK |= (1 << OCIE2);
			next_action = NULL;			
		}
		
		static void updateCurrentSensor(int16_t temp) {
			uint64_t timestamp = TaskManager::getSystemTime();

			int64_t dt = 6000 / (timestamp - first->last_update_timestamp);
			int derive = (temp - first->current_value) * dt;
				
			first->last_update_timestamp = timestamp;
			first->derive_samples->add(derive);
			first->derive = first->derive_samples->getAvg();
			first->current_value = temp;
		}
		
		static void toNextSensor(void) {
			first = first->next;
			busy = 1;
		}
	
};


ds18B20 * tempSensors::first = NULL;
unsigned char tempSensors::sensors_count = 0;
unsigned char tempSensors::period_counter = 0;
void(*tempSensors::next_action)(void) = NULL;
unsigned char tempSensors::pin_number = PORTD5;

char tempSensors::write_buf[2] = {0,0};
unsigned char tempSensors::write_buf_byte = 0;
unsigned char tempSensors::write_buf_bit = 0;
		
char tempSensors::read_buf[9] = {0,0,0,0,0,0,0,0,0};
unsigned char tempSensors::read_buf_byte = 0;
unsigned char tempSensors::read_buf_bit = 0;
unsigned char tempSensors::read_buf_size = 0;

unsigned char tempSensors::CRC = 0;
unsigned char tempSensors::busy = 0;



ISR(TIMER2_COMP_vect) {
	tempSensors::timeSlotISR();
}


#endif