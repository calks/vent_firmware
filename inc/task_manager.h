#ifndef TASK_MANAGER_CLASS
#define TASK_MANAGER_CLASS

#include <stdlib.h>

struct TaskManagerTask {	
	void(*func)(void);
	TaskManagerTask *next_task;
	int countdown;	
};

class TaskManager {
	public:
		static void init() {
			OCR0 = 143; // 100Hz			
			TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS00) | (1<<CS02); // CTC mode, 1024 prescaler
			#if defined (__AVR_ATmega128A__)
			TCCR0 |= (1<<CS01);
			#endif
			TIMSK |= (1<<OCIE0); // compare interrupt
		}
		
		static void addTask(void(*fn)(void), int countdown){
			TaskManagerTask *task;
			task = new TaskManagerTask();
			task->func = fn;
			task->next_task = NULL;
			task->countdown = countdown;
			
			if (!first_task) {
				first_task = task;
				first_task_countdown = countdown;
			}
			else {
				updateCountdown();
				if(countdown < first_task->countdown) {
					task->next_task = first_task;
					first_task = task;
					first_task_countdown = countdown;
				}
				else {
					TaskManagerTask *insert_after = first_task;
					while (insert_after->next_task && (insert_after->next_task->countdown<countdown)) {
						insert_after = insert_after->next_task;
					}					
					task->next_task = insert_after->next_task;
					insert_after->next_task = task;
				}
			}
		}
		
		static int getQueueSize(void){			
			if (!first_task) return 0;
			int ret = 0;
			
			TaskManagerTask *ptr = first_task;
			while(ptr) {				
				ptr = ptr->next_task;
				ret++;
			}
			
			return ret;
			
		}
		
		static void tick(void){
			//cli();
			sei();
			system_time++;
			
			if (first_task) {				
				first_task_countdown--;
				if (first_task_countdown <= 0) {
					//printf("\r\ntick:");printList();
					updateCountdown();
					TaskManagerTask *old_first_task;
					while(first_task && first_task->countdown<=0) {
						void(*func)(void) = first_task->func;
						old_first_task = first_task;
						first_task = first_task->next_task;
						first_task_countdown = first_task->countdown;
						delete(old_first_task);
						(*func)();
					}
				}
			}
			//sei();
		}
		
		static uint64_t getSystemTime(void) {
			return system_time;
		}
		
		
	protected:		
		static uint64_t system_time;
		static TaskManagerTask *first_task;
		static int first_task_countdown;		
		
		static void printList() {			
			TaskManagerTask *check = first_task;
			while (check) {
				printf(" %i", check->countdown);
				check = check->next_task;
			}
			//printf("\r\n");			
		}
		
		
		static void updateCountdown() {
			if (!first_task) return;
			if (first_task_countdown == first_task->countdown) return;
			unsigned int diff = first_task->countdown - first_task_countdown;
			
			TaskManagerTask *ptr = first_task;
			while(ptr) {
				ptr->countdown -= diff;
				ptr = ptr->next_task;
			}
			first_task_countdown = first_task->countdown;
		}
		
	
};

TaskManagerTask *TaskManager::first_task = NULL;
int TaskManager::first_task_countdown = 0;
uint64_t TaskManager::system_time = 0;


ISR(TIMER0_COMP_vect) {
	TaskManager::tick();
}




#endif