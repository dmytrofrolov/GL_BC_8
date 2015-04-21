/**
*
* @file		memman_3.cpp
* @brief	source file with implementation of myheap functions
* @version	0.3
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

// virtual address
static void * v_service = NULL;
static void * v_heap = NULL;

static unsigned int g_heapSize;
extern const unsigned int SERVICE_PART = 4;
extern const unsigned int MB = 1024 * 1024 ;
extern const unsigned int KB = 1024 ;
// const unsigned int MAX_SIZE = 4 * KB;


// starting of blocks in heap
static unsigned int blockStart[64] = {0};

// temp
static float tempStart[64] = {0};

// largest possible block that user can allocate
static unsigned int largestSize;


/////////////////////////////////////////////////////////////////////////////////////////////////////////


int initHeap( size_t size )
{

	const size_t heapSize = size;

	unsigned int powOf2 = floor( log(size) / log(2) ) - 1;
	if(powOf2 < 1)
	{
		powOf2 = 1;
	}

	float sum = 0, percentSum = 0;

	// temp value for normal distribution
	float tempX;

	// counter
	int i;

	// gaussian 
	for ( i = 0; i < powOf2; ++i)
	{
		// 
		tempX = exp( - pow(  int(i - ((int) 1<< (int)( powOf2 / 5 ) ) ), 2 ) / ( 5.8f ) );
		tempStart[i] = tempX;
		sum += tempX;

	}
	
	// make dest to 
	for ( i = 0; i < powOf2; ++i)
	{
		tempStart[i] /= sum;
		tempStart[i] *= 100;
		tempStart[i] = (int)(tempStart[i]);
		percentSum+=tempStart[i];
	}

	for( i = 0; i <powOf2 && percentSum!=100; i++)
	{
		++tempStart[i];
		++percentSum;
	}

	sum = 0;

	for( i = 0; i <powOf2 && tempStart[i]!=0; i++)
	{
		sum+=((int)1<<i) * tempStart[i];
	}

	

	for ( i = 0; i <powOf2; ++i)
	{
		tempStart[i] = round(tempStart[i] * size/sum);
	}


	sum = 0;
	for( i = 0; i <powOf2 && tempStart[i]!=0; i++)
	{
		sum+=((int)1<<i) * tempStart[i];
	}

	if( size-sum < 0 ){
		sum-=((int)1<<(i-1) ) * tempStart[i-1];
		--tempStart[i-1];

	}

	while(size-sum > 0)
	{
		for( i = 0; i <powOf2 && tempStart[i]!=0; i++)
		{
			if(size-sum>= ((int)1<<i) )
			{
				++tempStart[i];
				size-=((int)1<<i);
			}
		}
	}

	sum = 0;

	for( i = 0; i <powOf2 && tempStart[i]!=0; i++)
	{
		sum+=((int)1<<i) * tempStart[i];
	}

	largestSize = (int)1<<(i-1);
	blockStart[0] = 0;

	for( i = 0; i <powOf2 && tempStart[i]!=0; i++)
	{	
		if( i > 0 ) 
		{
			blockStart[i] = blockStart[i-1]+tempStart[i-1] * ((int)1<<(i-1) );
		}
		
		#ifdef DEBUG_INFO
			cout << ((int)1<<i) << " == " << blockStart[i] << endl;
		#endif
	}

	#ifdef DEBUG_INFO
		cout << "Total bytes to allocate : " << sum << endl;
		cout << "Targest block to alloc  : " << largestSize << endl;
	#endif
	

	///! allocation start

	ph_start = malloc( heapSize + heapSize / SERVICE_PART + 1 );

	//cout << ph_start << endl;
	if( ph_start != NULL )
	{
		v_service = ph_start;
		v_heap = (char*)ph_start + heapSize / SERVICE_PART + 1;

		#ifdef DEBUG_INFO
			cout << "v_service:\t" << v_service << endl;
			cout << "v_heap:\t\t" << v_heap << endl;
			cout << "g_heapSize:\t"<< heapSize << endl;
		#endif

		
		g_heapSize = heapSize;
		return SUCCESS;
	}
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

void * myMalloc( size_t size )
{
	
	if( size > largestSize || size == 0 ) 
	{
		return NULL;
	}
	
	short first = 1;
	
	int powOf2 = floor( log(size) / log(2) );
	
	if( (1 << powOf2) < size)
	{
		++powOf2;
	}
	
	
	/// ____ start malloc

	// address in memory what was allocated
	void * returnAddress;
	
	// current adress of the service block of memory
	const char * curAdr = (const char *) v_service + (int)(blockStart[ powOf2 ] / 4);

	// start block of memory programm allocates if it is free
	const char * currentBlock;

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
			// if it is first search startedBit has to be shifted
			if( first )
			{
				startedBit = (int)( blockStart[ powOf2 ] ) % ( BITS_IN_BYTE / BITS_IN_SERV );
				first = 0;
			}

			// remember bit were we start
			currentBit = startedBit;

			// remember size what is needed
			tempSize = size;

			// check if next "size" bytes is free
			do{

				// is free bit mask - check 0, 2st, 4th, 6th bit in order to currentBit
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
			
			if(currentBlock >= (char*)v_heap + size)
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
					// mask to mark the block
					isFreeMask = (3 << currentBit ); 

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
				
				return returnAddress;
		
			} // end if free block founded
			
		} // for each 2-bit in byte end

	} // for each byte in service memory

	// if nothing founded
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int myFree( void * address)
{

	if( address==NULL || ( ( address > ((char*)v_heap+g_heapSize) ) && ( address < v_heap ) ) ) 
	{
		return 1;
	}

	// bit mask to check if this byte is free
	unsigned short isFreeMask = 0;
	unsigned int startFrom = ( (char*)address - (char*)v_heap );

	// is next byte also has to be deallocated
	int next;
	
	const char * currentByte = (char*)v_service;

	currentByte += startFrom / SERVICE_PART;
	
	unsigned int j = (startFrom % SERVICE_PART ) * BITS_IN_SERV;
	
	while( 1 )
	{
		for(; j < BITS_IN_BYTE; j += BITS_IN_SERV )
		{
			// mask to mark the block
			isFreeMask = (1 << j) + (1 << (j+1)); 
			// mask to mark that next block is used too

			next = *(unsigned char*)currentByte & (1 << (j + 1) );
			*(char*)currentByte &= ~isFreeMask;
			//cout << std::dec << j << " " << next << endl;
			if( next == 0 )
			{
				return SUCCESS;
			}

		}
		j = 0;
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

	sizeToPrint = g_heapSize/4 - sizeToPrint +2;
	cout << "g_heapSize = " << std::dec << g_heapSize << endl;
	cout << "sizeToPrint = " << std::dec <<  sizeToPrint << endl;

	// counter for bytes in rows
	int i;
	int heapLeft = g_heapSize;
	const unsigned char * startOfTheService = (unsigned char*)v_service;
	//unsigned char * addr;
	cout << "_printHeap_" << endl << setfill('0');

	while( (startOfTheService <= v_heap)  && (sizeToPrint > 0) )
	{
		cout << setw(4) << std::dec << g_heapSize - heapLeft << " " << (void*)((char*)startOfTheService) << " : ";
		for (i = 0; (i < 16) && (sizeToPrint > 0) ; ++i)
		{

			cout << hex << setw(2) << (int)*startOfTheService << " ";
			
			++startOfTheService;
			
			heapLeft-=4;
			
			--sizeToPrint;
		}
		cout << endl;
	}
	cout << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/// eof memman_3.cpp