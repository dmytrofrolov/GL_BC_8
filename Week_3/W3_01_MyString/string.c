/**
*
* @file		string.c
* @brief	implementation of standard C String library 
*
* @author	Dmytro Frolov
* @date		Apr, 2015
*
*/

#include "string.h"

#define EQUAL 0
#define GREATER 1
#define LESS -1

const unsigned char TERM_SYMBOL = '~';



//////////////////////////////////////////////////////////////////////////////////

void *allocateString(const char *cString){

	// size of source - cString
	size_t cStringSize = c_strlen( cString );

	// allocate memory for MyString
	char * newString = (char*)malloc( ( cStringSize + 1) * sizeof(char) );

	// copy all bytes from cString to new MyString
	memcpy(newString, cString, cStringSize );

	// add terminating symbol
	*( newString + cStringSize ) = TERM_SYMBOL;
	
	return newString;
}

//////////////////////////////////////////////////////////////////////////////////

char *to_c_string(const void * source){

	// size of source
	size_t sourceSize = strlen(source);

	// allocate new memory for C-style char
	char * newCString = (char*)malloc( ( sourceSize + 1 ) * sizeof(char) );

	// copy all bytes to new memory
	memcpy(newCString, source, sourceSize);
	
	// add terminating char
	*( newCString + sourceSize ) = '\0';
	
	return newCString;
}

//////////////////////////////////////////////////////////////////////////////////

void *memchr(const void *ptr, char value, size_t num){
	
	// end of memory block
	const void *endS = ptr + num;

	// while is not end of the block
	for(;ptr!=endS;++ptr){
		// if value found
		if ( *(char*)ptr == value ) {
			// return pointer to memory were value found
			return (void*)ptr;
		}
	}
	// if nothing found
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

int memcmp(const void *s1, const void *s2, size_t n){
	if( n == 0 )
		return 0;

	// counter for bytes
	unsigned int i;

	// for all n bytes
	for( i = 0; i < n; ++i ){
		if( *(char*)s1 > *(char*)s2 ){
			return GREATER;
		}else{ 
			if( *(char*)s1 < *(char*)s2 ){
				return LESS;
			}
		}
		++s1;
		++s2;
	}
	return EQUAL;
}

//////////////////////////////////////////////////////////////////////////////////

void *memcpy(void *dest, const void *src, size_t n){
	// counter of bytes to copy
	unsigned int i;

	// for each byte
	for( i = 0; i < n; ++i )
		// copy src to dest
		*(((char*)dest) + i) = *(((char*)src) + i);
	return dest;
}

//////////////////////////////////////////////////////////////////////////////////

void *memmove(void *dest, const void *src, size_t n){
	// counter for n bytes
	unsigned int i;

	// difference between dest and src in memory
	unsigned int diff = dest - src;
	char signOfDiff = ( diff < 0 ) ? -1 : 1;
	// if the src and dest overlap - not to copy
	if( diff * signOfDiff > n ){
		return memcpy(dest, src, n);
	}
	
	for(; n != 0; --n)
		*(((char*)dest) + n ) = *(((char*)src) + n );
	return dest;
}

//////////////////////////////////////////////////////////////////////////////////

void *memset(void *dest, int z, size_t size){
	if( size == 0 )
		return dest;

	// start of dest
	void *newDest = dest;
	
	// this var is to remember the termination char
	int lessThanSize = 0;
	
	// if destination string size less than we want to fill
	// remember it in lessThanSize variable
	if(strlen(dest) <= size){
		lessThanSize = 1;
	}

	// while some bytes to set
	while(size){
		// set current char with z
		* ( (char*)dest ) = (char)z;

		// move to next byte
		++dest;

		// decrease the number what left to fill
		--size;
	}
	
	// if we set more than destination was, write new eol symbol at the end
	if(lessThanSize)
		*(char*)dest = TERM_SYMBOL;
	return newDest;

}

//////////////////////////////////////////////////////////////////////////////////

char *strcat(char *dest, const char *src){
	// copy src to memory after dest including termination char
	memcpy( dest + strlen(dest), src, strlen(src) + 1);
	
	return dest;
}

//////////////////////////////////////////////////////////////////////////////////

char *strncat(char *dest, const char *src, size_t size){
	if( size == 0 )
		return dest;

	// sizes of dest and src
	size_t srcSize = strlen(src);
	size_t destSize = strlen(dest);
	
	// if we want to concatenate less than available
	if( size <= srcSize ){
		// concatenate it
		memcpy( dest + destSize, src, size );

		// add termination symbol
		*( dest + destSize + size ) = TERM_SYMBOL;
	}
	// if we want to concatenate more than available
	else {
		// just concatenate src to dest
		memcpy( dest + destSize, src, srcSize + 1 );
	}
	return dest;
}

//////////////////////////////////////////////////////////////////////////////////

char *strchr(const char *src, int value){
	// for each symbol
	while( *src!=TERM_SYMBOL){
		// if value found return adress to it
		if (*src==value) {
			return (char*)src;
		}

		// move to next char
		++src;
	}

	// if nothing found
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

char *strrchr(const char *src, int value){
	// char points to the end of the string including terminating char
	const char *endS = src + strlen(src) + 1;

	// for each char in string from end to start
	for(; endS != src; --endS ){
		// if value found return it's adress
		if ( *endS == value ) {
			return (void*)endS;
		}
	}
	
	// nothing found return NULL
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

int strcmp(const char *s1, const char *s2){
	// while some symbols to compare
	while( ( *s1 != TERM_SYMBOL ) && ( *s2 != TERM_SYMBOL ) ){
		if( *s1 > *s2 ){
			return GREATER;
		}
		else {
			if( *s1 < *s2 ){
				return LESS;
			}
		}

		// move to next char
		++s1;
		++s2;
	}
	// if strings both ends - thay are equal
	if( ( *s1 == TERM_SYMBOL ) && ( *s2 == TERM_SYMBOL ) ){
		// 0 - thay are equal
		return EQUAL;
	}
	// if thay are not equal
	else {
		// if second ends
		if( *s2 == TERM_SYMBOL ){
			return GREATER;
		}
		// if first ends
		else {
			return LESS;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

int strncmp(const char * s1, const char * s2, size_t size){
	if( size == 0 )
		return EQUAL;

	// while some symbols to compare
	while( ( *s1 != TERM_SYMBOL ) && ( *s2 != TERM_SYMBOL ) && ( size != 0 ) ){

		if( *s1 > *s2 ){
			return GREATER;
		}
		else {
			if( *s1 < *s2 ){
				return LESS;
			}
		}
		// move to next character
		++s1;
		++s2;

		// decrease the number of chars to compare
		--size;
	}
	// if there are not any chars to compare or the end of the both strings
	if( size == 0 || ( *s1 == TERM_SYMBOL && *s2 == TERM_SYMBOL ) ){
		return EQUAL;
	}
	// if it is the end of s1 or s2
	else{ 
		// if it is the end of s2
		if( *s2 == TERM_SYMBOL ){
			return GREATER;
		} 
		// if it is the end of s1
		else {
			return LESS;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

// int strcoll(const char *, const char *);

//////////////////////////////////////////////////////////////////////////////////

char *strcpy(char *dest, const char *src){
	// copy src to dest include the terminating char
	return memcpy( dest, src, strlen(src) + 1 );
}

//////////////////////////////////////////////////////////////////////////////////

char *strncpy(char *dest, const char *src, size_t size){
	// if nothing to copy exit from file
	if( size == 0 )
		return destBegin;

	// begin of the dest
	char * destBegin = dest;

	// while some symbols to copy and it is not the end of dest or src
	while( ( size != 0 ) && ( *dest != TERM_SYMBOL ) && ( *src != TERM_SYMBOL ) ){
		// copy current char from src to dest
		*dest = *src;

		// move to next char
		++dest;
		++src;

		// decrease the size of bytes to copy
		--size;
	}

	// return the adress of the dest begin
	return destBegin;
}

//////////////////////////////////////////////////////////////////////////////////

// char *strerror(int);

//////////////////////////////////////////////////////////////////////////////////

size_t strlen(const char * source){
	// pointer to the string start
	const char * startSource = source;

	// find the end of the string
	for(; *source != TERM_SYMBOL; ++source);

	// return the difference end-begin what is the length
	return source - startSource;
}

//////////////////////////////////////////////////////////////////////////////////

size_t c_strlen(const char * source){
	// length of C-string
	size_t len = 0;

	// while it is not the length of the string
	while( *source != '\0'){
		// increase length 
		++len;

		// move to next char
		++source;
	}

	// return length of C-string
	return len;
}

//////////////////////////////////////////////////////////////////////////////////

size_t strspn(const char *str1, const char *str2){
	// number of symbols in str1 that are part of str2
	size_t n = 0;

	// find current char
	char * tempChar;

	// while there are symbols in str1
	while( *str1 != TERM_SYMBOL ){
		// find current str1 char in str2
		tempChar = strchr(str2, *str1);

		// if char found
		if( tempChar != NULL){
			// increase the number of founded chars
			++n;
		}else{ 
			// return number of chars in str1 that is consist of str2 symbols
			return n;
		}

		// move to next character
		++str1;
	}
	return n;
}

//////////////////////////////////////////////////////////////////////////////////

size_t strcspn(const char *str1, const char *str2){
	// number of symbols that not part of str2
	size_t n = 0;

	// find current char
	char * tempChar;

	// while symbols in str1
	while( *str1 != TERM_SYMBOL ){
		// find current str1 char in str2
		tempChar = strchr(str2, *str1);

		// if char not found
		if( tempChar == NULL){
			// increase the number of not founded chars
			++n;
		}else{ 
			// return number of chars in str1 that is not consist of str2 symbols
			return n;
		}

		++str1;
	}
	return n;
}

//////////////////////////////////////////////////////////////////////////////////

char *strpbrk(const char *str1, const char *str2){
	char * tempChar;

	// while some symbols in str1
	while( *str1 != TERM_SYMBOL ){
		// find for current str1 char in str2
		tempChar = strchr( str2, *str1 );

		// if found char in str2
		if( tempChar!=NULL ){
			// return pointer to place where char in str1 found
			return (char*)( str1 );
		}

		// move to next char
		++str1;
	}
	return (char *)NULL;
}

//////////////////////////////////////////////////////////////////////////////////

char *strstr(const char *str1, const char *str2){
	const char *str1TempCopy;
	const char *str2TempCopy;

	// if found a substring ( it has to be true, because if not found it becomes false )
	bool found = true;

	// while some symbols in str1
	while( *str1 != TERM_SYMBOL ){
		// initialize temp copies with current parts of strings
		str1TempCopy = str1;
		str2TempCopy = str2;

		// while some symbols in secons string
		while( *str2TempCopy != TERM_SYMBOL ){
			// if current symbol not the same, mark found as not found and break current search
			if( *str2TempCopy != *str1TempCopy ){
				found = false;
				break;
			}
			++str2TempCopy;
			++str1TempCopy;
		}

		if(found == true){
			return (char*)str1;
		}else{
			found = true;
		}

		// move to next char in str1
		++str1;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

char *strtok(char *str, const char *delimiters){
	// address of current token
	char * tempToken;

	// adress of current string
	static char * strSt = NULL;

	if(delimiters == NULL) {
		return NULL;
	}

	if(str != NULL) {
		strSt = str;
	}else{
		// if both static string and str what function get NULL return
		if(strSt == NULL) 
			return NULL;
	}
	
	// if there are more than one delimeter in row
	do{
		tempToken = strSt;

		// now string points to part of the itself were delimiter found
		strSt = strpbrk( strSt, delimiters );

		// if it is not the same place (if first char not delim)
		if( tempToken != strSt ){
			break;
		}else{
			++strSt;
		}
	}while(1);

	// if it was the last token
	if( strSt == NULL ) 
		return tempToken;

	// add terminated char were current delimiter is
	*(strSt) = TERM_SYMBOL;

	// move the pointer to string from place were last delim was to next char
	++strSt;

	return tempToken;

	
}

//////////////////////////////////////////////////////////////////////////////////

// size_t strxfrm(char *dest, const char *src, size_t n);

//////////////////////////////////////////////////////////////////////////////////

// EOF string.c