/*
*
*
*
*
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <cstring> // for memmove
#include "memman.h"

#define FREE 0
#define OCCUP 1

using std::cout;
using std::endl;
using std::hex;
using std::setw;
using std::setfill;

// starting of the heap in physical memory
static void * ph_start = NULL;

// virtual address
static void * v_service = NULL;
static void * v_heap = NULL;

unsigned int myHeapSize;
unsigned int blockNums;    // # of 14328 blocks

extern const unsigned int MB = 1024 * 1024 ;
extern const unsigned int KB = 1024 ;
extern const unsigned int BLOCK_SIZE = 14328;


struct FreeBlock{
	char *start;
	char *next; 
	size_t len:31;
	char state:1;	// 1 - free, 0 - occupied
};

int initHeap( size_t size ){
	cout << sizeof(FreeBlock) << endl;
	
	ph_start = malloc( size * BLOCK_SIZE );
	
	if(ph_start==NULL) return HEAP_INIT_ERR;

	v_service = ph_start;
	v_heap = (char*)ph_start + size / 3;

	((FreeBlock*)v_service)-> start = (char*)v_heap;
	((FreeBlock*)v_service)-> len 	= size;
	((FreeBlock*)v_service)-> state	= FREE;
	((FreeBlock*)v_service)-> next 	= NULL;
	myHeapSize = size * BLOCK_SIZE;
	
	blockNums = size;
	
	return SUCCESS;
}

int closeHeap( void ){
	free( ph_start );
}

void * myMalloc( size_t size ){
	FreeBlock * servStart = ((FreeBlock*)v_service);
 	cout <<  servStart->len << endl;
	do{
		while( servStart->state != FREE ){
			++servStart;
		}
		if( servStart->len >= size ){
			if( (servStart+1)->start != NULL ){
				memmove(servStart+2, servStart+1, (char*)v_heap-(char*)servStart+1);
			}
			++servStart;
			servStart->start = (servStart-1)->start + size;
			servStart->len = (servStart-1)->len - size;
			servStart->state = FREE;
			--servStart;
			servStart->state = OCCUP;
			servStart->len = size;
			return servStart->start;
		}
		++servStart;
	}while( 1 );
	return NULL;
}

int myFree( void * address){
	FreeBlock * servStart = ((FreeBlock*)v_service);
	cout << (void*)servStart->start << endl;
	while( (servStart->start != address) && (servStart->start < (char*)v_heap+myHeapSize) ){
		++servStart;
	}

	if( servStart->start >= (char*)v_heap+myHeapSize )
		return 1;
	
	servStart->state = FREE;
	return 0;
}

void printHeap( void ){

	FreeBlock * servStart = ((FreeBlock*)v_service);

	cout << "_printHeap_" << endl << setfill('0');
	while( (servStart < v_heap) ){
		if( servStart->start !=NULL )
		cout << (void*)servStart->start << " " << servStart->len << "\t" << (int)servStart->state << endl;
		++servStart;
	}
	cout << endl;
}
