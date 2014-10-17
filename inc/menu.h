#ifndef MENU_CLASS
#define MENU_CLASS

#define ENCORER_PIN1 PORTE5
#define ENCORER_PIN2 PORTE6
#define BUTTON_PIN PORTE7
#define PIN_MASK ((1<<ENCORER_PIN1) | (1<<ENCORER_PIN2) | (1<<BUTTON_PIN))

#define MENU_ACTION_EMPTY 0x00
#define MENU_ACTION_INCREMENT 0x01
#define MENU_ACTION_DECREMENT 0x02
#define MENU_ACTION_SHORT_CLICK 0x03
#define MENU_ACTION_LONG_CLICK 0x04

//#include "menu_screen.h"

class menuScreen {
	public:
	menuScreen * left_sibling;
	menuScreen * right_sibling;
	
	virtual void render(void);
	
	virtual void onUserAction(unsigned char action);
	
};

class menuScreenTemperature:public menuScreen{
	void render(void);
};


void printCounter(void);

class menu {
	
	public: 
		static unsigned char prev_state;
		static unsigned char waiting_edge;
		static uint64_t btn_press_time;
		static char counter;
		static menuScreen *active_screen;
		static unsigned char pending_action;
	
		static void init(void){
			PORTE |= PIN_MASK;
			EICRB |= (1<<ISC70); // change
			EICRB |= (1<<ISC61); // falling edge
			EICRB |= (1<<ISC50) | (1<<ISC51); // rising edge

			EIMSK |= ((1<<INT5) | (1<<INT6) | (1<<INT7));
			//printCounter();
			
			
			
			menuScreenTemperature * temp = new menuScreenTemperature();
			addScreen(temp);

			menuScreen * sys_time = new menuScreen();
			addScreen(sys_time);
			
			refresh();
		}
		
		static void setActiveScreen(menuScreen * screen) {
			active_screen = screen;
		}
		
		
		static void addScreen(menuScreen * screen) {
			if (!active_screen) {
				screen->left_sibling = screen;
				screen->right_sibling = screen;
				active_screen = screen;
			}
			else {
				screen->right_sibling = active_screen->right_sibling;
				active_screen->right_sibling->left_sibling = screen;
				screen->left_sibling = active_screen;
				active_screen->right_sibling = screen;
			}			
		}

	
	
		static void onButtonPinChange(void){
			unsigned char new_state = PINE & PIN_MASK;
									
			if (!(new_state^prev_state)) return;
			
			char button_pressed = ~(new_state & (1<<BUTTON_PIN)) & (prev_state & (1<<BUTTON_PIN));
			char button_released = (new_state & (1<<BUTTON_PIN)) & ~(prev_state & (1<<BUTTON_PIN));
			
			LCD1602::moveTo(1,1);
			
			if (button_pressed) {
				btn_press_time = TaskManager::getSystemTime();
				fprintf(LCD1602::getStream(), "press         ");
			}
			if (button_released) {
				uint64_t hold_time = TaskManager::getSystemTime() - btn_press_time;
				fprintf(LCD1602::getStream(), "release %i", hold_time);
			}
			
			prev_state = new_state;
		}
		
		static void refresh(void){
			if (active_screen) {
				if (pending_action != MENU_ACTION_EMPTY) {
					active_screen->onUserAction(pending_action);
					pending_action = MENU_ACTION_EMPTY;
				}				
				active_screen->render();
			}
			TaskManager::addTask(&menu::refresh, 10);
		}	
	
};

unsigned char menu::prev_state = PINE & PIN_MASK;
unsigned char menu::waiting_edge = 0;
uint64_t menu::btn_press_time = 0;
menuScreen *menu::active_screen = NULL;
char menu::counter = 0;
unsigned char menu::pending_action = MENU_ACTION_EMPTY;

ISR(INT5_vect) {
	if (menu::waiting_edge) {
		menu::waiting_edge = 0;
		if (PINE & (1 << ENCORER_PIN2)) {
			menu::pending_action = MENU_ACTION_INCREMENT;
		}
		else {
			menu::pending_action = MENU_ACTION_DECREMENT;
		}
	}	
}

ISR(INT6_vect) {
	menu::waiting_edge = 1;
}

ISR(INT7_vect) {
	menu::onButtonPinChange();
}


void printCounter(void) {
	LCD1602::moveTo(2,1);
	fprintf(LCD1602::getStream(), "%i      ", menu::counter);
	TaskManager::addTask(&printCounter, 10);
}









void menuScreen::render(void) {
	LCD1602::moveTo(1,1);
	fprintf(LCD1602::getStream(), "Системное время ");
	LCD1602::moveTo(2,1);
	fprintf(LCD1602::getStream(), "%li             ", TaskManager::getSystemTime());
	
	
	/*LCD1602::moveTo(1,1);
	fprintf(LCD1602::getStream(), "time: %i", TaskManager::getSystemTime());*/
	
}


void menuScreenTemperature::render(void) {
	
	int temp = tempSensors::getTemperature(0x01);
	int temp_units = temp/16;
	int temp_decimals = (temp*10/16)%10;
	/*int derive = tempSensors::getTemperature(0x01, 1);
	int derive_units = derive/16;
	int derive_decimals = abs(derive*10/16)%10;*/
	
	//char degree_symbol = 248;
	
	LCD1602::moveTo(1,1);
	fprintf(LCD1602::getStream(), "Температура     ");	
	LCD1602::moveTo(2,1);
	fprintf(LCD1602::getStream(), "%i.%i%             ", temp_units, temp_decimals);
	
}


void menuScreen::onUserAction(unsigned char action) {
	switch(action) {
		case MENU_ACTION_INCREMENT:
			menu::setActiveScreen(right_sibling);
			break;
		case MENU_ACTION_DECREMENT:
			menu::setActiveScreen(left_sibling);
			break;

	}
	
}




#endif