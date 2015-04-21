/**
*
* @file		memman_4.cpp
* @brief	source file with implementation of myheap functions
* @version	0.4
*
* @author	Dmytro Frolov
* @date		Apr, 2015
*
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <cstring> // for memmove
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

unsigned int g_heapSize;


extern const unsigned int MB = 1024 * 1024 ;
extern const unsigned int KB = 1024 ;

static const unsigned int SERV_PART = 3;

// it will interpret service-part as sequence of Block structures
struct Block
{
	char *start;
//	char *next; 
	size_t len:31;
	char state:1;	// 1 - free, 0 - occupied
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int initHeap( const size_t size )
{
	
	// var for initial marking service-part
	Block * serviceBlock;

	// try to allocate in global heap
	ph_start = malloc( size + size / SERV_PART + SERV_PART );
	
	// if it is not allocated
	if( ph_start == NULL ) 
	{
		return HEAP_INIT_ERR;
	}

	// start of service-part
	v_service = ph_start;

	// start of user-part
	v_heap = (char*)ph_start + size / SERV_PART + SERV_PART;

	// write to service block
	serviceBlock = ((Block*)v_service);

	// initial states
	serviceBlock-> start = (char*)v_heap;
	serviceBlock-> len 	 = size;
	serviceBlock-> state = FREE; 
	
	// remember the size of the heap
	g_heapSize = size;
	
	return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int closeHeap( void )
{
	free( ph_start );
	return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void * myMalloc( const size_t size )
{
	Block * currentBlock = ((Block*)v_service);
 	//cout <<  currentBlock->len << endl;
	do{
		// move to first free block
		while( (currentBlock->state != FREE) && (currentBlock < v_heap) )
		{
			++currentBlock;
		}
		// if there are no bytes to allocate
		if(currentBlock >= v_heap)
		{
			return NULL;
		}

		// if found just size enought block
		if( currentBlock->len == size )
		{
			currentBlock->state = OCCUP;
			return currentBlock->start;
		}

		// if found bigger size block
		if( currentBlock->len > size )
		{
			// if next if not free
			if( (currentBlock+1)->start != NULL )
			{
				// move service-block
				memmove(currentBlock + 2, currentBlock + 1, (char*)v_heap - (char*)currentBlock + 1);
			}

			// update info in next one (the free bytes left)
			(currentBlock+1)->start = (currentBlock)->start + size;
			(currentBlock+1)->len = (currentBlock)->len - size;
			(currentBlock+1)->state = FREE;

			// update this one
			currentBlock->state = OCCUP;
			currentBlock->len = size;
			return currentBlock->start;
		}

		// move to next block
		++currentBlock;
	}while( 1 );

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int myFree( void * address)
{
	Block * currentBlock = ((Block*)v_service);
	//cout << (void*)currentBlock->start << endl;
	while( (currentBlock->start != address) && (currentBlock->start < (char*)v_heap+g_heapSize) ){
		++currentBlock;
	}

	if( currentBlock->start >= (char*)v_heap+g_heapSize )
		return 1;
	
	currentBlock->state = FREE;
	if( currentBlock != v_service && (currentBlock-1)->state == FREE )
	{
		//(currentBlock-1)->start = currentBlock->start;
		(currentBlock-1)->len = (currentBlock-1)->len + currentBlock->len;

		memmove(currentBlock, currentBlock+1, g_heapSize - abs((char*)currentBlock + 1 - (char*)v_heap ) );
		
		if( currentBlock->state == FREE )
		{
			(currentBlock-1)->len = (currentBlock-1)->len + currentBlock->len;
			memmove(currentBlock, currentBlock+1, g_heapSize - abs((char*)currentBlock +1 - (char*)v_heap ) );
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void printHeap( void )
{

	Block * currentBlock = ((Block*)v_service);

	cout << "_printHeap_" << endl << setfill('0');
	while( (currentBlock < v_heap) )
	{
		if( currentBlock->start !=NULL )
		{
			cout << (void*)currentBlock->start 
				 << " " << currentBlock->len 
				 << "\t"<< ((int)currentBlock->state == 0?"FREE":"1") 
				 << endl;
		}
		++currentBlock;
	}
	cout << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/// eof memman_4.cpp