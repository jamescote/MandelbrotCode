// Name: Mandelbrot.h
// Description: Header for the mandelbrot image making module.
// Written By: James Coté
////////////////////////////////////////////////////////////////////////////////

#ifndef MANDELBROT_H
#define MANDELBROT_H

// Enum to easily identify the different RGB values in the fRGBMask array.
enum eColorCodes
{
    eRGB = 3,
    eBLUE = 2,
    eGREEN = 1,
    eRED = 0
};

// COLOR FILTER STRUCTURE
// Parts: fRGBMask[] - a Mask that filters each RGB % that the user can set to
//                     modify the colors of the image.
//        bInvertColors - a Boolean that, when set, will have the colors become
//                        on the opposite side of the spectrum (1.0 - currentMask)
//        bGreyScale - Makes the RGB values uniform so that each pixel is on a 
//                     scale from Black to White.
////////////////////////////////////////////////////////////////////////////////
struct sColorCode
{
    float fRGBMask[ eRGB ];
    bool bInvertColors;
    bool bInvertSpectrum;
    bool bGreyScale;
};

// FUNCTION DECLARATIONS
void Create_Image( char cFileName[], 
		   const int iWidth, 
		   const int iHeight, 
		   const int iMax_Iterations,
		   const sColorCode &sColor );

#endif
