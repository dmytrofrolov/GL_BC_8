/**
*
* @file		memman_1.cpp
* @brief	source file with implementation of myheap functions
* @version	0.1
*
* @author	Dmytro Frolov
* @date		Apr, 2015
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

// address for service-part of the heap and user-part
static void * v_service = NULL;
static void * v_heap = NULL;

static int g_heapSize;

// service-part is 4 times less than user-part
const unsigned int SERVICE_PART = 4;

// bytes in MB and KB
extern const unsigned int MB = 1024 * 1024 ;
extern const unsigned int KB = 1024 ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int initHeap( const size_t size )
{
	// try to allocate the memory in global heap
	// size - for user part, size / SERVICE_PART + SERVICE_PART - for service part
	ph_start = malloc( size + size / SERVICE_PART + SERVICE_PART );
	
	// if allocation successfull
	if( ph_start != NULL )
	{
		// service-part starts at the start of the global heat what was allocated
		v_service = ph_start;

		// user-part starts after service-part
		v_heap = (char*)ph_start + size / SERVICE_PART + SERVICE_PART;
		
		#ifdef DEBUG_INFO
			cout << "v_service:\t" << v_service << endl;
			cout << "v_heap:\t\t" << v_heap << endl;
		#endif

		// remember the size what we allocated in gloval var
		g_heapSize = size;

		// allocation was successfull, return success
		return SUCCESS;
	}
	// allocation was not successfull =(
	else
	{
		return HEAP_INIT_ERR;
	}
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

	// if user want to allocate 0 or more than available
	if( size == 0 || size >= g_heapSize )
	{
		return NULL;
	}

	// address in memory what was allocated
	void * returnAddress;

	// current adress of the service-part of memory
	const char * curAdr = (const char *) v_service;

	// start block of memory program allocates if it is free
	const char * currentBlock;

	// remember were was started
	const char * currentBlockStart;

	// counter for bits in byte
	unsigned int startedBit;

	// counter for current search
	unsigned int currentBit;

	// bit mask to check if this byte is free
	unsigned short isFreeMask = 0;

	// size of memory to allocate
	size_t tempSize;

	// for each byte in service block
	for(; curAdr < v_heap ; ++curAdr)
	{

		// remember were we start
		currentBlock = curAdr;
		currentBlockStart = curAdr;
		// for each 2 bits in byte
		for ( startedBit = 0; startedBit < BITS_IN_BYTE; startedBit += BITS_IN_SERV )
		{
			// remember bit were we start
			currentBit = startedBit;

			// remember size what is needed
			tempSize = size;

			// check if next "size" bytes is free
			do{

				// is free bit mask - check 0, 2st, 4th, 6th bit in order to j
				isFreeMask = ( 1 << currentBit );
				
				// if it is not free
				if( *curAdr & isFreeMask )
				{
						break;
				}

				// next var for next bit
				currentBit += BITS_IN_SERV;
				// if we are on 3rd 2-bit next will be zero
				if( currentBit == BITS_IN_BYTE )
				{
					currentBit = 0;
				}


				// if program has checked previous byte move to next one
				if( currentBit == 0 )
				{
					++curAdr;
				}
			}
			while(--tempSize);
			
			// if we found block ouside the user-part
			if(currentBlock >= (char*)v_heap - size)
			{
				return NULL;
			}

			// if the block found ( it started at "currentBlock" ) it marks the
			// part of the service that it will be allocated
			if(tempSize == 0)
			{
				// remember the 2-bit to mark it
				currentBit = startedBit;
				// remember size
				tempSize = size;
				// mark all bits that needed to remember that block is used
				do{
					// mask to mark the block and the next is used
					isFreeMask = ( 3 << currentBit ); 
					// mark it
					*(char*)currentBlock |= isFreeMask;

					// if it was the last byte unmark that next if used
					if( tempSize == 1 )
					{
						isFreeMask = (1 << (currentBit + 1) );
						*(char*)currentBlock ^= isFreeMask;
					}

					// next var for next bit
					currentBit += BITS_IN_SERV;
					// if we are on 3rd 2-bit next will be zero
					if( currentBit == BITS_IN_BYTE )
					{
						currentBit = 0;
					}

					// if program has checked previous byte move to next one
					// only if it is not last size
					if( currentBit == 0 )
					{
						++currentBlock;
					}
				}
				while(--tempSize);

				// return address in user-part of the heap
				returnAddress = (char*)v_heap;

				// it is shifted in order were currentBlockStart shifted in service part
				// don't forget that there are 4 times difference between user- and service- part (SERVICE_PART)
				returnAddress = (char*)returnAddress + (currentBlockStart - (const char *) v_service) * SERVICE_PART;

				// it is also shifted in order to what bit we started to count in service part
				returnAddress = (char*)returnAddress + startedBit / BITS_IN_SERV;
				
				return   returnAddress;
			
			} // end if free block founded
			
		} // for each 2-bit in byte end

	} // for each byte in service memory

	// if nothing founded
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int myFree( void * address)
{
	// if user dont know what he do
	if( address==NULL || ( ( address > ((char*)v_heap+g_heapSize) ) && ( address < v_heap ) ) ) 
	{
		return 1;
	}

	// bit mask to check if this byte is free
	unsigned short isFreeMask;
	unsigned int startFrom = ( (char*)address - (char*)v_heap );
	
	// is next byte is allocated
	int next;
	
	const char * currentByte = (char*)v_service;
	currentByte += startFrom / (BITS_IN_BYTE / BITS_IN_SERV);
	int currentBit = startFrom % (BITS_IN_BYTE / BITS_IN_SERV);

	currentBit *= BITS_IN_SERV;
	
	while( 1 )
	{
		for(; currentBit < BITS_IN_BYTE; currentBit += BITS_IN_SERV )
		{
			// mask to mark the block
			isFreeMask = (3 << currentBit); 
			// mask to mark that next block is used too

			next = *(unsigned char*)currentByte & (1 << (currentBit + 1) );
			*(char*)currentByte &= ~isFreeMask;

			// if next was not allocated with this block
			if( next == 0 )
			{
				return SUCCESS;
			}

		}

		currentBit = 0;
		++currentByte;

	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void printHeap( void )
{
	
	unsigned int sizeToPrint = 0;
	char* endOfHeap = (char*)v_service + g_heapSize / 4;
	while(*endOfHeap == 0 && endOfHeap > v_service )
	{
		++sizeToPrint; 
		--endOfHeap;
	} 
	sizeToPrint = g_heapSize/4 - sizeToPrint + 2;
	//cout << "g_heapSize = " << std::dec << g_heapSize << endl;
	cout << "sizeToPrint = " << std::dec <<  sizeToPrint << endl;


	// counter for 16 bytes in row
	int i = 0;

	// how many bytes in heap to print
	int heapLeft = g_heapSize;

	// start of the service part
	const unsigned char * startOfTheService = (unsigned char*)v_service;
	
	cout << "_printHeap_" << endl;
	
	// while it is in user part and some bytes to print
	while( ( startOfTheService <= v_heap )  && ( sizeToPrint > 0 ) )
	{
		// this row for address :
		cout << (void*)((char*)startOfTheService) << " : ";

		// print 16 bytes in row
		for ( i = 0; ( i < 16 ) && ( sizeToPrint > 0 ) ; ++i)
		{
			cout << hex << setw(2) << setfill('0') << (int)*startOfTheService << " ";
			++startOfTheService;
			heapLeft-=4;
			--sizeToPrint;
		}
		
		cout << endl;
	}
	cout << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/// eof memman_1.cpp