#ifndef MOTOR_CLASS
#define MOTOR_CLASS

#define FW_ANODE PD7
#define FW_CATHODE PD6
#define M_ANODE PD5
#define M_CATHODE PD4

#define MOTOR_OFF_FW_OFF 0x0F
#define MOTOR_ON_FW_OFF 0x2F
#define MOTOR_OFF_FW_ON 0x8F

class Motor {
	public:
		static void init(unsigned char power=0) {
			TCCR2 = (1<<FOC2) | (1<<WGM20) | (1<<CS20);
			PORTD = MOTOR_OFF_FW_OFF;
			DDRD = (1<<M_ANODE) | (1<<M_CATHODE) | (1<<FW_ANODE) | (1<<FW_CATHODE);			
			setPower(power);
		}
	
		static void setPower(unsigned char val) {
			power = val;
			OCR2 = val;
			if (power == 0)	stop();
			else start();
		}
		
		static unsigned char getPower() {
			return power;
		}

	protected: 
		static unsigned char power;

		static void start() {
			TIMSK |= (1<<OCIE2) | (1<<TOIE2);
		}
		
		static void stop() {
			TIMSK &= (~(1<<OCIE2) & ~(1<<TOIE2));
			PORTD = MOTOR_OFF_FW_OFF;
		}
	
};

unsigned char Motor::power = 0;

ISR(TIMER2_OVF_vect) {
	PORTD = MOTOR_ON_FW_OFF;	
}

ISR(TIMER2_COMP_vect) {
	PORTD = MOTOR_OFF_FW_ON;
}


#endif