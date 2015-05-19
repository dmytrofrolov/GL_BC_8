//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// File class.
// 

#include "File.h"
#include <stdio.h>
#include <string.h>

#define SUCCESS_RESULT 0

////////////////////////////////////////////////////////////////////////////

File::File():
file_ ( NULL )
{

}

////////////////////////////////////////////////////////////////////////////

int File::openFile( char * file_name, const char * param ){

	// if input parameters are wrong
	if( file_name == NULL || param == NULL ){
		return OPEN_ERROR;
	}

	// if file was opened but was not closed before
	if( file_ != NULL ){
		closeFile();
	}

	// if end-line symbol is somewhere in file_name, change it to '\0'
	void * file_end_line = strchr( file_name, '\n' );
	if( file_end_line != NULL ){
		memset( file_end_line, 0, 1 );
	}

	file_ = fopen ( file_name, param );

	if( file_ == NULL )
		return OPEN_ERROR;

	return SUCCESS_RESULT;
}

////////////////////////////////////////////////////////////////////////////

int File::readFile( char * const buffer, const size_t size ){
	if( buffer == NULL || size == 0 || file_ == NULL )
		return READ_ERROR;

	if( feof ( file_ ) )
		return EOF_ERROR;

	char * read_result = fgets ( buffer, size, file_ );

	if( read_result == NULL )
		return READ_ERROR;

	return SUCCESS_RESULT;
}

////////////////////////////////////////////////////////////////////////////

int File::writeFile( char * const buffer, const size_t size ){
	if( buffer == NULL || size == 0 || file_ == NULL )
		return READ_ERROR;

	size_t write_result = fwrite ( buffer, sizeof(char), size, file_ );

	if( write_result != size )
		return WRITE_ERROR;
	
	return SUCCESS_RESULT;
}

////////////////////////////////////////////////////////////////////////////

int File::closeFile(){
	if( file_ != NULL ){
		fclose( file_ );
		return SUCCESS_RESULT;
	}

	return CLOSE_ERROR;
}

////////////////////////////////////////////////////////////////////////////

File::~File(){
	closeFile();
}

////////////////////////////////////////////////////////////////////////////


/// EOF File.cpp