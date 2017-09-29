#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "bcurve.h"

int main(int argc, char **argv) {
  // Create a BCurve
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  // If we couldn't create the BCurve
  if (curve == NULL) {
    // Print a message
    fprintf(stderr, "BCurveCreate failed\n");
    // Stop here
    return 1;
  }
  // Print the BCurve
  BCurvePrint(curve, stdout);
  fprintf(stdout, "\n");
  // Create a VecFloat to set the values
  VecFloat *v = VecFloatCreate(dim);
  // If we couldn't create the VecFloat
  if (v == NULL) {
    // Release memory
    BCurveFree(&curve);
    // Stop here
    return 2;
  }
  // Set the control points
  float ctrlPts[8] = {0.0, 1.0, 2.0, 5.0, 4.0, 3.0, 6.0, 7.0};
  for (int iCtrl = 0; iCtrl < order + 1; ++iCtrl) {
    VecSet(v, 0, ctrlPts[2 * iCtrl]);
    VecSet(v, 1, ctrlPts[2 * iCtrl + 1]);
    BCurveSet(curve, iCtrl, v);
  }
  // Print the BCurve
  BCurvePrint(curve, stdout);
  fprintf(stdout, "\n");
  // Save the curve
  FILE *file = fopen("./curve.txt", "w");
  // If we couldn't open the file
  if (file == NULL) {
    // Print a message
    fprintf(stderr, "Can't open file\n");
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    // Stop here
    return 3;
  }
  int ret = BCurveSave(curve, file);
  // If we couldn't save
  if (ret != 0) {
    // Print a message
    fprintf(stderr, "BCurveSave failed (%d)\n", ret);
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    // Stop here
    return 4;
  }
  fclose(file);
  // Load the curve
  file = fopen("./curve.txt", "r");
  // If we couldn't open the file
  if (file == NULL) {
    // Print a message
    fprintf(stderr, "Can't open file\n");
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    // Stop here
    return 5;
  }
  BCurve *loaded = NULL;
  ret = BCurveLoad(&loaded, file);
  // If we couldn't load
  if (ret != 0) {
    // Print a message
    fprintf(stderr, "BCurveLoad failed (%d)\n", ret);
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    BCurveFree(&loaded);
    // Stop here
    return 6;
  }
  fclose(file);
  // Print the loaded curve
  BCurvePrint(loaded, stdout);
  fprintf(stdout, "\n");
  // Get some values of the curve
  for (float u = 0.0; u <= 1.01; u += 0.1) {
    VecFloat *w = BCurveGet(curve, u);
    // If we couldn't get the values
    if (w == NULL) {
      // Free memory
      VecFree(&v);
      BCurveFree(&curve);
      BCurveFree(&loaded);
      // Stop here 
      return 7;
    }
    fprintf(stdout, "%.1f: ", u);
    VecPrint(w, stdout);
    fprintf(stdout, "\n");
    VecFree(&w);
  }
  // Rotate the curve
  BCurveRot2D(curve, PBMATH_PI * 0.5);
  // Get some values of the curve
  fprintf(stdout, "after rotation:\n");
  for (float u = 0.0; u <= 1.01; u += 0.1) {
    VecFloat *w = BCurveGet(curve, u);
    // If we couldn't get the values
    if (w == NULL) {
      // Free memory
      VecFree(&v);
      BCurveFree(&curve);
      BCurveFree(&loaded);
      // Stop here 
      return 7;
    }
    fprintf(stdout, "%.1f: ", u);
    VecPrint(w, stdout);
    fprintf(stdout, "\n");
    VecFree(&w);
  }  
  // Print the curve approximate length
  fprintf(stdout, "approx length: %.3f\n", BCurveApproxLen(curve));
  // Free memory
  VecFree(&v);
  BCurveFree(&curve);
  BCurveFree(&loaded);
  // Return success code
  return 0;
}

