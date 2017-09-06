#include <stdio.h>
#include <stdlib.h>
#include "tgapaint.h"

int main(void) {
  int ret;
  TGA *theTGA;
  // Create the TGA
  short dim[2] = {120, 270};
  TGAPixel *pix = TGAGetWhitePixel();
  theTGA = TGACreate(dim, pix);
  if (theTGA == NULL) {
    fprintf(stderr, "Error while creating the tga\n");
    return 1;
  }
  // Set the color of some pixels
  short pos[2];
  pos[0] = 60; pos[1] = 50;
  TGASetPix(theTGA, pos, pix);
  pix->_rgba[0] = 255; pix->_rgba[1] = 0; pix->_rgba[2] = 0;
  pos[0] = 90; pos[1] = 50;
  TGASetPix(theTGA, pos, pix);
  pix->_rgba[0] = 0; pix->_rgba[1] = 0; pix->_rgba[2] = 255;
  pos[0] = 60; pos[1] = 25;
  TGASetPix(theTGA, pos, pix);
  pix->_rgba[0] = 0; pix->_rgba[1] = 255; pix->_rgba[2] = 0;
  pos[0] = 30; pos[1] = 75;
  TGASetPix(theTGA, pos, pix);
  // Draw some lines
  TGAPencil *pen = TGAGetBlackPencil();
  pix->_rgba[0] = 0; pix->_rgba[1] = 0; pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  float from[2];
  float to[2];
  from[0] = 50.5; from[1] = 40.5; to[0] = 50.5; to[1] = 60.5;
  TGADrawLine(theTGA, from, to, pen);
  from[0] = 50.5; from[1] = 60.5; to[0] = 70.5; to[1] = 60.5;
  TGADrawLine(theTGA, from, to, pen);
  pix->_rgba[0] = 255; pix->_rgba[1] = 0; pix->_rgba[2] = 255;
  from[0] = -10.5; from[1] = 50.5; to[0] = 60.5; to[1] = -10.5;
  TGADrawLine(theTGA, from, to, pen);
  from[0] = 60.5; from[1] = -10.5; to[0] = 130.5; to[1] = 50.5;
  TGADrawLine(theTGA, from, to, pen);
  from[0] = 130.5; from[1] = 50.5; to[0] = 60.5; to[1] = 110.5;
  TGADrawLine(theTGA, from, to, pen);
  from[0] = 60.5; from[1] = 110.5; to[0] = -10.5; to[1] = 50.5;
  TGADrawLine(theTGA, from, to, pen);
  // Apply gaussian blur
  TGAFilterGaussBlur(theTGA, 0.5, 2.0);
  // Draw a rectangle
  pix->_rgba[0] = 0; pix->_rgba[1] = 255; pix->_rgba[2] = 255;
  TGAPencilSetColor(pen, pix);
  from[0] = 70.5; from[1] = 40.5; to[0] = 100.5; to[1] = 10.5;
  TGADrawRect(theTGA, from, to, pen);
  // Draw a filled rectangle
  pix->_rgba[0] = 255; pix->_rgba[1] = 255; pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  from[0] = 75.5; from[1] = 35.5; to[0] = 95.5; to[1] = 15.5;
  TGAFillRect(theTGA, from, to, pen);
  // Draw an ellipse
  pix->_rgba[0] = 128; pix->_rgba[1] = 128; pix->_rgba[2] = 128;
  TGAPencilSetColor(pen, pix);
  float center[2] = {30.5, 50.5};
  float radius[2] = {15.5, 20.5};
  TGADrawEllipse(theTGA, center, radius, pen);
  // Draw a filled ellipse
  pix->_rgba[0] = 200; pix->_rgba[1] = 200; pix->_rgba[2] = 200;
  TGAPencilSetColor(pen, pix);
  center[0] = 60.5; center[1] = 75.5;
  radius[0] = 25.5; radius[1] = 10.5;
  TGAFillEllipse(theTGA, center, radius, pen);
  // Draw a line using blend colors
  from[0] = 30.5; from[1] = 25.5; to[0] = 90.5; to[1] = 75.5;
  pix->_rgba[0] = pix->_rgba[3] = 255;
  pix->_rgba[1] = pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  pix->_rgba[2] = pix->_rgba[3] = 255;
  pix->_rgba[1] = pix->_rgba[0] = 0;
  TGAPencilSelectColor(pen, 1);
  TGAPencilSetColor(pen, pix);
  TGAPencilSetModeColorBlend(pen, 0, 1);
  TGADrawLine(theTGA, from, to, pen);
  // Draw a curve
  float ctrlFrom[2] = {40.5, 0.5};
  float ctrlTo[2] = {80.5, 50.5};
  TGAPencilSetShapeRound(pen);
  TGAPencilSetAntialias(pen, true);
  TGAPencilSetModeColorSolid(pen);
  TGAPencilSetThickness(pen, 5.0);
  TGADrawCurve(theTGA, from, ctrlFrom, ctrlTo, to, pen);
  // Print some strings
  TGAPencilSetThickness(pen, 1.0);
  pix->_rgba[0] = pix->_rgba[1] = pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  TGAFont *font = TGAFontCreate(tgaFontDefault);
  if (font == NULL) {
    fprintf(stderr, "Can't create the font\n");
    return 1;
  }
  from[0] = 5.0; from[1] = 200.0;
  TGAFontSetSize(font, 12.0);
  float v[2] = {0.5, 1.0};
  TGAFontSetScale(font, v);
  v[0] = 5.0; v[1] = 3.0;
  TGAFontSetSpace(font, v);
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"ABCDEFGHIJ\nKLMNOPQRST\nUVWXYZ", from);
  from[0] = 5.0; from[1] = 155.0;
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"0123456789", from);
  from[0] = 5.0; from[1] = 250.0;
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"abcdefghij\nklmnopqrst\nuvwxyz^@", from);
  from[0] = 5.0; from[1] = 140.0;
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"!\"#$%&'()=\n~`{}*+<>?,\n./\\[]-|_;:", from);
  // Save the TGA
  TGASave(theTGA, "./out.tga");
  //Free the tga
  TGAFree(&theTGA);
  // Load the TGA 
  ret = TGALoad(&theTGA, "./out.tga");
  if (ret != 0) {
    fprintf(stderr, "Error while opening the file : %d\n", ret);
    return 1;
  }
  // Print its header on standard output stream
  TGAPrintHeader(theTGA, stdout);
  // Free the memory
  TGAFreeFont(&font);
  TGAFree(&theTGA);
  TGAFreePixel(&pix);
  TGAFreePencil(&pen);
  return 0;
}
