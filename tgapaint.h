// *************** TGAPAINT.H ***************

#ifndef TGAPAINT_H
#define TGAPAINT_H

// ================= Include =================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "bcurve.h"

// ================= Define ==================

// Maximum number of colors in a TGAPencil
#define TGA_NBCOLORPENCIL 10
// Maximum number of curves in the definition of a font's character
#define TGA_NBMAXCURVECHAR 10

// ================= Generic functions ==================

void TGATypeUnsupported(void*t, ...); 
#define TGADrawCurve(T,C,P) _Generic((C), \
  BCurve*: TGADrawBCurve, \
  SCurve*: TGADrawSCurve, \
  default: TGATypeUnsupported)(T,C,P)

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
  // Flag to memorize if this pixel is in read only mode
  bool _readOnly;
} TGAPixel;

// One layer of pixels in the TGA
typedef struct TGALayer {
  // Dimension of the layer
  VecShort *_dim;
  // Pixels (stored by rows)
  TGAPixel *_pixels;
} TGALayer;

// Main TGA structure
typedef struct TGA {
  // Header
  TGAHeader *_header;
  // Set of layers (first one is the deepest)
  GSet *_layers;
  // Current layer
  TGALayer *_curLayer;
  // Current layer index
  int _curLayerIndex;
  // Temporary working layer
  TGALayer *_tmpLayer;
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
  // Shapoid
  tgaPenShapoid,
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
  // Shapoid of the tip of the pen
  Shapoid *_tip;
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
  // SCurve defining this character
  SCurve *_curve;
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
  VecFloat *_space;
  // Scale of the characters, (x,y), multiplied to _size
  VecFloat *_scale;
  // Tabulation size, in pixel, when '\t' is printed move x to 
  // (floor(p/_tabSize)+1)*_tabSize, where p is current x position
  float _tabSize;
  // Anchor (position in the printed text corresponding to 'pos'
  // in TGAPrintString)
  tgaFontAnchor _anchor;
  // Direction to the right of the font
  VecFloat *_right;
} TGAFont;

// ================ Functions declaration ====================

// Create a TGA of width dim[0] and height dim[1] and background
// color equal to pixel
// If 'pixel' is NULL rgba(0,0,0,0) is used
// (0,0) is the bottom left corner, x toward right, y toward top
// Return NULL in case of invalid arguments or memory allocation
// failure
TGA* TGACreate(VecShort *dim, TGAPixel *pixel);

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

// Return true if 'pos' is inside 'tga'
// Return false else, or if arguments are invalid
bool TGAIsPosInside(TGA *tga, VecShort *pos);

// Get a pointer to the pixel at coord (x,y) = (pos[0],pos[1])
// in the current layer
// Return NULL in case of invalid arguments
TGAPixel* TGAGetPix(TGA *tga, VecShort *pos);

// Set the color of one pixel at coord (x,y) = (pos[0],pos[1]) to 'pix'
// in the current layer
// Do nothing in case of invalid arguments
void TGASetPix(TGA *tga, VecShort *pos, TGAPixel *pix);

// Draw one stroke at 'pos' with 'pen'
// in current layer
// Do nothing in case of invalid arguments
void TGAStrokePix(TGA *tga, VecFloat *pos, TGAPencil *pen);

// Draw a line between 'from' and 'to' with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawLine(TGA *tga, VecFloat *from, VecFloat *to, TGAPencil *pen);
  
// Draw the BCurve 'curve' (must be of dimension 2 and order > 0)
// do nothing if arguments are invalid
void TGADrawBCurve(TGA *tga, BCurve *curve, TGAPencil *pen);
  
// Draw the SCurve 'curve' (must be of dimension 2)
// do nothing if arguments are invalid
void TGADrawSCurve(TGA *tga, SCurve *curve, TGAPencil *pen);
  
// Draw a rectangle between 'from' and 'to' with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawRect(TGA *tga, VecFloat *from, VecFloat *to, TGAPencil *pen);

// Fill a rectangle between 'from' and 'to' with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGAFillRect(TGA *tga, VecFloat *from, VecFloat *to, TGAPencil *pen);

// Draw a ellipse at 'center' of radius 'r' (Rx,Ry) 
// with pencil 'pen' 
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGADrawEllipse(TGA *tga, VecFloat *center, VecFloat *r, TGAPencil *pen);

// Fill an ellipse at 'center' of radius 'r' (Rx, Ry) with pencil 'pen'
// pixels outside the TGA are ignored
// do nothing if arguments are invalid
void TGAFillEllipse(TGA *tga, VecFloat *center, VecFloat *r, TGAPencil *pen);

// Draw the shapoid 's' with pencil 'pen' 
// The shapoid must be of dimension 2
// Pixels outside the TGA are ignored
// Do nothing if arguments are invalid
void TGADrawShapoid(TGA *tga, Shapoid *s, TGAPencil *pen);

// Fill the shapoid 's' with pencil 'pen' 
// The shapoid must be of dimension 2
// Pixels outside the TGA are ignored
// Do nothing if arguments are invalid
void TGAFillShapoid(TGA *tga, Shapoid *s, TGAPencil *pen);

// Apply a gaussian blur of 'strength' and 'range' perimeter on the TGA
// Do nothing if arguments are invalid 
void TGAFilterGaussBlur(TGA *tga, float strength, float range);

// Print the string 's' with its anchor position at 'pos', TGAPencil 
// 'pen' and font 'font'
void TGAPrintString(TGA *tga, TGAPencil *pen, TGAFont *font, 
  unsigned char *s, VecFloat *pos);

// Print the char 'c' with its (bottom, left) position at 'pos'
// and (width, height) dimension 'dim' with font 'font'
void TGAPrintChar(TGA *tga, TGAPencil *pen, TGAFont *font, 
  unsigned char c, VecFloat *pos);
  
// Get a white TGAPixel
TGAPixel* TGAGetWhitePixel(void);

// Get a black TGAPixel
TGAPixel* TGAGetBlackPixel(void);

// Get a transparent TGAPixel
TGAPixel* TGAGetTransparentPixel(void);

// Free the memory used by tgapixel
void TGAPixelFree(TGAPixel **pixel);

// Return a new TGAPixel which is a blend of 'pixA' and 'pixB' 
// newPix = (1 - blend) * pixA + blend * pixB
// Return NULL if arguments are invalid
TGAPixel* TGAPixelBlend(TGAPixel *pixA, TGAPixel *pixB, float blend);

// Return a new TGAPixel which is the addition of 'ratio' 
// (in [0.0,1.0]) * 'pixB' to 'pixA' 
// Return NULL if arguments are invalid
TGAPixel* TGAPixelMix(TGAPixel *pixA, TGAPixel *pixB, float ratio);

// Create a default TGAPencil with all color set to transparent
// solid mode, thickness = 1.0, tip as facoid, no antialias
// Return NULL if it couldn't allocate memory
TGAPencil* TGAGetPencil(void);

// Free the memory used by the TGAPencil 'pen'
void TGAPencilFree(TGAPencil **pen);

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

// Get a TGAPixel equal to the active color of the TGAPencil 'pen'
// Return NULL if arguments are invalid
TGAPixel* TGAPencilGetPixel(TGAPencil *pen);

// Get the 

// Set the active color of TGAPencil 'pen' to TGAPixel 'col'
// Do nothing if arguments are invalid
void TGAPencilSetColor(TGAPencil *pen, TGAPixel *col);

// Set the active color of TGAPencil 'pen' to 'rgba'
// Do nothing if arguments are invalid
void TGAPencilSetColRGBA(TGAPencil *pen, unsigned char *rgba);

// Set the thickness of TGAPencil 'pen' to 'v'
// Equivalent to a scale of the shapoid of the tip
// Do nothing if arguments are invalid
void TGAPencilSetThickness(TGAPencil *pen, float v);

// Set the antialias of the TGAPencil 'pen' to 'v'
// Do nothing if arguments are invalid
void TGAPencilSetAntialias(TGAPencil *pen, bool v);

// Set the blend value 'v' of the TGAPencil 'pen'
// Do nothing if arguments are invalid
void TGAPencilSetBlend(TGAPencil *pen, float v);

// Set the shape of the TGAPencil 'pen' to 'tgaPenShapoid' and 
// set the tip of the pen to a new facoid centered on the origin
// and scaled to the pen thickness
// Do nothing if arguments are invalid
void TGAPencilSetShapeSquare(TGAPencil *pen);

// Set the shape of the TGAPencil 'pen' to 'tgaPenShapoid' and
// set the tip of the pen to a new ellipsoid scaled to the pen thickness
// Do nothing if arguments are invalid
void TGAPencilSetShapeRound(TGAPencil *pen);

// Set the shape of the TGAPencil 'pen' to 'tgaPenShapoid' and
// set the tip of the pen to a clone of the Shapoid 'shape'
// 'shape' is considered to be centered and given at a thickness 
// of 1.0 before rescaling to 'pen' thickness
// Do nothing if arguments are invalid
void TGAPencilSetShapeShapoid(TGAPencil *pen, Shapoid *shape);

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
// _dir = <1.0, 0.0>, _tabSize = _fontSize
// Return NULL if it couldn't create
TGAFont* TGAFontCreate(tgaFont font);

// Free memory used by TGAFont
// Do nothing if arguments are invalid
void TGAFreeFont(TGAFont **font);

// Set the font size of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSize(TGAFont *font, float v);

// Set the font tab size of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetTabSize(TGAFont *font, float v);

// Set the font scale of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetScale(TGAFont *font, VecFloat *v);

// Set the font spacing of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSpace(TGAFont *font, VecFloat *v);

// Set the anchor of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetAnchor(TGAFont *font, tgaFontAnchor v);

// Set the right direction of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetRight(TGAFont *font, VecFloat *v);

// Get the bounding box as a facoid of order 2 and dim 2 in pixels
// of the block of text representing string 's' printed with 'font'
// Return NULL if arguments are invalid
Shapoid* TGAFontGetStringBound(TGAFont *font, unsigned char *s);

// Get the angle of the right vector of the font with the abciss
// Return 0.0 if the arguments are invalid or memory allocation failed
float TGAFontGetAngleWithAbciss(TGAFont *font);

// Get the average color of the whole image
// Return a TGAPixel set to the avergae color, or NULL if the arguments
// are invalid
TGAPixel *TGAGetAverageColor(TGA *tga);

// Set the read only flag of a TGAPixel
// Do nothing if arguments are invalid
void TGAPixelSetReadOnly(TGAPixel *pix, bool v);

// Set the read only flag of all the TGAPixel of a TGA
// Do nothing if arguments are invalid
void TGAPixelSetAllReadOnly(TGA *tga, bool v);

// Get the read only flag of a TGAPixel
// Return true if arguments are invalid
bool TGAPixelIsReadOnly(TGAPixel *pix);

// Create a TGALayer of width dim[0] and height dim[1] and background
// color equal to 'pixel'
// If 'pixel' is NULL rgba(0,0,0,0) is used
// Return NULL in case of invalid arguments or memory allocation
// failure
TGALayer* TGALayerCreate(VecShort *dim, TGAPixel *pixel);

// Clone a TGALayer
// Return NULL in case of failure
TGALayer* TGALayerClone(TGALayer *that);

// Free the memory used by the TGALayer
void TGALayerFree(TGALayer **that);

// Set the current layer to the 'iLayer'-th layer
// Do nothing if arguments are invalid
void TGASetCurLayer(TGA *that, int iLayer);

// Add a layer above the current one
// Do nothing if the arguments are invalid
void TGAAddLayer(TGA *that);

// Blend layers 'that' and 'tho', the result is stored into 'that'
// 'tho' is considered to above 'that'
// If VecShort 'bound' is not null only pixels inside the box
// (bound[0],bound[1])-(bound[2],bound[3]) (included) are blended
// 'that' and 'tho' must have same dimension
// Do nothing if arguments are invalid
void TGALayerBlend(TGALayer *that, TGALayer *tho, VecShort *bound);

// Get a pointer to the pixel at coord (x,y) = (pos[0],pos[1]) 
// in the layer 'that'
// Return NULL in case of invalid arguments
TGAPixel* TGALayerGetPix(TGALayer *that, VecShort *pos);

// Set the color of one pixel at coord (x,y) = (pos[0],pos[1]) to 'pix'
// in the layer 'that'
// Do nothing in case of invalid arguments
void TGALayerSetPix(TGALayer *that, VecShort *pos, TGAPixel *pix);

// Draw one stroke at 'pos' with 'pen'
// in layer 'that'
// Do nothing in case of invalid arguments
void TGALayerStrokePix(TGALayer *that, VecFloat *pos, TGAPencil *pen);

// Return true if 'pos' is inside 'that'
// Return false else, or if arguments are invalid
bool TGALayerIsPosInside(TGALayer *that, VecShort *pos);

// Erase the content of the layer 'that' 
// (set all pixel to rgba(0,0,0,0) and readonly to false)
// Do nothing in case of invalid argument
void TGALayerClean(TGALayer *that); 

#endif
