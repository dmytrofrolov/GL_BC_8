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
#include "memman.h"

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
const unsigned int COLS = 8;

unsigned int colStarts[] = {1, 18, 51, 116, 245, 502, 1015, 2040, 6136};


int initHeap( size_t size ){
	if( size < BLOCK_SIZE ) size = 1;
	else size = size / BLOCK_SIZE + ( size%BLOCK_SIZE == 0 ? 0 : 1);
	
	ph_start = malloc( size * BLOCK_SIZE );
	
	if(ph_start==NULL) return HEAP_INIT_ERR;

	myHeapSize = size * BLOCK_SIZE;
	
	blockNums = size;
	
	return SUCCESS;
}

int closeHeap( void ){
	free( ph_start );
}

void * myMalloc( size_t size ){
	
	return NULL;
}

int myFree( void * address){
	
	return 0;
}

void printHeap( void ){

}
