//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// File class.
// 

#include <stdio.h>

#ifndef _FILE_H_
#define _FILE_H_

#define PATH_SIZE 256
#define READ_PARAM "r"
#define WRITE_PARAM "w"

enum FILE_ERRORS {
	OPEN_ERROR = 1,
	EOF_ERROR,
	READ_ERROR,
	WRITE_ERROR,
	CLOSE_ERROR
};

class File{
private:
	FILE * file_;


public:
	// Constructor - initialize class members
	File();

	// Opens file with received name and parameter
	// @param [char*] - name of file to open
	// @param [const char *] - parameter: open for read or write
	// @return - 0 if success or error code
	int openFile( char * file_name, const char * param );

	// Read part of file to buffer
	// @param [char * const] - buffer to read file in
	// @param [const size_t] - size of bytes to read from file to buffer
	// @return - 0 if success ot error code
	int readFile( char * const buffer, const size_t size );
	
	// Write size bytes from buffer to file
	// @param [char * const] buffer to write from
	// @param [const size_t] number of bytes write to file from buffer
	// @return - 0 if success or error code
	int writeFile( char * const buffer, const size_t size );
	
	// Close file
	// @return - 0 if success or error code
	int closeFile( void );

	// destructor closes the file if it was not closed before
	~File();

};

#endif // _FILE_H_ 