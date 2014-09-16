#ifndef FIFO_BUFFER_CLASS
#define FIFO_BUFFER_CLASS
 
#include <stdlib.h>

class FifoBuffer {
	public:
		FifoBuffer(unsigned char size) {
			_size = size;
			_data = (char*)malloc(size);
			_head = 0;
			_tail = 0;
		}
		
		~FifoBuffer() {
			delete(_data);
		}
		
		bool isFull(){
			return (_head - _tail) == _size;
		}
		
		bool isEmpty(){
			return _head==_tail;
		}
		
		void push(char byte) {
			_data[_head & (_size-1)] = byte;
			_head++;
		}
		
		char pop() {
			char byte = _data[_tail & (_size-1)];
			_tail++;
			if (isEmpty()) {
				_tail = 0;
				_head = 0;
			}
			return byte;
		}	
		
		unsigned char count() {
			return _head-_tail;			
		}
	
	protected:	
		char * _data;
		unsigned char _size;
		unsigned char _tail;
		unsigned char _head;
	
};

#endif //FIFO__H
