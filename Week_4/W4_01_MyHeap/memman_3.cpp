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
extern const unsigned int SERVICE_PART = 4;
extern const unsigned int MB = 1024 * 1024 ;
extern const unsigned int KB = 1024 ;
// const unsigned int MAX_SIZE = 4 * KB;


// starting of blocks in heap
static unsigned int blockStart[100] = {0};

// temp
static float tempStart[100] = {0};

// largest possible block that user can allocate
static unsigned int largestSize;




int initHeap( size_t size ){

	const size_t HeapSize = size;

	unsigned int powOf2 = floor( log(size) / log(2) ) - 1;
	if(powOf2 < 1)powOf2 = 1;
	float sum = 0, percentSum = 0;
	float tempX;
	int i;

	// gaussian 
	for ( i = 0; i < powOf2; ++i)
	{
		// 
		tempX = exp( - pow(  int(i - ((int) 1<< (int)( powOf2 / 5 ) ) ), 2 ) / ( 5.8f ) );
		tempStart[i] = tempX;
		sum += tempX;

	}
	
	//
	for ( i = 0; i < powOf2; ++i)
	{
		tempStart[i] /= sum;
		tempStart[i] *= 100;
		tempStart[i] = (int)(tempStart[i]);
		percentSum+=tempStart[i];
	}

	for( i = 0; i <powOf2 && percentSum!=100; i++){
		++tempStart[i];
		++percentSum;
	}

	sum = 0;

	for( i = 0; i <powOf2 && tempStart[i]!=0; i++){
		sum+=((int)1<<i) * tempStart[i];
	}

	

	for ( i = 0; i <powOf2; ++i)
	{
		tempStart[i] = round(tempStart[i] * size/sum);
	}


	sum = 0;
	for( i = 0; i <powOf2 && tempStart[i]!=0; i++){
		sum+=((int)1<<i) * tempStart[i];
	}

	if( size-sum < 0 ){
		sum-=((int)1<<(i-1) ) * tempStart[i-1];
		--tempStart[i-1];

	}
	//cout << __LINE__ << endl;
	//cout << size-sum << endl;
	//if(size-sum==0)tempStart[0] += 1;
	while(size-sum > 0){
		for( i = 0; i <powOf2 && tempStart[i]!=0; i++){
			if(size-sum>= ((int)1<<i) )
				{
					++tempStart[i];
					size-=((int)1<<i);
				}
		}
	}
	
	

	sum = 0;
	for( i = 0; i <powOf2 && tempStart[i]!=0; i++){
		sum+=((int)1<<i) * tempStart[i];
	}
	largestSize = (int)1<<(i-1);
	blockStart[0] = 0;
	for( i = 0; i <powOf2 && tempStart[i]!=0; i++){	
		if(i>0)
			blockStart[i] = blockStart[i-1]+tempStart[i-1] * ((int)1<<(i-1) );
		cout << ((int)1<<i) << " == " << blockStart[i] << endl;
	}
	cout << sum << endl;
	cout << largestSize << endl;
	

	/// allocation start

	ph_start = malloc( HeapSize );
	//cout << ph_start << endl;
	if( ph_start != NULL ){
		v_service = ph_start;
		v_heap = (char*)ph_start + HeapSize / SERVICE_PART + 1;
		cout << "v_service:\t" << v_service << endl;
		cout << "v_heap:\t\t" << v_heap << endl;
		
		heapSize = HeapSize;
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
	short first = 1;
	if( size > largestSize ) return NULL;
	int powOf2 = floor( log(size) / log(2) );
	if( (1 << powOf2) < size)
		++powOf2;
	// cout << "__in malloc \npowOf2 == " << powOf2 << endl;

	// start malloc
	
	// current adress of the service block of memory
	const char * curAdr = (const char *) v_service + (int)(blockStart[ powOf2 ] / 4);
	// cout << hex << (void*)curAdr << endl;
	// start block of memory programm allocates if it is free
	const char * currentBlock;

	const char * currentBlockStart;

	// counter for bits in byte
	unsigned int i;

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
		for ( i = 0; i < 8; i += 2 ){
			if(first){
				i = (int)( blockStart[ powOf2 ] ) % 4;
				first=0;
			}

			// remember bit were we start
			j = i;

			// remember size what is needed
			tempSize = size;

			// check if next "size" bytes is free
			do{

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
			
			if(currentBlock >= (char*)v_heap + size)
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
					// mask to mark the block
					isFreeMask = (3 << j ); 

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

				return (char*)v_heap + (currentBlockStart - (const char *) v_service) * SERVICE_PART + i / 2;
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
	// cout << "address " << address << endl;
	// cout << "v_heap " << v_heap << endl;
	
	const char * startAdd = (char*)v_service;
	startAdd += startFrom / 4;
	unsigned int j = (startFrom % 4 ) * 2;

	cout << "startAdd " << (void*)startAdd << endl;
	cout << "j " << j << endl;
	int next = 0;
	while( 1 ){
		for(; j < 8; j += 2 ){
			// mask to mark the block
			isFreeMask = (1 << j) + (1 << (j+1)); 
			// mask to mark that next block is used too

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
	cout << "_printHeap_" << endl << setfill('0');
	while( (startOfTheService <= v_heap)  && (heapLeft > 0) ){
		cout << setw(4) << std::dec << heapSize - heapLeft << " " << (void*)((char*)startOfTheService) << " : ";
		for (int i = 0; (i < 16) && (heapLeft > 0) ; ++i)
		{
			//addr = ++startOfTheService;
			cout << hex << setw(2) << (int)*startOfTheService << " ";
			++startOfTheService;
			heapLeft-=4;
		}
		cout << endl;
		//startOfTheService+=16;
	}
	cout << endl;
}
