/*
*
*
*
*
*/

#ifndef _MEMMAN_H_
#define _MEMMAN_H_

#define SUCCESS 0
#define HEAP_INIT_ERR 1

extern const unsigned int MB;
extern const unsigned int KB;

int initHeap( size_t size );

int closeHeap( void );

void * myMalloc( size_t size );

int myFree( void * );

void printHeap( void );

#endif // _MEMMAN_H_