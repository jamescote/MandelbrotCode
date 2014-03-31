// Name: ioutil.cpp
// Description:  Contains the functions for our two other I/O utility
//               functions required: readBool and readString
// Written By:  James Coté
/////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include <iostream>
#include <stdlib.h>
#include <climits>
#include <vector>
#include "ioutil.h"

// NAMESPACES
using namespace std;

// CONSTANTS
const char cNEWLINE = '\n'; 
const int iCHAR_MAX_LENGTH = 2;
const char cWHITESPACE = ' ';
const char cNULL_CHAR = '\0';

// Name: clearFailBuffer
// Description: Clears any fail state on cin then ignores what's left in the 
//              input buffer.
// Written by:  James Coté
/////////////////////////////////////////////////////////////////////////////
void clearFailBuffer()
{
    if( cin.fail() )
    {
	// Clear the fail state
	cin.clear();	
	cin.ignore( INT_MAX, '\n' );
    }    
}

// Name: clearString
// Description: Sets every value of the c-string to the null character.
// Parameters: char str[] - a pointer to the first element of the c-string
//                          that we wish to clear.
//             int maxlen - the maximum length of the c-string that we want 
//                          to clear (NOTE: may
//                          not be the full size of the character array).
// Written by: James Coté
/////////////////////////////////////////////////////////////////////////////
void clearString( char str[], int maxlen )
{
    // Go through the string, up to max length specified and set the
    // characters to null.
    for( int i = 0; i < maxlen; ++i )
    {
	str[i] = '\0';
    }    
}

// Description: reads in a long integer as required by our readInt function
// Method: Get a line from the user, determine if we triggered an end of file,
//         convert to a long int via strtol and finally set a boolean to see
//         if the long integer can be properly converted to a regular integer.
// Parameters: cInput - reference to the input array that is used to read in
//                      an integer.
//             bEOF - end of file flag, set if we get and eof.
//             bWrongInput - boolean to determine whether the integer entered
//                           was wrong or not.  Passed as reference to use
//                           for loop conditional.
// Return Value: returns the integer read in.  Return value will be different
//               if the user enters an integer outside the bounds of an int,
//               however, in that case, we ask them for a new input.
/////////////////////////////////////////////////////////////////////////////
int Get_int( char cInput[], bool &bWrongInput, bool &bEOF )
{
    // Local Variables
    long int liVar = 0;
    char *cpEndPtr = NULL;

    // Get a line of input from user to parse
    cin.getline( cInput, CHAR_MAX, cNEWLINE );
    bEOF = cin.eof(); // Set our end of file flag.

    // Attempt to convert the input to a long int
    liVar = strtol( cInput, &cpEndPtr, 0 );

    // Set our wrong input flag to determine if the integer read in is correct.
    bWrongInput = ( ( cin.fail() || 
		      ( (*cpEndPtr) != '\0' ) || 
		      ( ( cInput[ 0 ] == cWHITESPACE ) ||
			( cInput[ 0 ] == (*cpEndPtr) ) ) || 
		      ( ( ( liVar > INT_MAX ) ||
			  ( liVar < INT_MIN ) ) ) ) && 
		    !bEOF );

    // return our int value
    return (int)liVar;
}

// Name: readInt( const char prompt[], bool &eof )
// Description: Successfully reads a valid integer.  uses error checking to
//              ensure only a proper integer is kept.
// Method: To start, we prompt the user with the passed in message then read
//         a line of text from the user and set our eof flag in case the user
//         triggers an end of file.  We then take the character string that 
//         was grabbed by the user and attempt to convert it to a long int
//         via the strtol function.  Passed into the strol function is a
//         character pointer that is set to point at the first point in the
//         character array that encounters a problem (if no problem is 
//         encountered, the pointer will be pointing at a NULL character
//         '\0').  After we read the long integer, we attempt to see if the
//         long int lies within the limits of a regular integer.  We're 
//         basically testing the string to make sure it is a valid integer.
//         If the input isn't a valid integer, we enter a while loop until
//         we get input that is valid.  In the loop we prompt the user
//         again and clear any flags that may have arose from our first
//         prompt.  We then repeat the process until we get a valid integer.
//         We also make sure to clear the fail state if we hit the end of
//         file.
// Parameters: const char prompt[] - A message passed in to prompt the user 
//                                   to enter an integer.
//             bool &eof - A pointer to the program's end of file flag.  Set 
//                         to true if we reach the end of file, otherwise, 
//                         set to false if we get a valid integer.
// Returns: The integer that was input from the user.
/////////////////////////////////////////////////////////////////////////////
int readInt( const char prompt[], bool &eof )
{
    // Internal Variable Declarations:
    char cInputString[ CHAR_MAX ] = {};
    bool bWrongInput = true;
    int iReturnValue;

    // Output our prompt message to prompt the user to enter an integer.
    cout << prompt << endl;

    // read in the next int value
    iReturnValue = Get_int( cInputString, bWrongInput, eof );
    
    // While we don't have the input we want, re-prompt the user.
    while( bWrongInput )
    {
	// Output an error message:
	cout << "I'm sorry, '" << cInputString;
	cout << ( cin.fail() ? "..." : "" ) << "' isn't what I'm ";
	cout << "looking for.  Please try again.  " << endl << endl;
	cout << prompt << endl;

	// If we're in the fail state, clear it
	if( cin.fail() )
	{
	    // clear fail state
	    cin.clear();
	    
            // ignore anything left in the input buffer.
	    cin.ignore( INT_MAX, cNEWLINE );
	}
	
	// read in another int value.
	iReturnValue = Get_int( cInputString, bWrongInput, eof );
    }
    
    // If we're leaving because of end of file, clear fail flags.
    if( eof )
    {
	cin.clear();
    }

    // Return the integer we grabbed.
     return iReturnValue;
}

// Name: readBool( const char prompt[], bool &eof )
// Description: Prompts the user to enter in either a 'y' or 'n' character
//              followed by a newline. If we receive the wrong input, 
//              we will notify the user of an error then reprompt the user 
//              until we get the input we're looking for.
// Method: We first use readChar to reliably get a character as input.  We
//         call readChar with 'y' and 'n' variable parameters to ensure we
//         don't get a character that we don't want.  Once we get the
//         character, we make sure there was no end of file called and then
//         we use a switch statement on the input to determine what to 
//         return.  The default case is strictly for safety purposes;
//         readChar should only return 'y' or 'n'.  We then return from the
//         function with the proper return value.
// Parameters: const char prompt - This is a string passed into the function
//                                 and is used to prompt the user for input.
//             bool &eof - This is a reference to a boolean variable used
//                         by the main program to determine when the end of
//                         file is reached.
// Return value: returns a requested boolean value from the user:
//               'y' = true, 'n' = false.
// Written by: James Coté
/////////////////////////////////////////////////////////////////////////////
bool readBool( const char prompt[], bool &eof )
{
    // Create variables
    bool bReturn;
    char cInput = '\0';

    // grab a character from the user, ensure it's a y or an n
    cInput = readChar( prompt, eof, 2, 'y', 'n' );
    
    if( !eof )
    {
	// Parse the first character
	switch( cInput )
	{
	case 'n':            
	    bReturn = false; 
	    break;
	case 'y':            
	    bReturn = true;  
	    break;
	default:      
	    // Prompt error message and request new input.
	    cerr << "I'm sorry, '" << cInput;
	    cerr << "' was not the answer we were looking for.";
	    cerr << "  Please try again." << endl << endl;
	    cerr << prompt << endl;
	    cInput = readChar( prompt, eof, 2, 'y', 'n' );
	    break;
	}
    }

    // Return our boolean value.
    return bReturn;
}

// Name: readChar
// Description: Reliably reads a character from the user, compares it with
//              any additional comparative arguments passed into the
//              function and ensures it's a character that the caller is
//              looking for.
// Method: After declaring our variables, create a vector of variable
//         parameters if there are variable parameters to check
//         (iVarArgCount > 0).  Once our vector is filled, prompt the user
//         and get input.  After we get the input and set our eof flag,
//         we enter a while loop to begin parsing the input for the
//         correct input.  We need to check if (1) the input was entered
//         correctly and (2) it is the proper character (compare it to our
//         variable arguments).  Otherwise, we've succeeded and we can
//         return the input.  If we didn't succeed, we reprompt the user
//         and ask for input again.
// Parameters: prompt - Message to prompt the user to enter a character.
//             eof - EoF flag passed by reference to determine if the user
//                   triggers an end of file.
//             iVarArgCount - Integer that indicates how many variable
//                            arguments are passed into the function.
//             ... - variable character arguments that are used when the
//                   caller is looking for a specific character(s).
///////////////////////////////////////////////////////////////////////////
char readChar( const char prompt[], bool &eof, int iVarArgCount, ... )
{
    // Variable Declarations
    char cInput[ iCHAR_MAX_LENGTH ] = { };
    vector< char > cCompareList;
    bool bSuccess = false;

    // if there's variable arguments, store them in a character array
    if( iVarArgCount > 0 )
    {
	// Get our Variable Arguments
	va_list va_Character_List;
	va_start( va_Character_List, iVarArgCount );

	for( int i = 0; i < iVarArgCount; ++i )
	{
	    cCompareList.push_back( ( char ) va_arg( va_Character_List, int ) );
	}

	va_end( va_Character_List );
    }

    // Prompt the user to enter a character
    cout << prompt << endl;
    
    // read in input
    cin.getline( cInput, ( iCHAR_MAX_LENGTH + 1 ), cNEWLINE );
    eof = cin.eof();

    // Error Checking
    while( !bSuccess & !eof )
    {
	if( cInput[ iCHAR_MAX_LENGTH - 1 ] != cNULL_CHAR )
	{
	    // Error, output a message and re-prompt
	    cerr << "I'm sorry, the format needs to contain a character ";
	    cerr << "followed by a newline.  Please try again." << endl;
	    cerr << endl << prompt << endl;

	    // Clear the fail state and input buffer
	    clearFailBuffer();
	}
	else if( !cCompareList.empty() )
	{
	    // We have arguments to compare our input against
	    for( vector< char >::iterator it = cCompareList.begin(); it != cCompareList.end(); ++it )
	    {
		// If it is a character we're looking for, we succeeded.
		if( cInput[ 0 ] == (*it) )
		{
		    bSuccess = true;
		}
	    }
	    
	    // If not successful, let the user know.
	    if( !bSuccess )
	    {
		cerr << "I'm sorry, I'm looking for one of the following";
		cerr << " characters: ";
		
		for( vector< char >::iterator it = cCompareList.begin(); it != cCompareList.end(); ++it )
		{
		    cerr << "'" << (*it) << "'";
		    
		    if( (it + 2) == cCompareList.end() )
			cerr << ", or ";
		    else if( (it + 1 ) != cCompareList.end() )
			cerr << ", ";
		}

		cerr << "." << endl << endl;
		cerr << prompt << endl;
	    }
	}
	else
	    bSuccess = true;

	// "If at first you don't succeed, try, try again."
	if( !bSuccess )
	{
	    // Clear the Input String
	    clearString( cInput, iCHAR_MAX_LENGTH );

	    // get more input from the user.
	    cin.getline( cInput, ( iCHAR_MAX_LENGTH + 1 ), cNEWLINE );
	    eof = cin.eof();
	}
    }
    
    // Clear fail state if we are leaving because of an end of file.
    if( eof )
    {
	cin.clear();
    }

    // return the character
    return cInput[ 0 ];
}

// Function Name: readString
// Description: Prompts the user for a string between a minimum and maximum
//              length as specified by the variables passed in.  
//              Upon reading the string, it will parse the string to make 
//              sure it's within the specified bounds.  Once it receives 
//              a string from the user that is valid, it will exit the 
//              function.
// Method: When we enter the function, we first set every element in the 
//         return string to '\0' for use with error checking later.
//         We then prompt the user and grab a string from them.  We set
//         getline to read in up to a maximum of maxlen+1 because it
//         will enter a fail state if it attempts to read past that and
//         then we know that the string entered is too long.  We use
//         the same boolean flag technique for our while loop as we did
//         in the readbool function.  We then check to see if the string
//         entered is either too long or too short.  Otherwise, we set
//         our flag to true, clear any eof flags, then return.
// Parameters: const char prompt[] - a c-string that contains a message for
//                                   the user prompting them to enter a
//                                   string.
//             char str[] - a pointer to the first element of the c-string
//                          that the user's string will be stored in.
//             int maxlen - an integer value that specifies the maximum
//                          length of the string to be read.
//             int minlen - an integer value that specifies the minimum
//                          length of the string to be read.
//             bool &eof - a reference to the program's end of file boolean
//                         to determine if the user enters an end of file
//                         ( ctrl+D ).
// Written by:  James Coté
/////////////////////////////////////////////////////////////////////
void readString( const char prompt[], char str[], int maxlen, int minlen, bool &eof )
{
    // Initialize our success flag
    bool bSuccess = false;

    // Ensure our str c-string is nullified.  Don't assume it is.
    clearString( str, maxlen );

    // Prompt the user for input
    cout << prompt << endl;

    // Get input from the user then ensure we haven't reached the 
    // end of file.
    cin.getline( str, ( maxlen + 1 ), '\n' );
    eof = cin.eof();

    //check to make sure it's valid.
    while( !bSuccess && !eof )
    {
	// Check to make sure minlen isn't set to 0 so we don't go out 
	// of bounds in the array.  Also, make sure that the string is
	// at least the minimum length.
	if( ( minlen != 0 ) && ( str[ minlen - 1 ] == '\0' ) )
	{
	    // Output an error message to let the user know that what they
	    // entered was too short of a string.
	    cerr << "I'm sorry, the minimum length of the string must be ";
	    cerr << minlen << " long.  Please try again." << endl << endl;
	    cerr << prompt << endl;
	    clearString( str, maxlen );
	}
	else if( cin.fail() )  // Entered fail state (input > maxlength)
	{
	    cerr << "I'm sorry, but the string you entered was too long.";
	    cerr << "  Please keep it at a maximum length of " << maxlen;
	    cerr << ".  Thank you." << endl << endl << prompt << endl;
	    clearFailBuffer();
	}
	else                   // We succeeded, trigger our success flag.
	{
	    bSuccess = true;
	}

	// If we didn't succeed, read in another string from the user.
	if( !bSuccess )
	{
	    cin.getline( str, ( maxlen + 1 ), '\n' );
	    eof = cin.eof();
	}
    }

    // If we're leaving because of end of file, clear fail flags.
    if( eof )
    {
	cin.clear();
    }

    // Exit function
    return;
}
