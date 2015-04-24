/**
*
* @file		main.cpp
* @brief	file for testing myheap
* 
* @author	Dmytro Frolov
* @date		Apr, 2015
*
*/

#include <iostream>
#include "memman.h"
#include <cstdlib>
#include <stdint.h>
// function for benchmark
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

using std::cout;
using std::endl;

int main(){



	int initResult;
	initResult = initHeap( 2 * KB );
	if( initResult != SUCCESS ){
		cout << "ERROR! Heap cannot be initialized!\nError code:" << initResult << endl;
		return HEAP_INIT_ERR;
	}

	printHeap();

	int *ptr1, *ptr2, *ptr3, *ptr4;
	// for version 2




	/*
	// for version 1

	ptr1 = (int*) myMalloc(128);
	cout << "ptr1 " << ptr1 << endl;
	printHeap();

	ptr1 = (int*) myMalloc(128);
	cout << "ptr1 " << ptr1 << endl;
	printHeap();
	*/
	ptr1 = (int*) myMalloc(34);
	cout << "allocate 34 bytes\nptr1 " << ptr1 << endl;
	printHeap();
	
	ptr2 = (int*) myMalloc(60);
	cout << "allocate 60 bytes\nptr2 "<< ptr2 << endl;
	printHeap();

	
	myFree(ptr1);
	cout << "free ptr1" << endl;
	printHeap();

	ptr1 = (int*) myMalloc(4);
	cout << "allocate 4 bytes\nptr1 " << ptr1 << endl;
	printHeap();
	
	ptr3 = (int*) myMalloc(2);
	cout << "allocate 2 bytes\nptr3 " << ptr3 << endl;
	printHeap();	

	myFree(ptr2);
	cout << "free ptr2 " << endl;
	printHeap();

	ptr2 = (int*) myMalloc(1004);
	cout <<  "allocate 1004 bytes\nptr2 " << ptr2 << endl;
	printHeap();

		/*	*/
	cout << endl;

	unsigned int i, totalCalc = 1000;
	unsigned long int sum = 0;
	
	for(i = 0; i < totalCalc; i++){
		uint64_t i1 = rdtsc();
		ptr4 = (int*) malloc( 1 * KB );
		//ptr4 = new int;
		//ptr4 = (int*)myMalloc( 1 * KB );
		uint64_t i2 = rdtsc();
		free(ptr4);
		//delete ptr4;
		//myFree(ptr4);
		sum += i2 - i1;
	}

	cout << std::dec << sum/totalCalc << endl;
//	*/	

	// stop work here
	closeHeap();
	return 0;
}