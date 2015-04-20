#include <stdio.h>
#include <stdint.h>


// function for benchmark
uint64_t rdtsc();


void main(void){

	int a =0, b = 0;
	++a, ++b;

	char * p  		= (char*) allocateString ("HwithmmaHello23320");
	char * string2 	= (char*) allocateString ("mamma miall what is that0");
	
	unsigned int i, totalCalc = 10000000;
	unsigned long int sum = 0;
	
	for(i = 0; i < totalCalc; i++){
		uint64_t i1 = rdtsc();
		//strpbrk (str, key);
		uint64_t i2 = rdtsc();
		sum += i2 - i1;
	}
	
	printf("%d\n", sum/totalCalc);

	printf("%x\n", 123);

}

// function for benchmark
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}