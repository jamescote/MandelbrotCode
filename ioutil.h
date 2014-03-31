// Name: ioutil.h
// Description: The header file for I/O utility functions 
// Written By: James Coté
///////////////////////////////////////////////////////////

// DEFINES
#ifndef IOUTIL_H
#define IOUTIL_H

// INCLUDES
#include <cstdarg>

// FUNCTION PROTOTYPES:
int readInt( const char prompt[], 
	     bool &eof );

bool readBool( const char prompt[], 
	       bool &eof );

char readChar( const char prompt[], 
	       bool &eof, 
	       int iVarArgCount, 
	       ... );

void readString( const char prompt[], 
		 char str[], 
		 int maxlen, 
		 int minlen, 
		 bool &eof );

// End of our define.
#endif
