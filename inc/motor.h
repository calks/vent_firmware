#ifndef MOTOR_CLASS
#define MOTOR_CLASS

#define FW_ANODE PD7
#define FW_CATHODE PD6
#define M_ANODE PD5
#define M_CATHODE PD4

#define MOTOR_OFF_FW_OFF 0x0F
#define MOTOR_ON_FW_OFF 0x2F
#define MOTOR_OFF_FW_ON 0x8F

#define MOTOR_DEAD_TIME_DELAY __asm__ __volatile__("nop;nop;nop;nop;nop;");


class Motor {
	public:
		static void init(unsigned char power=0) { 
			
			TCCR1A = ((1<<WGM10) | (1<<WGM11));
			TCCR1B = (1<<WGM12);
			
			PORTD = MOTOR_OFF_FW_OFF;
			DDRD = (1<<M_ANODE) | (1<<M_CATHODE) | (1<<FW_ANODE) | (1<<FW_CATHODE);			
			setPower(power);
		}
	
		static void setPower(unsigned char val) {
			
			cli();
			power = val;			
			
			unsigned int compare_value = ((unsigned int)power << 2);
			
			if (compare_value == 0)	{
				PORTD = MOTOR_OFF_FW_OFF;
				MOTOR_DEAD_TIME_DELAY;
				PORTD = MOTOR_OFF_FW_ON;
				stop();
			}
			else if (compare_value > 1000) {
				PORTD = MOTOR_OFF_FW_OFF;
				MOTOR_DEAD_TIME_DELAY;
				PORTD = MOTOR_ON_FW_OFF;
				stop();
			}	
			else {				
				OCR1AH = compare_value >> 8;
				OCR1AL = compare_value & 0xFF;				
				start();				
			}
			sei();
		}
		
		static unsigned char getPower() {
			return power;
		}

	protected: 
		static unsigned char power;

		static void start() {
			TIMSK |= ((1<<OCIE1A) | (1<<TOIE1));
			TCCR1B |= (1<<CS10);
		}
		
		static void stop() {
			TIMSK &= (~(1<<OCIE1A) & ~(1<<TOIE1));
			TCCR1B &= ~(1<<CS10);
			TCNT1H = 0x00;
			TCNT1L = 0x00;			
		}
	
};

unsigned char Motor::power = 0;

ISR(TIMER1_OVF_vect) {
	PORTD = MOTOR_OFF_FW_OFF;
	MOTOR_DEAD_TIME_DELAY;
	PORTD = MOTOR_ON_FW_OFF;	
}

ISR(TIMER1_COMPA_vect) {
	PORTD = MOTOR_OFF_FW_OFF;
	MOTOR_DEAD_TIME_DELAY;
	PORTD = MOTOR_OFF_FW_ON;	
}


#endif