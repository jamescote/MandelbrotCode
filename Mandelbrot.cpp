// Name: Mandelbrot.cpp
// Description: Module implementation of the Mandelbrot Image making module.
// Known Issues: Unsure how to get smoothing to work.  Don't entirely understand
//               the algorithm and, therefore, it isn't implemented.
// Written By: James Coté
////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include "Mandelbrot.h"
#include <Magick++.h>
#include <complex>
#include <math.h>
#include <iostream>

// Namespaces
using namespace Magick;
using namespace std;

// CONSTANTS
const int iPROGRESS_BAR_SIZE = 80;

// Bounds of our complex Plane
const float fCXMIN = -2.5f;
const float fCXMAX = 1.0f;
const float fCYMIN = -1.0f;
const float fCYMAX = 1.0f;

// Description: Recursive function that outputs a Progress Bar based on a
//              desired size constant and a percentage of completion of
//              compiling the image.
// Method: If it's the first iteration (special case), we output the start of
//         the progress bar: '['.  For each subsequent iteration we determine if
//         we output a space or a bar character depending on how much of the
//         image we have left to fill.  This is modified based on the desired
//         size of the bar to ensure the percentage complete is accurate,
//         regardless of the bar size.  This function will iterate iPROGRESS_BAR
//         _SIZE times and will output the closing ']' on the last iteration.
//         NOTE: This function is only to output the Bar, it doesn't output the
//         progress information that precedes the bar.  We also end our output
//         with the character '\r' to reset our cursor to the beginning of the
//         buffer.  We then flush the buffer to ensure it's properly cleared.
// Parameters: iPercent - Integer that tells us how much of the image we have
//                        completed.  Assumed to be already truncated to be
//                        relative to the size of the progress bar when passed
//                        in.
//             iIterations - Recursive flag.  When we recursively call this 
//                           function again, we add one to this variable and 
//                           exit back through the stack once the iterations ==
//                           the size of the progress bar.
////////////////////////////////////////////////////////////////////////////////
void Output_Progress_Bar( int iPercent, int iIterations = 0 )
{
    // Special case for start of bar
    if( iIterations == 0 )
	cout << '[';

    // each node: determine whether we output a Bar character or a space.
    if( iPercent >= iIterations )
	cout << (char)(219);
    else
	cout << ' ';

    // Recursive Check
    if( iIterations < iPROGRESS_BAR_SIZE )
	Output_Progress_Bar( iPercent, ( iIterations + 1 ) );
    else
    {
	cout << "]\r";
	cout.flush();
    }
}

// Description: Isolated function to output progress text then call the
//              recursive "Output_Progress_Bar" function to visually show the
//              current progress of the program.
// Method: Fairly simple.  Output the total Progress as a percent then call on
//         the Output_Progress_Bar function to recursively draw the visual
//         progress of the program.
// Parameters: iPercentComplete - True percentage of completion.  We output this
//                                to the user then pass it into the Progress Bar
//                                function.
////////////////////////////////////////////////////////////////////////////////
void Show_Progress( int iPercentComplete )
{
    cout << "Progress: " << iPercentComplete << "%";
    Output_Progress_Bar( ( iPercentComplete * iPROGRESS_BAR_SIZE ) / 100 );
}

// Description: Inverts the color values of a provided ColorRGB object.  Used
//              multiple times in the program.
// Method: Take the colors and subtract them from 1.0f to get the opposite value
//         (.70 -> .30, 1.0 -> 0.0, etc.).
// Parameters: Color - A ColorRGB object that's passed by reference.  Each value
//                     of the object is manipulated then the function returns.
////////////////////////////////////////////////////////////////////////////////
void Invert_Colors( ColorRGB &Color )
{
    Color.red( 1.0f - Color.red() );
    Color.green( 1.0f - Color.green() );
    Color.blue( 1.0f - Color.blue() );
}

// Description: Takes a calculated weight that's to be used for the RGB values
//              of the pixel and applies any Color Filters that were specified
//              by the user.
// Method: First, start by initializing a local ColorRGB variable that is to be
//         passed back from the function after all the filters have been applied.
//         Second, apply any filters.  If a Grey Scale has been specified, don't
//         apply any filters since each RGB value has been set to the same weight.
//         Otherwise, apply the masks specified by the user (Default: 1.0f) then,
//         if the user wanted the colors inverted, take the current weight and
//         subtract it from 1.0f to get the opposite weight.
// Parameters: fColorWeight - The weight of each RGB value as determined by the
//                            escape time algorithm.
//             sColor - a constant reference to our Color Filters that were
//                      specified by the user.
// Return Value: Returns a ColorRGB variable for the current pixel.
////////////////////////////////////////////////////////////////////////////////
ColorRGB Parse_Color( float fColorWeight, 
		      const sColorCode &sColor )
{
    // Local Variables
    ColorRGB ReturnColor = ColorRGB( fColorWeight, 
				     fColorWeight, 
				     fColorWeight );
    
    // Apply Filters
    if( sColor.bInvertColors )
    {
	Invert_Colors( ReturnColor );
    }    

    if( !sColor.bGreyScale )
    {
	ReturnColor.red( ReturnColor.red() *  sColor.fRGBMask[ 0 ] );
	ReturnColor.green( ReturnColor.green() *  sColor.fRGBMask[ 1 ] );
	ReturnColor.blue( ReturnColor.blue() *  sColor.fRGBMask[ 2 ] );
    }	
    
    if( sColor.bInvertSpectrum )
    {
	Invert_Colors( ReturnColor );
    }
    
    return ReturnColor;
}

// Description: Implements the recursive logic of determining the Mandelbrot set
//              z -> z^2 + c where z and c are complex numbers.  c is the distance
//              from the center of the Mandelbrot set and z is the iterating 
//              number to determine if we've exited the bounds of the set.  We
//              determine the color of the pixel based on how many iterations we
//              called to get out of the set.
// Method: Since c and z are set in the caller, we can simply check if we are
//         still within the bounds of the Mandelbrot set.  Once we reach max
//         iterations or we escape outside the Mandelbrot set (abs(z) < 2.0f),
//         we can then determine the weight of color on the pixel and call the
//         Parse Color function to apply any color filters set by the user.
// Parameters: c - complex number that contains the distance from the center of
//                 the Mandelbrot set.
//             z - complex number that helps us check when we've escaped the
//                 Mandelbrot set.
//             iterations - The number of recursive iterations we've done.
//             sColor - a Constant reference to our color filters, specified
//                      by the user. 
// Return Value: Returns a ColorRGB variable for the current pixel.
////////////////////////////////////////////////////////////////////////////////
ColorRGB Get_Pixel_Color( complex< float > c, 
			  complex< float > z, 
			  int iterations,
			  const int iMax_Iterations,
			  const sColorCode &sColor )
{
    // Local Variables
    ColorRGB ReturnColor = ColorRGB( 0.0, 0.0, 0.0 );

    // Iterate until we've escaped the Mandelbrot set.
    if( ( iterations < iMax_Iterations ) && ( std::abs(z) < 2.0f ) )
	ReturnColor = Get_Pixel_Color( c, 
				       ( (z*z) + c ), 
				       ( iterations + 1 ), 
				       iMax_Iterations,
				       sColor );
    else if( iterations == iMax_Iterations )
	ReturnColor = Parse_Color( 1.0, sColor );
    else
	ReturnColor = Parse_Color( ( (float)(iterations) / 
				     (float)(iMax_Iterations) ), 
				   sColor );

    // Return the parsed color of the pixel.
    return ReturnColor;
}

// Description: Part 2 of the recursion algorithm that follows down the Y axis
//              for each x in the X-axis and draws each pixel.  This function
//              calls the Show_Progress function as well as the Get_Pixel_Color
//              function.
// Method: It starts by generating a complex c variable based on the current 
//         pixel being looked at, the maximum size of the image and the bounds
//         of the complex plane.  it also sets a pixel color variable that grabs
//         the pixel color that's parsed from the Get_Pixel_Color function, 
//         assuming that we haven't reached the bottom of the image.  Otherwise,
//         we call this function again and increment the current Y value until we
//         get to the bottom.  Then we follow back through the function calls to
//         the draw image function.
// Parameters: magImage - A reference to our Image file that we will use to draw
//                        the pixels and write the image.
//             fCurrentX - A floating point value of the x position of the
//                         current pixel. 
//             fCurrentY - A floating point value of the y position of the 
//                         current pixel.
//             fMaxX - The maximum width of the image.  Since we are working with
//                     a 0-based algorithm, we need to use fMaxX - 1.0f when
//                     doing comparisons to the current x value.
//             fMaxY - The maximum height of the image.  Since we are working
//                     with a 0-based algorithm, we need to use fMaxY - 1.0f when
//                     doing comparisons to the current y value.
//             sColor - A constant reference to our color filters, specified
//                      by the user. 
////////////////////////////////////////////////////////////////////////////////
void Draw_Y_Line( Image &magImage, 
		  const float fCurrentX, 
		  const float fCurrentY, 
		  const float fMaxX, 
		  const float fMaxY,
		  const int iMax_Iterations,
		  const sColorCode &sColor )
{
    // Local Variables
    std::complex< float > c( ( fCXMIN + fCurrentX / ( fMaxX - 1.0f ) * ( fCXMAX - fCXMIN ) ),
			     ( fCYMIN + fCurrentY / ( fMaxY - 1.0f ) * ( fCYMAX - fCYMIN ) ) );
    ColorRGB LocalPixelColor;

    // Show our Progress.
//    Show_Progress( ( ( fCurrentY + ( ( fCurrentX - 1 ) * fMaxY ) ) / ( fMaxY * fMaxX ) ) * 100 );

    if( fCurrentY != fMaxY )
    {
	// Draw Pixel Logic
	LocalPixelColor = Get_Pixel_Color( c, 0, 0, iMax_Iterations, sColor );
	magImage.pixelColor( fCurrentX, fCurrentY, LocalPixelColor );
    }

    // Recursive condition.
    if( fCurrentY < (fMaxY - 1.0f) )
	Draw_Y_Line( magImage, 
		     fCurrentX, 
		     (fCurrentY + 1.0f), 
		     fMaxX, 
		     fMaxY, 
		     iMax_Iterations,
		     sColor );
}

// Description: Part 1 of the Drawing Algorithm.  This function draws all the
//              pixels in the Y axis for each position of X. 
// Method: We have one special case in which we don't want to draw a line when
//         we've reached the bounds of X.  Otherwise, we call the Draw_Y_Line
//         function that will draw each pixel on the Y axis for the current
//         value of X.  After we've drawn the line, we check our recursive 
//         condition to ensure we're not at the last value of X (in a 0-based
//         mapping) and if we aren't, we'll recursively call this function again
//         and pass in the currentX + 1.0f as our iterator.
// Parameters: magImage - A reference to our Image file that we will use to draw
//                        the pixels and write the image.
//             fCurrentX - A floating point value of the x position of the
//                         current pixel. 
//             fMaxX - The maximum width of the image.  Since we are working with
//                     a 0-based algorithm, we need to use fMaxX - 1.0f when
//                     doing comparisons to the current x value.
//             fMaxY - The maximum height of the image.  Since we are working
//                     with a 0-based algorithm, we need to use fMaxY - 1.0f when
//                     doing comparisons to the current y value.
//             sColor - A constant reference to our color filters, specified
//                      by the user. 
////////////////////////////////////////////////////////////////////////////////
void Draw_Image( Image &magImage, 
		 const float fCurrentX, 
		 const float fMaxX, 
		 const float fMaxY,
		 const int iMax_Iterations,
		 const sColorCode &sColor )
{
    // Local Variables
    if( fCurrentX != fMaxX )
	Draw_Y_Line( magImage, fCurrentX, 0.0f, fMaxX, fMaxY, iMax_Iterations, sColor );
    
    if( fCurrentX < (fMaxX - 1) )
	Draw_Image( magImage, (fCurrentX + 1.0f), fMaxX, fMaxY, iMax_Iterations, sColor );

    // return
    return;
}

// Description: Creates a mandelbrot image.  Saves it into a file with the
//              provided file name and sizes the image to the provided width
//              and height.
// Method: This is our main interface with the caller.  We first create a new
//         Image, set the bounds of the Geometry of the image (width & height),
//         we then Draw the image using the Escape Time Algorithm.  After the 
//         image is drawn, we output a completion prompt along with a buffer of
//         space to clear any of the progress bar we're outputting over and we
//         write the image to a specified file.
// Parameters: cFileName[] - the name of the file to save the image to.
//             iWidth - the desired width of the image.
//             iHeight - the desired height of the image.
//             sColor - A constant reference to the color filters specified from
//                      the user.
////////////////////////////////////////////////////////////////////////////////
void Create_Image( char cFileName[], 
		   const int iWidth, 
		   const int iHeight,
		   const int iMax_Iterations,
		   const sColorCode &sColor )
{
    // Local Variables
    Image magNewImage;
    
    // set up new image
    magNewImage.extent( Geometry( iWidth, iHeight ) );

    // Call recursive function to draw mandelbrot image
    Draw_Image( magNewImage, 
		0.0f, 
		(float)(iWidth), 
		(float)(iHeight),
		iMax_Iterations,
		sColor );

    // Output Completion
    cout << "Process Complete!" << string( iPROGRESS_BAR_SIZE, ' ' ) << "\n";

    // Write the image.
    magNewImage.write( cFileName );
}
