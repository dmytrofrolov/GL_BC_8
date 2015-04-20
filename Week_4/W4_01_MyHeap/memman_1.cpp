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

static int heapSize;
const unsigned int SERVICE_PART = 4;
extern const unsigned int MB = 1024 * 1024 ;
extern const unsigned int KB = 1024 ;

int initHeap( size_t size ){
	
	ph_start = malloc( size );
	//cout << ph_start << endl;
	if( ph_start != NULL ){
		v_service = ph_start;
		v_heap = (char*)ph_start + size / SERVICE_PART + 1;
		cout << "v_service:\t" << v_service << endl;
		cout << "v_heap:\t\t" << v_heap << endl;
		
		heapSize = size;
		return SUCCESS;
	}
	else{
		return HEAP_INIT_ERR;
	}
}

int closeHeap( void ){
	free( ph_start );
}

void * myMalloc( size_t size ){
	// current adress of the service block of memory
	const char * curAdr = (const char *) v_service;

	// start block of memory programm allocates if it is free
	const char * currentBlock;

	const char * currentBlockStart;

	// counter for bits in byte
	unsigned int i = 0;

	// counter for current search
	unsigned int j;

	// bit mask to check if this byte is free
	unsigned short isFreeMask = 0;

	// size of memory to allocate
	size_t tempSize;

	// for each byte in service block
	for(; curAdr < v_heap ; ++curAdr){

		// remember were we start
		currentBlock = curAdr;
		currentBlockStart = curAdr;
		// for each 2 bits in byte
		for ( i = 0; i < 8; i += 2 )
		{
			// remember bit were we start
			j = i;

			// remember size what is needed
			tempSize = size;

			// check if next "size" bytes is free
			do{
				//cout << "j = " << j << endl;

				// is free bit mask - check 0, 2st, 4th, 6th bit in order to j
				isFreeMask = ( 1 << j );
				
				// if it is not free
				if( *curAdr & isFreeMask ){
						break;
				}

				// next var for next bit
				// if we are on 3rd 2-bit next will be zero
				if( j == 6 ){
					j = 0;
				}
				else{
					j += 2;
				}

				// if program has checked previous byte move to next one
				if(j==0){
					++curAdr;
				}
			}
			while(--tempSize);
			

			if(currentBlock >= (char*)v_heap - size)
				return NULL;

			// if the block found ( it started at "currentBlock" ) it marks the
			// part of the service that it will be allocated
			if(tempSize == 0){
				// remember the 2-bit to mark it
				int j = i;
				// remember size
				tempSize = size;
				// mark all bits that needed to remember that block is used
				do{
					// mask to mark the block and the next is used
					isFreeMask = ( 3 << j ); 
					// mark it
					*(char*)currentBlock |= isFreeMask;

					// if it was the last byte unmark that next if used
					if( tempSize == 1 ) {
						isFreeMask = (1 << (j + 1) );
						*(char*)currentBlock ^= isFreeMask;
					}

					// next var for next bit
					// if we are on 3rd 2-bit next will be zero
					if( j == 6 ){
						j = 0;
					}
					else{
						j += 2;
					}

					// if program has checked previous byte move to next one
					// only if it is not last size
					if( j == 0 ){
						++currentBlock;

					}
				}
				while(--tempSize);

				return (char*)v_heap + (currentBlockStart - (const char *) v_service) * SERVICE_PART + i;
			} // end if free block founded
			
		} // for each 2-bit in byte end

	} // for each byte in service memory

	// if nothing founded
	return NULL;
}

int myFree( void * address){
	if(address==NULL) return 1;

	// bit mask to check if this byte is free
	unsigned short isFreeMask = 0;
	unsigned int startFrom = ( (char*)address - (char*)v_heap );
	//cout << "address " << address << endl;
	//cout << "v_heap " << v_heap << endl;
	
	const char * startAdd = (char*)v_service;
	startAdd += startFrom / 4;
	int j = startFrom % 4;
	j *= 2;
	//cout << "startAdd " << (void*)startAdd << endl;
	//cout << "j " << j << endl;
	int next = 0;
	while( 1 ){
		for(; j<8; j += 2 ){
			// mask to mark the block
			isFreeMask = (1 << j) + (1 << (j+1)); 
			// mask to mark that next block is used too
			//isFreeMask += pow(2, j + 1);

			next = *(unsigned char*)startAdd & (1 << (j + 1) );
			*(char*)startAdd &= ~isFreeMask;
			//cout << std::dec << j << " " << next << endl;
			if( next == 0 )break;
			// mark it
			*(char*)startAdd &= ~isFreeMask;

		}
		j = 0;
		++startAdd;
		if(next == 0)break;
	}
	
	return 0;
}

void printHeap( void ){
	int i = 0;
	int heapLeft = heapSize;
	const unsigned char * startOfTheService = (unsigned char*)v_service;
	//unsigned char * addr;
	cout << "_printHeap_" << endl;
	while( (startOfTheService <= v_heap)  && (heapLeft >= 0) ){
		cout << (void*)((char*)startOfTheService) << " : ";
		for (int i = 0; (i < 16) && (heapLeft >= 0) ; ++i)
		{
			//addr = ++startOfTheService;
			cout << hex << setw(2) << setfill('0') << (int)*startOfTheService << " ";
			++startOfTheService;
			heapLeft-=4;
		}
		cout << endl;
		//startOfTheService+=16;
	}
	cout << endl;
}
