
#define F_CPU 14745600
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../inc/console.h"
#include "../inc/temp_sensors.h"
#include "../inc/task_manager.h"

void * operator new(size_t size) {
	return malloc(size);
}

void operator delete(void * ptr) {
	free(ptr);
}
