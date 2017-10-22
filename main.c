#include <stdio.h>
#include <stdlib.h>
#include "tgapaint.h"

int main(void) {
  int ret;
  TGA *theTGA;
  // Create the TGA
  VecShort *dim = VecShortCreate(2); 
  VecSet(dim, 0, 120); VecSet(dim, 1, 270);
  TGAPixel *pix = TGAGetWhitePixel();
  theTGA = TGACreate(dim, pix);
  if (theTGA == NULL) {
    fprintf(stderr, "Error while creating the tga\n");
    return 1;
  }
  // Set the color of some pixels
  VecShort *pos = VecShortCreate(2);
  if (pos == NULL) {
    fprintf(stderr, "VecShortCreate failed\n");
    return 2;
  }
  VecSet(pos, 0, 60); VecSet(pos, 1, 50);
  TGASetPix(theTGA, pos, pix);
  pix->_rgba[0] = 255; pix->_rgba[1] = 0; pix->_rgba[2] = 0;
  VecSet(pos, 0, 90); VecSet(pos, 1, 50);
  TGASetPix(theTGA, pos, pix);
  pix->_rgba[0] = 0; pix->_rgba[1] = 0; pix->_rgba[2] = 255;
  VecSet(pos, 0, 60); VecSet(pos, 1, 25);
  TGASetPix(theTGA, pos, pix);
  pix->_rgba[0] = 0; pix->_rgba[1] = 255; pix->_rgba[2] = 0;
  VecSet(pos, 0, 30); VecSet(pos, 1, 75);
  TGASetPix(theTGA, pos, pix);
  // Draw some lines
  TGAPencil *pen = TGAGetBlackPencil();
  pix->_rgba[0] = 0; pix->_rgba[1] = 0; pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  VecFloat *from = VecFloatCreate(2);
  if (from == NULL) {
    fprintf(stderr, "VecFloatCreate failed\n");
    return 3;
  }
  VecFloat *to = VecFloatCreate(2);
  if (to == NULL) {
    fprintf(stderr, "VecFloatCreate failed\n");
    return 4;
  }
  VecSet(from, 0, 50.5); VecSet(from, 1, 40.5); 
  VecSet(to, 0, 50.5); VecSet(to, 1, 60.5);
  TGADrawLine(theTGA, from, to, pen);
  VecSet(from, 0, 50.5); VecSet(from, 1, 60.5); 
  VecSet(to, 0, 70.5); VecSet(to, 1, 60.5);
  TGADrawLine(theTGA, from, to, pen);
  pix->_rgba[0] = 255; pix->_rgba[1] = 0; pix->_rgba[2] = 255;
  VecSet(from, 0, -10.5); VecSet(from, 1, 50.5); 
  VecSet(to, 0, 60.5); VecSet(to, 1, -10.5);
  TGADrawLine(theTGA, from, to, pen);
  VecSet(from, 0, 60.5); VecSet(from, 1, -10.5); 
  VecSet(to, 0, 130.5); VecSet(to, 1, 50.5);
  TGADrawLine(theTGA, from, to, pen);
  VecSet(from, 0, 130.5); VecSet(from, 1, 50.5); 
  VecSet(to, 0, 60.5); VecSet(to, 1, 110.5);
  TGADrawLine(theTGA, from, to, pen);
  VecSet(from, 0, 60.5); VecSet(from, 1, 110.5); 
  VecSet(to, 0, -10.5); VecSet(to, 1, 50.5);
  TGADrawLine(theTGA, from, to, pen);
  // Apply gaussian blur
  TGAFilterGaussBlur(theTGA, 0.5, 2.0);
  // Draw a rectangle
  pix->_rgba[0] = 0; pix->_rgba[1] = 255; pix->_rgba[2] = 255;
  TGAPencilSetColor(pen, pix);
  VecSet(from, 0, 70.5); VecSet(from, 1, 40.5); 
  VecSet(to, 0, 100.5); VecSet(to, 1, 10.5);
  TGADrawRect(theTGA, from, to, pen);
  // Draw a filled rectangle
  pix->_rgba[0] = 255; pix->_rgba[1] = 255; pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  VecSet(from, 0, 75.5); VecSet(from, 1, 35.5); 
  VecSet(to, 0, 95.5); VecSet(to, 1, 15.5);
  TGAFillRect(theTGA, from, to, pen);
  // Draw an ellipse
  pix->_rgba[0] = 128; pix->_rgba[1] = 128; pix->_rgba[2] = 128;
  TGAPencilSetColor(pen, pix);
  VecFloat *center = VecFloatCreate(2); 
  VecSet(center, 0, 30.5); VecSet(center, 1, 50.5);
  VecFloat *radius = VecFloatCreate(2); 
  VecSet(radius, 0, 15.5); VecSet(radius, 1, 20.5);
  TGADrawEllipse(theTGA, center, radius, pen);
  // Draw a filled ellipse
  pix->_rgba[0] = 200; pix->_rgba[1] = 200; pix->_rgba[2] = 200;
  TGAPencilSetColor(pen, pix);
  VecSet(center, 0, 60.5); VecSet(center, 1, 75.5);
  VecSet(radius, 0, 25.5); VecSet(radius, 1, 10.5);
  TGAFillEllipse(theTGA, center, radius, pen);
  // Draw a line using blend colors
  VecSet(from, 0, 30.5); VecSet(from, 1, 25.5); 
  VecSet(to, 0, 90.5); VecSet(to, 1, 75.5);
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
  VecFloat *ctrlFrom = VecFloatCreate(2); 
  VecSet(ctrlFrom, 0, 40.5); VecSet(ctrlFrom, 1, 0.5);
  VecFloat *ctrlTo = VecFloatCreate(2); 
  VecSet(ctrlTo, 0, 80.5); VecSet(ctrlTo, 1, 50.5);
  BCurve *curve = BCurveCreate(3, 2);
  if (curve == NULL) {
    fprintf(stderr, "Can't create the curve\n");
    return 5;
  }
  BCurveSet(curve, 0, from);
  BCurveSet(curve, 1, ctrlFrom);
  BCurveSet(curve, 2, ctrlTo);
  BCurveSet(curve, 3, to);
  TGAPencilSetShapeRound(pen);
  TGAPencilSetAntialias(pen, true);
  TGAPencilSetModeColorSolid(pen);
  TGAPencilSetThickness(pen, 5.0);
  TGADrawCurve(theTGA, curve, pen);
  BCurveFree(&curve);
  // Print some strings
  TGAPencilSetThickness(pen, 1.0);
  pix->_rgba[0] = pix->_rgba[1] = pix->_rgba[2] = 0;
  TGAPencilSetColor(pen, pix);
  TGAFont *font = TGAFontCreate(tgaFontDefault);
  if (font == NULL) {
    fprintf(stderr, "Can't create the font\n");
    return 6;
  }
  TGAFontSetAnchor(font, tgaFontAnchorTopLeft);
  VecSet(from, 0, 5.0); VecSet(from, 1, 212.0);
  TGAFontSetSize(font, 12.0);
  VecFloat *v = VecFloatCreate(2); 
  VecSet(v, 0, 0.5); VecSet(v, 1, 1.0);
  TGAFontSetScale(font, v);
  VecSet(v, 0, 5.0); VecSet(v, 1, 3.0);
  TGAFontSetSpace(font, v);
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"ABCDEFGHIJ\nKLMNOPQRST\nUVWXYZ", from);
  VecSet(from, 0, 5.0); VecSet(from, 1, 167.0);
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"0123456789", from);
  VecSet(from, 0, 5.0); VecSet(from, 1, 257.0);
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"abcdefghij\nklmnopqrst\nuvwxyz^@", from);
  VecSet(from, 0, 5.0); VecSet(from, 1, 152.0);
  TGAPrintString(theTGA, pen, font, 
    (unsigned char *)"!\"#$%&'()=\n~`{}*+<>?,\n./\\[]-|_;:", from);
  // Draw some Shapoid
  Shapoid *shapoid = FacoidCreate(2);
  if (shapoid == NULL) {
    fprintf(stderr, "Can't create the shapoid\n");
    return 7;
  }
  VecSet(v, 0, 20.0); VecSet(v, 1, 0.0);
  ShapoidSetAxis(shapoid, 0, v);
  VecSet(v, 0, 10.0); VecSet(v, 1, 20.0);
  ShapoidSetAxis(shapoid, 1, v);
  VecSet(v, 0, 10.0); VecSet(v, 1, 40.0);
  ShapoidSetPos(shapoid, v);
  TGADrawShapoid(theTGA, shapoid, pen);
  shapoid->_type = ShapoidTypePyramidoid;
  VecSet(v, 0, 20.0); VecSet(v, 1, 80.0);
  ShapoidSetPos(shapoid, v);
  ShapoidRotate2D(shapoid, 1.0);
  TGADrawShapoid(theTGA, shapoid, pen);
  shapoid->_type = ShapoidTypeSpheroid;
  VecSet(v, 0, 110.0); VecSet(v, 1, 80.0);
  ShapoidSetPos(shapoid, v);
  ShapoidRotate2D(shapoid, 0.5);
  TGADrawShapoid(theTGA, shapoid, pen);
  // Draw some filled shapoid with depth gradation
  TGAPencilSetModeColorBlend(pen, 0, 1);
  TGAPencilSetShapePixel(pen);
  pix->_rgba[3] = 255;
  pix->_rgba[0] = 255; pix->_rgba[1] = 0; pix->_rgba[2] = 0; 
  TGAPencilSelectColor(pen, 0);
  TGAPencilSetColor(pen, pix);
  pix->_rgba[0] = 0; pix->_rgba[1] = 0; pix->_rgba[2] = 255; 
  TGAPencilSelectColor(pen, 1);
  TGAPencilSetColor(pen, pix);
  shapoid->_type = ShapoidTypeFacoid;
  VecSet(v, 0, 20.0); VecSet(v, 1, 0.0);
  ShapoidSetAxis(shapoid, 0, v);
  VecSet(v, 0, 10.0); VecSet(v, 1, 20.0);
  ShapoidSetAxis(shapoid, 1, v);
  VecSet(v, 0, 5.0); VecSet(v, 1, 5.0);
  ShapoidSetPos(shapoid, v);
  TGAFillShapoid(theTGA, shapoid, pen);
  shapoid->_type = ShapoidTypePyramidoid;
  VecSet(v, 0, 50.0); VecSet(v, 1, 5.0);
  ShapoidSetPos(shapoid, v);
  ShapoidRotate2D(shapoid, 1.0);
  TGAFillShapoid(theTGA, shapoid, pen);
  shapoid->_type = ShapoidTypeSpheroid;
  VecSet(v, 0, 100.0); VecSet(v, 1, 12.0);
  ShapoidSetPos(shapoid, v);
  ShapoidRotate2D(shapoid, 0.5);
  TGAFillShapoid(theTGA, shapoid, pen);
  // Save the TGA
  TGASave(theTGA, "./out.tga");
  //Free the tga
  TGAFree(&theTGA);
  // Load the TGA 
  ret = TGALoad(&theTGA, "./out.tga");
  if (ret != 0) {
    fprintf(stderr, "Error while opening the file : %d\n", ret);
    return 8;
  }
  // Print its header on standard output stream
  TGAPrintHeader(theTGA, stdout);
  // Free the memory
  ShapoidFree(&shapoid);
  VecFree(&pos);
  VecFree(&dim);
  VecFree(&v);
  VecFree(&ctrlFrom);
  VecFree(&ctrlTo);
  VecFree(&center);
  VecFree(&radius);
  VecFree(&from);
  VecFree(&to);
  TGAFreeFont(&font);
  TGAFree(&theTGA);
  TGAFreePixel(&pix);
  TGAFreePencil(&pen);
  return 0;
}
