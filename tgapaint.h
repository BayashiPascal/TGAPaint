// *************** TGAPAINT.H ***************

#ifndef TGAPAINT_H
#define TGAPAINT_H

// ================= Include =================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

// ================= Define ==================

// Maximum number of colors in a TGAPencil
#define TGA_NBCOLORPENCIL 10
// Maximum number of curves in the definition of a font's character
#define TGA_NBMAXCURVECHAR 10

// ================= Data structure ===================

// Header of a TGA file
typedef struct TGAHeader {
  // Origin of the color map
  short int _colorMapOrigin;
  // Length of the color map
  short int _colorMapLength;
  // X coordinate of the origin
  short int _xOrigin;
  // Y coordinate of the origin
  short int _yOrigin;
  // Width of the TGA
  short _width;
  // Height of the TGA
  short _height;
  // Length of a string located located after the header
  char _idLength;
  // Type of the color map
  char _colorMapType;
  // Type of the image
  char _dataTypeCode;
  // Depth of the color map
  char _colorMapDepth;
  // Number of bit per pixel
  char _bitsPerPixel;
  // Image descriptor
  char _imageDescriptor;
} TGAHeader;

// One pixel of the TGA
typedef struct TGAPixel {
  // RGB and transparency values
  unsigned char _rgba[4];
} TGAPixel;

// Main TGA structure
typedef struct TGA {
  // Header
  TGAHeader *_header;
  // Pixels (stored by rows)
  TGAPixel *_pixels;
} TGA;

// Enumeration of TGAPencil's color modes
typedef enum tgaPencilModeColor {
  // Constant color
  tgaPenSolid, 
  // Blend between two colors
  tgaPenBlend
} tgaPencilModeColor;

// Enumeration of TGAPencil's shapes
typedef enum tgaPencilShape {
  // Square shape
  tgaPenSquare, 
  // Round shape
  tgaPenRound,
  // Pixel mode
  tgaPenPixel
} tgaPencilShape;

// Pencil to draw on a TGA
typedef struct TGAPencil {
  // List of available colors in this pencil
  TGAPixel _colors[TGA_NBCOLORPENCIL];
  // Currently active color (index in _colors)
  int _activeColor;
  // Current color mode
  tgaPencilModeColor _modeColor;
  // Current shape
  tgaPencilShape _shape;
  // The 2 colors used when color mode is tgaPenBlend (index in _colors)
  int _blendColor[2];
  // Parameter cotnroling the blend when color mode is tgaPenBlend
  // (0.0 -> _blendColor[0], 1.0 -> _blendColor[1])
  float _blend;
  // Thickness of the TGAPencil, in pixel
  float _thickness;
  // Apply antialiasing if true
  bool _antialias;
} TGAPencil;

// One character in a TGAFont
typedef struct TGAChar {
  // Number of curve defining this character
  int _nbCurve;
  // Definition of the curves 
  // (1st anchor(x,y), 1st ctrl point(x,y), 
  // 2nd ctrl point(x,y), 2nd anchor(x,y))
  // in pixels
  float _curve[TGA_NBMAXCURVECHAR * 8];
} TGAChar;

// Enumeration of available fonts
typedef enum tgaFont {
  // Default font
  tgaFontDefault
} tgaFont;

// Enumeration of available anchor position for fonts
typedef enum tgaFontAnchor {
  tgaFontAnchorTopLeft, tgaFontAnchorTopCenter, tgaFontAnchorTopRight, 
  tgaFontAnchorCenterLeft, tgaFontAnchorCenterCenter,
  tgaFontAnchorCenterRight, tgaFontAnchorBottomLeft,
  tgaFontAnchorBottomCenter, tgaFontAnchorBottomRight 
} tgaFontAnchor;

// Font to write on the TGA
typedef struct TGAFont {
  // Size in pixel of one character
  float _size;
  // Definition of the characters
  TGAChar _char[256];
  // Space between character, (x,y), in pixel
  // _space[0] is added to x after each character in a string
  // _space[1] is added to y when '\n' is printed
  float _space[2];
  // Scale of the characters, (x,y), multiplied to _size
  float _scale[2];
  // Tabulation size, in pixel, when '\t' is printed move x to 
  // (floor(p/_tabSize)+1)*_tabSize, where p is current x position
  float _tabSize;
  // Anchor (position in the printed text corresponding to 'pos'
  // in TGAPrintString)
  tgaFontAnchor _anchor;
} TGAFont;

// ================ Functions declaration ====================

// Create a TGA of width dim[0] and height dim[1] and background
// color equal to pixel
// (0,0) is the bottom left corner, x toward right, y toward top
// Return NULL in case of invalid arguments or memory allocation
// failure
TGA* TGACreate(short *dim, TGAPixel *pixel);

// Clone a TGA
// Return NULL in case of failure
TGA* TGAClone(TGA *tga);

// Free the memory used by the TGA
void TGAFree(TGA **tga);

// Load a TGA from the file pointed to by 'fileName'
// If 'tga' already contains a TGA, it is overwritten
// return 0 upon success, else
// 1 : couldn't open the file
// 2 : malloc failed
// 3 : can only handle image type 2 and 10
// 4 : can only handle pixel depths of 16, 24, and 32
// 5 : can only handle colour map types of 0 and 1
// 6 : unexpected end of file
// 7 : invalid arguments
int TGALoad(TGA **tga, char *fileName);

// Save the TGA 'tga' to the file pointed to by 'fileName'
// return 0 upon success, else
// 1 : couldn't open the file
// 2 : invalid arguments
int TGASave(TGA *tga, char *fileName);

// Print the header of 'tga' on 'stream'
// If arguments are invalid, do nothing
void TGAPrintHeader(TGA *tga, FILE *stream);

// Get a pointer to the pixel at coord (x,y) = (pos[0],pos[1])
// Return NULL in case of invalid arguments
TGAPixel* TGAGetPix(TGA *tga, short *pos);

// Set the color of one pixel at coord (x,y) = (pos[0],pos[1]) to 'pix'
// Do nothing in case of invalid arguments
void TGASetPix(TGA *tga, short *pos, TGAPixel *pix);

// Draw one stroke at 'pos' with 'pen'
// Don't do anything in case of invalid arguments
void TGAStrokePix(TGA *tga, float *pos, TGAPencil *pen);

// Draw a line between 'from' and 'to' with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawLine(TGA *tga, float *from, float *to, TGAPencil *pen);
  
// Draw a curve between 'from' and 'to' with pencil 'pen'
// and control points 'ctrlFrom' and 'ctrlTo'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawCurve(TGA *tga, float *from, float *ctrlFrom, 
  float *ctrlTo, float *to, TGAPencil *pen);
  
// Draw a rectangle between 'from' and 'to' with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawRect(TGA *tga, float *from, float *to, TGAPencil *pen);

// Fill a rectangle between 'from' and 'to' with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGAFillRect(TGA *tga, float *from, float *to, TGAPencil *pen);

// Draw a ellipse at 'center' of radius 'r' (Rx,Ry) 
// with pencil 'pen' 
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawEllipse(TGA *tga, float *center, float *r, TGAPencil *pen);

// Fill an ellipse at 'center' of radius 'r' (Rx, Ry) with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGAFillEllipse(TGA *tga, float *center, float *r, TGAPencil *pen);

// Apply a gaussian blur of 'strength' and 'range' perimeter on the TGA
// Do nothing if arguments are invalid 
void TGAFilterGaussBlur(TGA *tga, float strength, float range);

// Print the string 's' with its (bottom, left) position at 'pos'
// and (width, height) dimension 'dim' with font 'font'
void TGAPrintString(TGA *tga, TGAPencil *pen, TGAFont *font, 
  unsigned char *s, float *pos);

// Print the char 'c' with its (bottom, left) position at 'pos'
// and (width, height) dimension 'dim' with font 'font'
void TGAPrintChar(TGA *tga, TGAPencil *pen, TGAFont *font, 
  unsigned char c, float *pos);
  
// Get a white TGAPixel
TGAPixel* TGAGetWhitePixel(void);

// Get a black TGAPixel
TGAPixel* TGAGetBlackPixel(void);

// Get a transparent TGAPixel
TGAPixel* TGAGetTransparentPixel(void);

// Free the memory used by tgapixel
void TGAFreePixel(TGAPixel **pixel);

// Return a new TGAPixel which is a blend of 'pixA' and 'pixB' 
// newPix = (1 - blend) * pixA + blend * pixB
// Return NULL if arguments are invalid
TGAPixel* TGABlendPixel(TGAPixel *pixA, TGAPixel *pixB, float blend);

// Create a default TGAPencil with all color set to transparent
// solid mode, thickness = 1.0, square shape, no antialias
// Return NULL if it couldn't allocate memory
TGAPencil* TGAGetPencil(void);

// Free the memory used by the TGAPencil 'pen'
void TGAFreePencil(TGAPencil **pen);

// Clone the TGAPencil 'pen'
// Return NULL if it couldn't clone
TGAPencil* TGAPencilClone(TGAPencil *pen);

// Create a TGAPencil with 1st color active and set to black
// Return NULL if it couldn't create
TGAPencil* TGAGetBlackPencil(void);

// Select the active color of TGAPencil 'pen' to 'iCol'
// Do nothing if arguments are invalid
void TGAPencilSelectColor(TGAPencil *pen, int iCol);

// Get the index of active color of TGAPencil 'pen'
// Return -1 if arguments are invalid
int TGAPencilGetColor(TGAPencil *pen);

// Get the active color of the TGAPencil 'pen'
// Return NULL if arguments are invalid
TGAPixel* TGAPencilGetPixel(TGAPencil *pen);

// Set the active color of TGAPencil 'pen' to TGAPixel 'col'
// Do nothing if arguments are invalid
void TGAPencilSetColor(TGAPencil *pen, TGAPixel *col);

// Set the active color of TGAPencil 'pen' to 'rgba'
// Do nothing if arguments are invalid
void TGAPencilSetColRGBA(TGAPencil *pen, unsigned char *rgba);

// Set the thickness of TGAPencil 'pen' to 'v'
// Do nothing if arguments are invalid
void TGAPencilSetThickness(TGAPencil *pen, float v);

// Set the antialias of the TGAPencil 'pen' to 'v'
// Do nothing if arguments are invalid
void TGAPencilSetAntialias(TGAPencil *pen, bool v);

// Set the blend value 'v' of the TGAPencil 'pen'
// Do nothing if arguments are invalid
void TGAPencilSetBlend(TGAPencil *pen, float v);

// Set the shape of the TGAPencil 'pen' to 'tgaPenSquare'
// Do nothing if arguments are invalid
void TGAPencilSetShapeSquare(TGAPencil *pen);

// Set the shape of the TGAPencil 'pen' to 'tgaPenRound'
// Do nothing if arguments are invalid
void TGAPencilSetShapeRound(TGAPencil *pen);

// Set the shape of the TGAPencil 'pen' to 'tgaPenPixel'
// Do nothing if arguments are invalid
void TGAPencilSetShapePixel(TGAPencil *pen);

// Set the mode of the TGAPencil 'pen' to 'tgaPenSolid'
// Do nothing if arguments are invalid
void TGAPencilSetModeColorSolid(TGAPencil *pen);

// Set the mode of the TGAPencil 'pen' to 'tgaPenBlend'
// Blend is done from 'fromCol' to 'toCol'
// Do nothing if arguments are invalid
void TGAPencilSetModeColorBlend(TGAPencil *pen, int fromCol, int toCol);

// Create a TGAFont with set of character 'font', 
// _fontSize = 18.0, _space[0] = _space[1] = 3.0, 
// _scale[0] = 0.5, _scale[1] = 1.0, _anchor = tgaFrontAnchorTopLeft
// Return NULL if it couldn't create
TGAFont* TGAFontCreate(tgaFont font);

// Free memory used by TGAFont
// Do nothing if arguments are invalid
void TGAFreeFont(TGAFont **font);

// Set the font size of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSize(TGAFont *font, float v);

// Set the font scale of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetScale(TGAFont *font, float *v);

// Set the font spacing of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSpace(TGAFont *font, float *v);

// Set the anchor of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetAnchor(TGAFont *font, tgaFontAnchor v);

// Get the dimension in pixels of the block of text representing 
// string 's' printed with 'font'
// Return the dimension in float[2] 'dim', return {-1, -1} if arguments
// are invalid
void TGAFontGetStringSize(TGAFont *font, unsigned char *s, float *dim);

// Get the average color of the whole image
// Return a TGAPixel set to the avergae color, or NULL if the arguments
// are invalid
TGAPixel *TGAGetAverageColor(TGA *tga);

#endif
