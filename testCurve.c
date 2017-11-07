#include <stdio.h>
#include <stdlib.h>
#include "tgapaint.h"

int main(void) {
  VecShort *dim = VecShortCreate(2);
  VecSet(dim, 0, 120);
  VecSet(dim, 1, 120);
  TGAPixel *pixel = TGAGetWhitePixel();
  TGA *tga = TGACreate(dim, pixel);
  VecFloat *pos = VecFloatCreate(2);
  VecSet(pos, 0, 10.0);
  VecSet(pos, 1, 10.0);
  Shapoid *penTip = SpheroidCreate(2);
  VecFloat *v = VecFloatCreate(2);
  VecSet(v, 0, 1.0);
  VecSet(v, 1, 0.3);
  ShapoidScale(penTip, v);
  ShapoidRotate2D(penTip, ConvDeg2Rad(45.0));
  TGAPencil *pen = TGAGetPencil();

  TGAPencilSetShapePixel(pen);
  //TGAPencilSetShapeSquare(pen);
  //TGAPencilSetShapeRound(pen);
  //TGAPencilSetShapeShapoid(pen, penTip);

  TGAPencilSetThickness(pen, 2.0);
  unsigned char rgba[4] = {0, 0, 0, 155};
  TGAPencilSetColRGBA(pen, rgba);
  TGAPencilSetAntialias(pen, true);
  TGAFont *font = TGAFontCreate(tgaFontDefault);
  TGAFontSetSize(font, 12.0);
  VecSet(v, 0, 0.5);
  VecSet(v, 1, 1.0);
  TGAFontSetScale(font, v);
  char c = 'a';
  TGAPrintChar(tga, pen, font, c, pos);
  TGASave(tga, "testCurve.tga");
  return 0;
}
