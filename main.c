#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "pbmath.h"

int main(int argc, char **argv) {
  // Initialise the random generator
  srandom(time(NULL));

  // -------------- VecShort
  fprintf(stdout, "-------- VecShort\n");
  // Create a vector of dimension 3
  VecShort *a = VecShortCreate(3);
  // If we couldn't create the vector
  if (a == NULL) {
    fprintf(stderr, "VecCreate failed\n");
    return 1;
  }
  // Print the vector
  fprintf(stdout, "a: ");
  VecPrint(a, stdout);
  fprintf(stdout, "\n");
  // Set the 2nd value to 1
  VecSet(a, 1, 1);
  // Print the vector
  fprintf(stdout, "a: ");
  VecPrint(a, stdout);
  fprintf(stdout, "\n");
  // Clone the vector
  VecShort *cloneShort = VecClone(a);
  if (cloneShort == NULL) {
    fprintf(stderr, "VecClone failed\n");
    return 2;
  }
  // Print the vector
  fprintf(stdout, "cloneShort: ");
  VecPrint(cloneShort, stdout);
  fprintf(stdout, "\n");
  VecFree(&cloneShort);
  // Save the vector
  FILE *f = fopen("./vecshort.txt", "w");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 3;
  }
  int ret = VecSave(a, f);
  if (ret != 0) {
    fprintf(stderr, "VecSave failed (%d)\n", ret);
    return 4;
  }
  fclose(f);
  // Load the vector
  f = fopen("./vecshort.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 5;
  }
  VecShort *b = NULL;
  ret = VecLoad(&b, f);
  if (ret != 0) {
    fprintf(stderr, "VecLoad failed (%d)\n", ret);
    return 6;
  }
  fclose(f);
  // Get the dimension and values of the loaded vector
  fprintf(stdout, "b: %d ", VecDim(b));
  for (int i = 0; i < VecDim(b); ++i)
    fprintf(stdout, "%d ", VecGet(b, i));
  fprintf(stdout, "\n");
  // Change the values of the loaded vector and print it
  VecSet(b, 0, 2);
  VecSet(b, 2, 3);
  fprintf(stdout, "b: ");
  VecPrint(b, stdout);
  fprintf(stdout, "\n");
  // Copy the loaded vector into the first one and print the first one
  VecCopy(a, b);
  fprintf(stdout, "a: ");
  VecPrint(a, stdout);
  fprintf(stdout, "\n");
  // Free memory
  VecFree(&a);
  VecFree(&b);

  // -------------- VecFloat
  fprintf(stdout, "-------- VecFloat\n");
  // Create a vector of dimension 3
  VecFloat *v = VecFloatCreate(3);
  // If we couldn't create the vector
  if (v == NULL) {
    fprintf(stderr, "VecCreate failed\n");
    return 7;
  }
  // Print the vector
  fprintf(stdout, "v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  // Set the 2nd value to 1.0
  VecSet(v, 1, 1.0);
  // Print the vector
  fprintf(stdout, "v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  // Clone the vector
  VecFloat *cloneFloat = VecClone(v);
  if (cloneFloat == NULL) {
    fprintf(stderr, "VecClone failed\n");
    return 8;
  }
  // Print the vector
  fprintf(stdout, "cloneFloat: ");
  VecPrint(cloneFloat, stdout);
  fprintf(stdout, "\n");
  VecFree(&cloneFloat);
  // Save the vector
  f = fopen("./vecfloat.txt", "w");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 9;
  }
  ret = VecSave(v, f);
  if (ret != 0) {
    fprintf(stderr, "VecSave failed (%d)\n", ret);
    return 10;
  }
  fclose(f);
  // Load the vector
  f = fopen("./vecfloat.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 11;
  }
  VecFloat *w = NULL;
  ret = VecLoad(&w, f);
  if (ret != 0) {
    fprintf(stderr, "VecLoad failed (%d)\n", ret);
    return 12;
  }
  fclose(f);
  // Get the dimension and values of the loaded vector
  fprintf(stdout, "w: %d ", VecDim(w));
  for (int i = 0; i < VecDim(w); ++i)
    fprintf(stdout, "%f ", VecGet(w, i));
  fprintf(stdout, "\n");
  // Change the values of the loaded vector and print it
  VecSet(w, 0, 2.0);
  VecSet(w, 2, 3.0);
  fprintf(stdout, "w: ");
  VecPrint(w, stdout);
  fprintf(stdout, "\n");
  // Copy the loaded vector into the first one and print the first one
  VecCopy(v, w);
  fprintf(stdout, "v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  // Get the norm
  float norm = VecNorm(v);
  fprintf(stdout, "Norm of v: %.3f\n", norm);
  // Normalise
  VecNormalise(v);
  fprintf(stdout, "Normalized v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  // Distance between v and w  
  fprintf(stdout, "Distance between v and w: %.3f\n", VecDist(v, w));
  // Equality
  if (VecIsEqual(v, w) == true)
    fprintf(stdout, "v = w\n");
  else
    fprintf(stdout, "v != w\n");
  if (VecIsEqual(v, v) == true)
    fprintf(stdout, "v = v\n");
  else
    fprintf(stdout, "v != v\n");
  // Op
  VecFloat *x = VecGetOp(v, norm, w, 2.0);
  if (x == NULL) {
    fprintf(stderr, "VecGetOp failed\n");
    return 13;
  }
  fprintf(stdout, "x: ");
  VecPrint(x, stdout);
  fprintf(stdout, "\n");
  VecOp(v, norm, NULL, 0.0);
  fprintf(stdout, "v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  // Dot prod
  fprintf(stdout, "dot prod v.x: %.3f\n", VecDotProd(v, x));
  // Rotate
  VecFree(&v);
  v = VecFloatCreate(2);
  if (v == NULL) {
    fprintf(stderr, "malloc failed\n");
    return 14;
  }
  VecSet(v, 0, 1.0);
  fprintf(stdout, "v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  VecRot2D(v, PBMATH_QUARTERPI);
  fprintf(stdout, "v: ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  VecFree(&x);
  x = VecGetRot2D(v, PBMATH_QUARTERPI);
  if (v == NULL) {
    fprintf(stderr, "VecGetRot2D failed\n");
    return 15;
  }
  fprintf(stdout, "x: ");
  VecPrint(x, stdout);
  fprintf(stdout, "\n");
  // AngleTo
  fprintf(stdout, "Angle between vector:\n");
  float dtheta = PBMATH_PI / 6.0;
  VecSet(x, 0, 1.0); VecSet(x, 1, 0.0); 
  for (int i = 0; i < 12; ++i) {
    VecSet(v, 0, 1.0); VecSet(v, 1, 0.0); 
    for (int j = 0; j < 12; ++j) {
      VecPrint(x, stdout);
      fprintf(stdout, " ");
      VecPrint(v, stdout);
      fprintf(stdout, " %.3f\n", ConvRad2Deg(VecAngleTo2D(x, v)));
      VecRot2D(v, dtheta);
    }
    VecRot2D(x, dtheta);
  }
  // Free memory
  VecFree(&x);
  VecFree(&w);
  VecFree(&v);

  // -------------- MatFloat
  fprintf(stdout, "-------- MatFloat\n");
  // Create a matrix of dimension 3,2
  VecShort *dimMat = VecShortCreate(2);
  VecSet(dimMat, 0, 3);
  VecSet(dimMat, 1, 2);
  MatFloat *mat = MatFloatCreate(dimMat);
  // If we couldn't create the matrix
  if (mat == NULL) {
    fprintf(stderr, "MatCreate failed\n");
    return 16;
  }
  // Print the matrix
  fprintf(stdout, "mat: \n");
  MatPrint(mat, stdout);
  fprintf(stdout, "\n");
  // Set some values
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 0);
  MatSet(mat, dimMat, 0.5);
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 1);
  MatSet(mat, dimMat, 2.0);
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 2);
  MatSet(mat, dimMat, 1.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 0);
  MatSet(mat, dimMat, 2.0);
  VecSet(dimMat, 0, 2);
  VecSet(dimMat, 1, 1);
  MatSet(mat, dimMat, 1.0);
  // Print the matrix
  fprintf(stdout, "mat: \n");
  MatPrint(mat, stdout);
  fprintf(stdout, "\n");
  // Clone the matrix
  MatFloat *cloneMatFloat = MatClone(mat);
  if (cloneMatFloat == NULL) {
    fprintf(stderr, "MatClone failed\n");
    return 17;
  }
  // Print the matrix
  fprintf(stdout, "cloneMatFloat:\n");
  MatPrint(cloneMatFloat, stdout);
  fprintf(stdout, "\n");
  MatFree(&cloneMatFloat);
  // Save the matrix
  f = fopen("./matfloat.txt", "w");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 18;
  }
  ret = MatSave(mat, f);
  if (ret != 0) {
    fprintf(stderr, "MatSave failed (%d)\n", ret);
    return 19;
  }
  fclose(f);
  // Load the matrix
  f = fopen("./matfloat.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 20;
  }
  MatFloat *matb = NULL;
  ret = MatLoad(&matb, f);
  if (ret != 0) {
    fprintf(stderr, "MatLoad failed (%d)\n", ret);
    return 21;
  }
  fclose(f);
  // Get the dimension and values of the loaded matrix
  VecShort *dimMatb = MatDim(matb);
  fprintf(stdout, "dim loaded matrix: ");
  VecPrint(dimMatb, stdout);
  fprintf(stdout, "\n");
  for (VecSet(dimMat, 1, 0); VecGet(dimMat, 1) < VecGet(dimMatb, 1); 
    VecSet(dimMat, 1, VecGet(dimMat, 1) + 1)) {
    for (VecSet(dimMat, 0, 0); VecGet(dimMat, 0) < VecGet(dimMatb, 0); 
      VecSet(dimMat, 0, VecGet(dimMat, 0) + 1))
      fprintf(stdout, "%f ", MatGet(matb, dimMat));
    fprintf(stdout, "\n");
  }
  // MatProdVec
  v = VecFloatCreate(3);
  if (v == NULL) {
    fprintf(stderr, "VecFloatCreate failed\n");
    return 22;
  }
  VecSet(v, 0, 2.0);
  VecSet(v, 1, 3.0);
  VecSet(v, 2, 4.0);
  w = MatProd(matb, v);
  if (w == NULL) {
    fprintf(stderr, "MatProd failed\n");
    return 23;
  }
  fprintf(stdout, "Mat prod of\n");
  MatPrint(matb, stdout);
  fprintf(stdout, "\nand\n");
  VecPrint(v, stdout);
  fprintf(stdout, "\nequals\n");
  VecPrint(w, stdout);
  fprintf(stdout, "\n");
  VecFree(&v);
  VecFree(&w);
  // MatProdMat
  VecSet(dimMat, 0, VecGet(dimMatb, 1));
  VecSet(dimMat, 1, VecGet(dimMatb, 0));
  MatFloat *matc = MatFloatCreate(dimMat);
  if (matc == NULL) {
    fprintf(stderr, "MatFloatCreate failed\n");
    return 24;
  }
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 0);
  MatSet(matc, dimMat, 1.0);
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 1);
  MatSet(matc, dimMat, 2.0);
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 2);
  MatSet(matc, dimMat, 3.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 0);
  MatSet(matc, dimMat, 4.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 1);
  MatSet(matc, dimMat, 5.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 2);
  MatSet(matc, dimMat, 6.0);
  fprintf(stdout, "Mat prod of\n");
  MatPrint(mat, stdout);
  fprintf(stdout, "\nand\n");
  MatPrint(matc, stdout);
  fprintf(stdout, "\nequals\n");
  MatFloat *matd = MatProd(mat, matc);
  if (matd == NULL) {
    fprintf(stderr, "MatProd failed\n");
    return 25;
  }
  MatPrint(matd, stdout);
  fprintf(stdout, "\n");  
  // Create a matrix and set it to identity
  VecSet(dimMat, 0, 3);
  VecSet(dimMat, 1, 3);
  MatFloat *squareMat = MatFloatCreate(dimMat);
  MatSetIdentity(squareMat);
  fprintf(stdout, "identity:\n");
  MatPrint(squareMat, stdout);
  fprintf(stdout, "\n");
  // Matrix inverse
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 0);
  MatSet(squareMat, dimMat, 3.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 0);
  MatSet(squareMat, dimMat, 0.0);
  VecSet(dimMat, 0, 2);
  VecSet(dimMat, 1, 0);
  MatSet(squareMat, dimMat, 2.0);
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 1);
  MatSet(squareMat, dimMat, 2.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 1);
  MatSet(squareMat, dimMat, 0.0);
  VecSet(dimMat, 0, 2);
  VecSet(dimMat, 1, 1);
  MatSet(squareMat, dimMat, -2.0);
  VecSet(dimMat, 0, 0);
  VecSet(dimMat, 1, 2);
  MatSet(squareMat, dimMat, 0.0);
  VecSet(dimMat, 0, 1);
  VecSet(dimMat, 1, 2);
  MatSet(squareMat, dimMat, 1.0);
  VecSet(dimMat, 0, 2);
  VecSet(dimMat, 1, 2);
  MatSet(squareMat, dimMat, 1.0);
  MatFloat *matinv = MatInv(squareMat);
  if (matinv == NULL) {
    fprintf(stderr, "MatInv failed\n");
    return 26;
  }
  fprintf(stdout, "inverse of:\n");
  MatPrint(squareMat, stdout);
  fprintf(stdout, "\nequals\n");
  MatPrint(matinv, stdout);
  fprintf(stdout, "\n");
  MatFloat *checkinv = MatProd(squareMat, matinv);
  fprintf(stdout, "check of inverse:\n");
  MatPrint(checkinv, stdout);
  fprintf(stdout, "\n");
  // Free memory
  VecFree(&dimMat);
  VecFree(&dimMatb);
  MatFree(&mat);
  MatFree(&matb);
  MatFree(&matc);
  MatFree(&matd);
  MatFree(&checkinv);
  MatFree(&squareMat);
  MatFree(&matinv);

  // -------------- Gauss
  fprintf(stdout, "-------- Gauss\n");
  // Create a Gauss function
  float mean = 0.0;
  float sigma = 1.0;
  Gauss *gauss = GaussCreate(mean, sigma);
  // If we couldn't create the Gauss
  if (gauss == NULL) {
    fprintf(stderr, "Couldn't create the Gauss\n");
    return 27;
  }
  // Get some values of the Gauss function
  fprintf(stdout, "Gauss function (mean:0.0, sigma:1.0):\n");
  for (float x = -2.0; x <= 2.01; x += 0.2)
    fprintf(stdout, "%.3f %.3f\n", x, GaussGet(gauss, x));
  // Change the mean
  gauss->_mean = 1.0;
  gauss->_sigma = 0.5;
  // Get some random values according to the Gauss function
  fprintf(stdout, "Gauss rnd (mean:1.0, sigma:0.5):\n");
  for (int iVal = 0; iVal < 10; ++iVal)
    fprintf(stdout, "%.3f %.3f\n", GaussRnd(gauss), GaussRnd(gauss));
  //Free memory
  GaussFree(&gauss);
  
  // -------------- Smoother
  fprintf(stdout, "-------- Smoother\n");
  for (float x = 0.0; x <= 1.01; x += 0.1)
    fprintf(stdout, "%.3f %.3f %.3f\n", x, SmoothStep(x), 
      SmootherStep(x));

  // -------------- Shapoid
  fprintf(stdout, "-------- Shapoid\n");
  Shapoid* facoidA = FacoidCreate(2);
  ShapoidPrint(facoidA, stdout);
  v = VecFloatCreate(2);
  if (v == NULL) {
    fprintf(stderr, "malloc failed\n");
    return 28;
  }
  VecSet(v, 0, 2.0);
  VecSet(v, 1, 3.0);
  ShapoidScale(facoidA, v);
  fprintf(stdout, "scale by ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  ShapoidPrint(facoidA, stdout);
  ShapoidRotate2D(facoidA, PBMATH_PI * 0.5);
  fprintf(stdout, "rotate by %.3f\n", PBMATH_PI * 0.5);
  ShapoidPrint(facoidA, stdout);
  VecSet(v, 0, 1.0);
  VecSet(v, 1, -1.0);
  ShapoidTranslate(facoidA, v);
  fprintf(stdout, "translate by ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  ShapoidPrint(facoidA, stdout);
  VecSet(v, 0, 0.0);
  VecSet(v, 1, 1.0);
  ShapoidSetAxis(facoidA, 0, v);
  fprintf(stdout, "set axis 0 to ");
  VecPrint(v, stdout);
  fprintf(stdout, "\n");
  ShapoidPrint(facoidA, stdout);
  fprintf(stdout, "save shapoid to shapoid.txt\n");
  f = fopen("./shapoid.txt", "w");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 29;
  }
  ShapoidSave(facoidA, f);
  fclose(f);
  fprintf(stdout, "load shapoid from shapoid.txt\n");
  Shapoid* facoidB = NULL;
  f = fopen("./shapoid.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "fopen failed\n");
    return 30;
  }
  ShapoidLoad(&facoidB, f);
  fclose(f);
  ShapoidPrint(facoidB, stdout);
  VecSet(v, 0, 1.0);
  VecSet(v, 1, 1.0);
  VecFloat *coordEx = ShapoidExportCoord(facoidB, v);
  if (coordEx == NULL) {
    fprintf(stderr, "ShapoidExport failed\n");
    return 31;
  }
  fprintf(stdout,"coordinates ");
  VecPrint(v, stdout);
  fprintf(stdout," in the shapoid becomes ");
  VecPrint(coordEx, stdout);
  fprintf(stdout," in the standard coordinate system\n");
  VecCopy(v, coordEx);
  VecFloat *coordIm = ShapoidImportCoord(facoidB, v);
  if (coordIm == NULL) {
    fprintf(stderr, "ShapoidImport failed\n");
    return 32;
  }
  fprintf(stdout,"coordinates ");
  VecPrint(v, stdout);
  fprintf(stdout," in the standard coordinate system becomes ");
  VecPrint(coordIm, stdout);
  fprintf(stdout," in the shapoid\n");
  VecSet(v, 0, 0.0);
  VecSet(v, 1, 0.0);
  VecPrint(v, stdout);
  if (ShapoidIsPosInside(facoidB, v) == true)
    fprintf(stdout, " is in the facoid\n");
  else
    fprintf(stdout, " is not in the facoid\n");
  VecSet(v, 0, 1.0);
  VecSet(v, 1, -4.0);
  VecPrint(v, stdout);
  if (ShapoidIsPosInside(facoidB, v) == true)
    fprintf(stdout, " is in the facoid\n");
  else
    fprintf(stdout, " is not in the facoid\n");
  ShapoidRotate2D(facoidB, -PBMATH_QUARTERPI);
  Shapoid *bounding = ShapoidGetBoundingBox(facoidB);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 33;
  }
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  ShapoidPrint(bounding, stdout);
  ShapoidFree(&bounding);
  VecFloat *center = ShapoidGetCenter(facoidB);
  if (center == NULL) {
    fprintf(stderr, "ShapoidGetCenter failed\n");
    return 34;
  }
  fprintf(stdout, "center of the facoid is ");
  VecPrint(center, stdout);
  fprintf(stdout, "\n");
  VecFree(&center);
  facoidB->_type = ShapoidTypePyramidoid;
  bounding = ShapoidGetBoundingBox(facoidB);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 35;
  }
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  ShapoidPrint(bounding, stdout);
  ShapoidFree(&bounding);
  center = ShapoidGetCenter(facoidB);
  if (center == NULL) {
    fprintf(stderr, "ShapoidGetCenter failed\n");
    return 36;
  }
  fprintf(stdout, "center of the facoid is ");
  VecPrint(center, stdout);
  fprintf(stdout, "\n");
  VecFree(&center);
  facoidB->_type = ShapoidTypeSpheroid;
  bounding = ShapoidGetBoundingBox(facoidB);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 37;
  }
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  ShapoidPrint(bounding, stdout);
  center = ShapoidGetCenter(facoidB);
  if (center == NULL) {
    fprintf(stderr, "ShapoidGetCenter failed\n");
    return 38;
  }
  fprintf(stdout, "center of the shapoid is ");
  VecPrint(center, stdout);
  fprintf(stdout, "\n");
  VecFree(&center);

  GSet *setBounding = GSetCreate();
  if (setBounding == NULL) {
    fprintf(stderr, "GSetCreate failed\n");
    return 39;
  }
  GSetAppend(setBounding, facoidA);
  GSetAppend(setBounding, facoidB);
  facoidB->_type = ShapoidTypeFacoid;
  VecSet(facoidB->_pos, 0, 2.0);
  fprintf(stdout, "bounding box of\n");
  ShapoidPrint(facoidA, stdout);
  fprintf(stdout, "and\n");
  ShapoidPrint(facoidB, stdout);
  fprintf(stdout, "is\n");
  bounding = ShapoidGetBoundingBox(setBounding);
  if (bounding == NULL) {
    fprintf(stderr, "ShapoidGetBoundingBox failed\n");
    return 40;
  }
  ShapoidPrint(bounding, stdout);
  ShapoidFree(&bounding);
  GSetFree(&setBounding);
  // Free memory
  VecFree(&coordEx);
  VecFree(&coordIm);
  VecFree(&v);
  ShapoidFree(&facoidA);
  ShapoidFree(&facoidB);
  ShapoidFree(&bounding);

  // -------------- Conversion functions
  fprintf(stdout, "-------- Conversion functions\n");
  fprintf(stdout, "%f radians -> %f degrees\n", PBMATH_QUARTERPI, 
    ConvRad2Deg(PBMATH_QUARTERPI));
  fprintf(stdout, "%f radians -> %f degrees\n", 90.0, 
    ConvDeg2Rad(90.0));

  // Return success code
  return 0;
}

