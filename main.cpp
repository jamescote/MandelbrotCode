// Name: main.cpp
// Description: Main driver for the Magick program.
// Purpose: This program is designed to draw a mandelbrot image recursively.
// Known Issues: 
// Written by: James Coté
// ID: 201512008
// MRU E-Mail: jcote008@mtroyal.ca
////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include "Mandelbrot.h"
#include "ioutil.h"
#include <iostream>

// NAMESPACES
using namespace std;

// CONSTANTS
const int iMAX_FILE_NAME_LENGTH = 20;
const int iMIN_FILE_NAME_LENGTH = 5;
const int iMAX_DIMENSION_ITERATIONS = 5;

// FUNCTION DECLARATIONS
sColorCode Initiate_Color_Code( );
int Get_Recursive_Int( const char cPrompt[], bool &bEOF, int iIteration = 0 );
void Get_Color_Code( sColorCode &sColor, bool &bEOF );
float Get_RGB_Mask( const char cPrompt[], bool &bEOF );
bool Get_Informed_Bool( const char cReadBoolPrompt[],
			const char cTitle[],
			const char cTrueCaseExplanation[],
			const char cFalseCaseExplanation[],
			bool &bEOF );

// DEFINES
#define Formatting cout << "_______________________________________" << endl << endl
#define ExitLine   cout << "End of File Reached.  Exiting Program..." << endl << endl

// Description: Our main function is designed to simply get all the necessary
//              information from the user and call the Draw Image function.
//              This is done in one pass and in the even of an EoF, we simply
//              skip the rest of the input asked by the user and end the program.
// Method: After declaring local variables, we output a welcome message and get
//         the name of the file the user would like to save.  We assume that the
//         user enters in the proper file extension for the file name.  If they
//         don't, then they have to wait for the entire image to be drawn before
//         they get an error message from Magick++ about not knowing what kind of
//         file it is.  After getting the file name, we ask the users for the X
//         and Y dimensions of the image.  This is called from a seperate function
//         that recursively ensures that the Dimensions are > 0.  After the
//         dimensions are set, we call a function that will set up any color 
//         filters from the user.  If we didn't hit an end of file, we create the
//         image and exit the program once complete.
// Assumptions: We assume the user enters a proper file extension for the file name.
////////////////////////////////////////////////////////////////////////////////////
int main( )
{
    // Local Variables
    sColorCode sColor = Initiate_Color_Code( );
    char cFileName[ iMAX_FILE_NAME_LENGTH ] = { };
    int iXDimension = 0;
    int iYDimension = 0;
    int iMax_Iterations = 100;
    bool bEOF = false;

    Formatting;

    cout << "Welcome to the Mandelbrot Set image generator (Ver: 1.0)!" << endl << endl;

    // Get File Name
    readString( "Please enter the name of the file you'd like to save the image to: ", 
		cFileName,
		iMAX_FILE_NAME_LENGTH,
		iMIN_FILE_NAME_LENGTH,
		bEOF );

    if( !bEOF )
	Formatting;
    else
	ExitLine;   

    // Get Image Parameters
    iXDimension = Get_Recursive_Int( "Please enter the width of the image (must be > 0): ", bEOF );
    iYDimension = Get_Recursive_Int( "Please enter the height of the image (must be > 0): ", bEOF );
    iMax_Iterations = Get_Recursive_Int( "Please enter the maximum iterations to use when determining if a pixel lies in the Mandelbrot Set (a good default is 100): ", bEOF );

    if( !bEOF )
	Formatting;

    // Set up our color filters
    Get_Color_Code( sColor, bEOF );

    if( !bEOF )
	Formatting;

    // Create the Image
    if( !bEOF )
	Create_Image( cFileName, 
		      iXDimension, 
		      iYDimension, 
		      iMax_Iterations, 
		      sColor );

    if( !bEOF )
	Formatting;

    return 0;
}

// Description: This function initializes the sColorCode struct to the default
//              settings and returns the newly created sColorCode.
// Defaults: Each mask is set to 1.0f which will keep the same values that the
//           algorithm decides for the color weight (colorweight * 1).  bInverColors
//           and bGreyScale are both set to false.
// Return Value: Returns a sColorCode variable set to the default parameters.
////////////////////////////////////////////////////////////////////////////////////
sColorCode Initiate_Color_Code( )
{
    sColorCode sReturnValue;
    
    sReturnValue.fRGBMask[ eRED ]   = 1.0f;
    sReturnValue.fRGBMask[ eGREEN ] = 1.0f; 
    sReturnValue.fRGBMask[ eBLUE ]  = 1.0f;
    sReturnValue.bInvertColors      = false;
    sReturnValue.bGreyScale         = false;

    return sReturnValue;
}

// Description: This function handles all the logic for prompting the user and 
//              setting the Color Filters for the image.
// Method: If we encountered an End of File before entering this function then we
//         don't do anything.  Otherwise, we first ask the user if they would like
//         to edit the colors on the image.  If they choose yes, we first prompt
//         them to enter in a percentage value for each RGB value that will be used
//         to mask the weight to a specified color.  Following that, we ask the user
//         if they would like to invert the colors and/or implement a grey scale
//         filter.  These assignments call a special function that informs the user
//         of what will happen based on their choice.
// Parameters: sColor - A reference to the sColorCode struct that will hold all the
//                      newly set up parameters from the user.
//             bEOF - A boolean reference that we check to make sure we haven't
//                    encountered an end of file.
////////////////////////////////////////////////////////////////////////////////////
void Get_Color_Code( sColorCode &sColor, bool &bEOF )
{
    if( !bEOF )
    {
	// Local Variables
	bool bSetUpColors = readBool( "Would you like to edit the color that the image will take (y/n)? ", bEOF );

	if( bSetUpColors )
	{
	    if( !bEOF )
		Formatting;

	    // Get RGB Mask Values
	    sColor.fRGBMask[ eRED ]   = 
		Get_RGB_Mask( "Please enter a weight mask for Red (0-100%): ", bEOF );
	    sColor.fRGBMask[ eGREEN ] = 
		Get_RGB_Mask( "Please enter a weight mask for Green (0-100%): ", bEOF );
	    sColor.fRGBMask[ eBLUE ]  = 
		Get_RGB_Mask( "Please enter a weight mask for Blue (0-100%): ", bEOF );

	    if( !bEOF )
		Formatting;

	    // Inform the user and prompt them to set up the bInvertColors boolean
	    sColor.bInvertColors = Get_Informed_Bool( "Would you like the colors inverted (y/n)? ",
						      "Color Inversion: (True/False)",
						      "True: The color you specified will be Outside the mandelbrot set.",
						      "False: The color you specified will be Inside the mandelbrot set.",
						      bEOF );

	    if( !bEOF )
		Formatting;

	    sColor.bInvertSpectrum = Get_Informed_Bool( "Would you like to flip the color spectrum (y/n)? ",
							"Spectrum Inversion: (True/False)",
							"True: Each color mask you specified will be on the opposite color spectrum (70% -> 30%, etc).",
							"False: Each color mask you specified will remain the same.",
							bEOF );

	    if( !bEOF )
		Formatting;

	    // Inform the user and prompt them to set up the bGreyScale boolean
	    sColor.bGreyScale = Get_Informed_Bool( "Would you like to turn on the Grey Scale (y/n)? ",
						   "Grey Scale: (True/False)",
						   "True: Set the colors to be on a scale from Black to White.",
						   "False: Set the colors to be on the color spectrum you masked.",
						   bEOF );
	}
	else if( bEOF )
	    ExitLine;
    }
}

// Description: Prompts the user for a weight percentage to return a desired color
//              weight.
// Method: After initializing our variables, we do a check to ensure we haven't
//         encountered an end of file.  If we haven't, we read an int from the user.
//         We don't care what the user enters in because if they enter anything out
//         of our bounds (0-100) we simply truncate it to a bound before converting
//         it to a floating point variable that we return to the caller.
// Parameters: cPrompt - the prompt passed in from the caller that we use when
//                       calling the read int function.
//             bEOF - A boolean reference that we check to make sure we haven't
//                    encountered an end of file.
// Return Value: We return a floating point value that signifies a weight (0.0 - 1.0).
////////////////////////////////////////////////////////////////////////////////////
float Get_RGB_Mask( const char cPrompt[], bool &bEOF )
{
    // Local Variables
    float fReturnValue = 1.0f;
    int iPercent = 100;

    if( !bEOF )
    {
	// Get input
	iPercent = readInt( cPrompt, bEOF );

	if( bEOF )
	    ExitLine;

	// Truncate Input
	if( iPercent < 0 )        // 0% Min
	    iPercent = 0;
	else if( iPercent > 100 ) // 100% Max 
	    iPercent = 100;

	// Convert to Float
	fReturnValue = (float)(iPercent) / 100.0f; 
    }

    return fReturnValue;
}

// Description: We inform the user of what effects setting the boolean will have
//              before prompting them to enter a boolean that we return to the
//              caller.
// Method: We are passed in each prompt to the user, so we simply do an EOF check
//         then output the title, true case explanation and false case explanation
//         before prompting the user and returning our result.
// Parameters: cReadBoolPrompt - the prompt passed in from the caller that we use 
//                               when calling the read bool function.
//             cTitle - The title to display to the user, this signifies what bool
//                      we're asking the user to manipulate.
//             cTrueCaseExplanation - A prompt that explains to the user what will
//                                    happen if they set the value to be true.
//             cFalseCaseExplanation - A prompt that explains to the user what will
//                                    happen if they set the value to be false.
//             bEOF - A boolean reference that we check to make sure we haven't
//                    encountered an end of file.
// Return Value: Returns the boolean choice of the user.
////////////////////////////////////////////////////////////////////////////////////
bool Get_Informed_Bool( const char cReadBoolPrompt[],
			const char cTitle[],
			const char cTrueCaseExplanation[],
			const char cFalseCaseExplanation[],
			bool &bEOF )
{
    // Local Variables
    bool bReturnValue = false;

    if( !bEOF )
    {
	cout << cTitle << endl;
	cout << cTrueCaseExplanation << endl;
	cout << cFalseCaseExplanation << endl << endl;
	bReturnValue = readBool( cReadBoolPrompt,
				 bEOF );

	if( bEOF )
	    ExitLine;
    }

    return bReturnValue;
}

// Description: This function is to recursively get some integer from the user.
//              The only stipulation is that we don't want negative dimensions or
//              a dimension that is 0.  If a user wants to wait for an 80000x80000
//              image to be processed that's their prerogative, even though that may
//              cause a segmentation fault, this program doesn't care.  When used to
//              grab the number of iterations, we also don't want a value <= 0.
// Method: After initializing our return variable we do a end of file check to
//         ensure we want to prompt the user at all.  This function is recursive
//         and limits the user's number of attempts at entering in a dimension.
//         Since we're required to use recursion and we don't want the possibility
//         of an infinite recursive function, we increment the iterations each time
//         the user enters in a dimension <= 0.  Once the user is out of attempts,
//         we simply set our end of file flag and exit the program.  Otherwise, if
//         the user still has attempts that he can use, we read in an int and check
//         that there were no errors in their input (end of file reached/value <= 0).
//         If there is an error, we recall this function and once we reached the max
//         iterations, we return from the stack and end the program.
// Parameters: cPrompt - A message to prompt the user when asking them for our return
//                       value.
//             bEOF - A boolean reference that we check to make sure we haven't
//                    encountered an end of file.
//             iIteration - The current iteration of the recursive function.
// Return Value: Returns the integer entered by the user.
////////////////////////////////////////////////////////////////////////////////////
int Get_Recursive_Int( const char cPrompt[], bool &bEOF, int iIteration )
{
    // Local Variables
    int iReturnValue = 0;

    if( !bEOF )
    {
	// Read input form user
	if( iIteration <= iMAX_DIMENSION_ITERATIONS )
	{
	    iReturnValue = readInt( cPrompt, bEOF );

	    // output a message if EOF reached.
	    if( bEOF )
		ExitLine;
	   
	}
	else  // too many iterations
	{
	    cout << "I'm sorry, you're out of attempts.  Exiting Program..." << endl;
	    bEOF = true;
	}

	// check to make sure input is correct
	if( !bEOF && ( iReturnValue <= 0 ) )
	{
	    cout << "I'm sorry, the dimension of the image cannot be less than or equal to 0.  ";
	    cout << "You have '" << iMAX_DIMENSION_ITERATIONS - iIteration;
	    cout << "' trys remaining.  Please Try Again!" << endl;
	    
	    iReturnValue = Get_Recursive_Int( cPrompt, bEOF, ( iIteration + 1 ) );
	}
	
    }
    
    // return the dimension.
    return iReturnValue;
}
