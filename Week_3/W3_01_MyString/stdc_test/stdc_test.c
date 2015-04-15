#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void main(void){
	char p[50]  	 = "HwithmmaHello2332";
	char string2[50] = "mamma miall what is that";
	
	//char *str = "This is a sample string0";
	char *key = "aeiou0";

	//printf("%d\n", strspn(p, string2));

	unsigned int i, totalCalc = 10000000;
	unsigned long int sum = 0;
	for(i = 0; i < totalCalc; ++i){
		uint64_t i1 = rdtsc();
		//strpbrk (str, key);
		uint64_t i2 = rdtsc();
		sum += i2 - i1;
	}
	printf("%d\n", sum/totalCalc);

	printf("\n");

}