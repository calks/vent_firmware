#ifndef SAMPLES_GROUP_CLASS
#define SAMPLES_GROUP_CLASS

class samplesGroup {
	
	public:
		samplesGroup (unsigned char size) {
			this->size = size;
			this->samples = (int*)calloc(size, sizeof(int));
			this->pos = 0;
		}
		
		void add(int sample_val) {
			*(this->samples+this->pos) = sample_val;
			this->pos++;
			if (this->pos == this->size) this->pos=0;	
			
		}
		
		int getAvg(void) {
			int sum = * this->samples;
			for(unsigned char i=1; i<this->size; i++) {
				sum += *(this->samples+i);
			}
			int avg = sum / this->size;
			
			return avg;
		}
		
	protected: 
		int * samples;
		unsigned char size;
		unsigned char pos;
};

#endif