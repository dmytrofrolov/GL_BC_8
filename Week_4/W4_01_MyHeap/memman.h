/**
*
* @file		memman.h
* @brief	header file with functions for memory management
*
* @author	Dmytro Frolov
* @date		Apr, 2015
*
*/


#ifndef _MEMMAN_H_
#define _MEMMAN_H_

// 2 bits in servise-part for 1 byte in user-part
#define BITS_IN_SERV 2

// there are 8 bits in byte
#define BITS_IN_BYTE 8

// for getting additianal output uncomment next
#define DEBUG_INFO 1

// if successfull allocation
#define SUCCESS 0

// if error in heap initialization
#define HEAP_INIT_ERR 1

// consts for marking free and occup blocks
#define FREE 0
#define OCCUP 1

// constants for bytes MB and KB
extern const unsigned int MB;
extern const unsigned int KB;


/**
* @brief  initialize myheap and allocates memory from system
* @param  [in] size - size of heap we want to allocate
* @return int - 0 if Successfull init and 1 if not
*/
int initHeap( const size_t size );


/**
* @brief  deallocate globalmemory from myheap
* @return int - 0 if Successfull init and 1 if not
*/
int closeHeap( void );


/**
* @brief  allocates memory in myheap
* @param  [in] size - size of bytes we want to allocate in myheap
* @return void* - pointer to memory if Successfull alloc and NULL if not
*/
void* myMalloc( const size_t size );


/**
* @brief  deallocates memory in myheap
* @param  [in] ptr - pointer to memory we want to deallocate in myheap
* @return int - 0 if success or 1 if not
*/
int myFree( void * ptr );


/**
* @brief  prints memory map of heap
* @return void - nothing
*/
void printHeap( void );

#endif /* _MEMMAN_H_  */