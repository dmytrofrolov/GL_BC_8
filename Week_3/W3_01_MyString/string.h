/**
*
* @file		string.h
* @brief	header file of standard C String library 
*
* @author	Dmytro Frolov
* @date		Apr, 2015
*
*/

#ifndef _STRING_H_
#define _STRING_H_

#ifndef BOOL
#define BOOL short
#define bool short
#define FALSE 0
#define false 0
#define TRUE 1
#define true 1
#endif	// BOOL


#ifndef NULL
#define NULL 0
#endif // NULL

typedef unsigned int size_t;


/**
* @brief  function gets '\0'-terminated string and stores it to heap
* @param  [in] char* - address of C-string
* @return void* - address of string with new terminating symbol
*/
void *allocateString(const char *);


/**
* @brief  function gets string with new terminating symbol and returns '\0'-terminated string
* @param  [in] void* - address of MyString
* @return char* - address of string with C-style '\0'-termination
*/
char *to_c_string(const void *);


/**
* @brief  function copy n byte from src address to dest address
* @param  [in, out] dest - address of destination memory
* @param  [in] src - address of source memory
* @param  [in] n - number of bytes to copy
* @return address of destination memory block begin
*/
void *memcpy(void *dest, const void *src, size_t n);


/**
* @brief  function copy n byte from src address to dest address with overwrite control
* @param  [in, out] dest - address of destination memory
* @param  [in] src - address of source memory
* @param  [in] n - number of bytes to copy
* @return address of destination memory block begin
*/
void *memmove(void *dest, const void *src, size_t n);


/**
* @brief  function search byte in block of memory
* @param  [in] str - address of memory
* @param  [in, out] c - char symbol to search
* @param  [in] n - number of bytes to search
* @return address of destination memory block begin
*/
void *memchr(const void *str, char c, size_t n);


/**
* @brief  function check if two blocks of memory are equal
* @param  [in] str1 - address of 1st block of memory
* @param  [in] str2 - address of 2nd block of memory
* @param  [in] n - number of bytes to check
* @return 0 if equal, 1 if str1 > str2, -1 if str1 < str2
*/
int memcmp(const void *str1, const void *str2, size_t n);


/**
* @brief  function set n bytes in memory starting from dest with z
* @param  [in, out] dest - address of block of memory
* @param  [in] z - byte to change with
* @param  [in] n - number of bytes to change
* @warning int z cast to char
* @return 0 if equal, 1 if str1 > str2, -1 if str1 < str2
*/
void *memset(void *dest, int z, size_t n);


/**
* @brief  function concatenate src to the end of dest
* @param  [in, out] dest - address of 1st string
* @param  [in] src - address of 2nd string
* @return address of dest
*/
char *strcat(char *dest, const char *src);


/**
* @brief  function concatenate n bytes from src to the end of dest
* @param  [in, out] dest - address of 1st string
* @param  [in] src - address of 2nd string
* @param  [in] n - number of bytes to concatenate
* @return address of dest
*/
char *strncat(char *dest, const char *src, size_t n);


/**
* @brief  function search (char)value in the dest string
* @param  [in] dest - address of 1st string
* @param  [in] value - char to found
* @warning int value cast to char
* @return address of place were char found or NULL if not found
*/
char *strchr(const char *dest, int value);


/**
* @brief  function search (char)value in the dest string starting from the end
* @param  [in] dest - address of 1st string
* @param  [in] value - char to found
* @warning int value cast to char
* @return address of place were char found or NULL if not found
*/
char *strrchr(const char *dest, int value);


/**
* @brief  function check if two strings are equal
* @param  [in] str1 - address of 1st string
* @param  [in] str2 - address of 2nd string
* @return 0 if equal, 1 if str1 > str2, -1 if str1 < str2
*/
int strcmp(const char *str1, const char *str2);


/**
* @brief  function check if first n bytes of two strings are equal
* @param  [in] str1 - address of 1st string
* @param  [in] str2 - address of 2nd string
* @param  [in] n - number of bytes to check
* @return 0 if equal, 1 if str1 > str2, -1 if str1 < str2
*/
int strncmp(const char *str1, const char *str2, size_t n);\


/**
* @brief  function compares string str1 to string str2, both interpreted according to LC_COLLATE
* @param  [in] str1 - address of 1st string
* @param  [in] str2 - address of 2nd string
* @return 0 if equal, 1 if str1 > str2, -1 if str1 < str2
*/
// int strcoll(const char *str1, const char *str2);


char *strcpy(char *toHere, const char *fromHere);

/**
* @brief  function copy n byte from src string to dest string
* @param  [in, out] dest - address of destination string
* @param  [in] src - address of source string
* @param  [in] n - number of bytes to copy
* @return address of destination string begin
*/
char *strncpy(char *dest, const char *src, size_t n);


/**
* @brief  function interprets value of errnum, generating a string with a message that describes the error
* @param  [in] errnum - value of error
* @return string with text of error
*/
// char *strerror(int errnum);


/**
* @brief  function find length of string
* @param  [in] src - address of string
* @return length of src string
*/
size_t strlen(const char *src);


/**
* @brief  function find length of C-style string
* @param  [in] src - address of string
* @return length of src string
*/
size_t c_strlen(const char *src);


/**
* @brief  function find size of string' part what consist only with symbols from second string
* @param [in] str - address of string
* @param [in] accept - address of string with symbols to find
* @return size of string what consist only with symbols from accept string
*/
size_t strspn(const char *str, const char *accept);


/**
* @brief  function find size of string' part what not consist with symbols from second string
* @param [in] str - address of string
* @param [in] reject - address of string with symbols not to find
* @return size of string what not consist only with symbols from reject string
*/
size_t strcspn(const char *str, const char *reject);


/**
* @brief  function finds first occurrence in str of any symbol from accept string
* @param [in] str - address of string
* @param [in] accept - address of string with symbols to search
* @return address of founded symbol
*/
char *strpbrk(const char *str, const char *accept);


/**
* @brief  function finds substring sub in str
* @param [in] str - address of string
* @param [in] sub - address of substring
* @return address of first occurrence of sub in str
*/
char *strstr(const char *str, const char *sub);


/**
* @brief  function returns addresses of substrings in str which are divided by delimitators
* @param [in] str - address of string or NULL to find second substring
* @param [in] delim - delimitators
* @return address of current substring or NULL
*/
char *strtok(char *str, const char *delim);


/**
* @brief  Transforms the C string (src)  according to the current locale, copies the first num characters ro dest
* @param [in] dest - string what copy to
* @param [in] src -  string what copy from
* @param [in] n -  number of bytes to copy copy from
* @return number of copied bytes
*/
// size_t strxfrm(char *dest, const char *src, size_t n);

#endif //_STRING_H_

// EOF string.h