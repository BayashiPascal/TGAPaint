// ============ PBMATH.C ================

// ================= Include =================

#include "pbmath.h"

// ================= Define ==================

#define rnd() (float)(rand())/(float)(RAND_MAX)

// -------------- VecShort

// ================= Define ==================

// ================ Functions implementation ====================

// Create a new Vec of dimension 'dim'
// Values are initalized to 0.0
// Return NULL if we couldn't create the Vec
VecShort* VecShortCreate(int dim) {
  // Check argument
  if (dim <= 0)
    return NULL;
  // Allocate memory
  VecShort *that = (VecShort*)malloc(sizeof(VecShort));
  //If we could allocate memory
  if (that != NULL) {
    // Allocate memory for values
    that->_val = (short*)malloc(sizeof(short) * dim);
    // If we couldn't allocate memory
    if (that->_val == NULL) {
      // Free memory
      free(that);
      // Stop here
      return NULL;
    }
    // Set the default values
    that->_dim = dim;
    for (int i = dim; i--;)
      that->_val[i] = 0.0;
  }
  // Return the new VecShort
  return that;
}

// Clone the VecShort
// Return NULL if we couldn't clone the VecShort
VecShort* VecShortClone(VecShort *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Create a clone
  VecShort *clone = VecShortCreate(that->_dim);
  // If we could create the clone
  if (clone != NULL) {
    // Clone the properties
    for (int i = that->_dim; i--;)
      clone->_val[i] = that->_val[i];
  }
  // Return the clone
  return clone;
}

// Load the VecShort from the stream
// If the VecShort is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
int VecShortLoad(VecShort **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    VecShortFree(that);
  }
  // Read the number of dimension
  int dim;
  int ret = fscanf(stream, "%d", &dim);
  // If we coudln't fscanf
  if (ret == EOF)
    return 4;
  if (dim <= 0)
    return 3;
  // Allocate memory
  *that = VecShortCreate(dim);
  // If we coudln't allocate memory
  if (*that == NULL) {
    return 2;
  }
  // Read the values
  for (int i = 0; i < dim; ++i) {
    ret = fscanf(stream, "%hi", (*that)->_val + i);
    // If we coudln't fscanf
    if (ret == EOF)
      return 4;
  }
  // Return success code
  return 0;
}

// Save the VecShort to the stream
// Return 0 upon success, or:
// 1: invalid arguments
// 2: fprintf error
int VecShortSave(VecShort *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the dimension
  int ret = fprintf(stream, "%d ", that->_dim);
  // If we coudln't fprintf
  if (ret < 0)
    return 2;
  // Save the values
  for (int i = 0; i < that->_dim; ++i) {
    ret = fprintf(stream, "%hi ", that->_val[i]);
    // If we coudln't fprintf
    if (ret < 0)
      return 2;
  }
  fprintf(stream, "\n");
  // If we coudln't fprintf
  if (ret < 0)
    return 2;
  // Return success code
  return 0;
}

// Free the memory used by a VecShort
// Do nothing if arguments are invalid
void VecShortFree(VecShort **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  free((*that)->_val);
  free(*that);
  *that = NULL;
}

// Print the VecShort on 'stream' with 'prec' digit precision
// Do nothing if arguments are invalid
void VecShortPrint(VecShort *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return;
  // Print the values
  fprintf(stream, "<");
  for (int i = 0; i < that->_dim; ++i) {
    fprintf(stream, "%hi", that->_val[i]);
    if (i < that->_dim - 1)
      fprintf(stream, ",");
  }
  fprintf(stream, ">");
}

// Return the i-th value of the VecShort
// Index starts at 0
// Return 0.0 if arguments are invalid
short VecShortGet(VecShort *that, int i) {
  // Check argument
  if (that == NULL || i < 0 || i >= that->_dim)
    return 0.0;
  // Return the value
  return that->_val[i];
}

// Set the i-th value of the VecShort to v
// Index starts at 0
// Do nohting if arguments are invalid
void VecShortSet(VecShort *that, int i, short v) {
  // Check argument
  if (that == NULL || i < 0 || i >= that->_dim)
    return;
  // Set the value
  that->_val[i] = v;
}

// Return the dimension of the VecShort
// Return 0 if arguments are invalid
int VecShortDim(VecShort *that) {
  // Check argument
  if (that == NULL)
    return 0;
  // Return the dimension
  return that->_dim;
}

// Copy the values of 'w' in 'that' (must have same dimensions)
// Do nothing if arguments are invalid
void VecShortCopy(VecShort *that, VecShort *w) {
  // Check argument
  if (that == NULL || w == NULL || that->_dim != w->_dim)
    return;
  // Copy the values
  memcpy(that->_val, w->_val, sizeof(short) * that->_dim);
}

// -------------- VecFloat

// ================= Define ==================

// ================ Functions implementation ====================

// Create a new Vec of dimension 'dim'
// Values are initalized to 0.0
// Return NULL if we couldn't create the Vec
VecFloat* VecFloatCreate(int dim) {
  // Check argument
  if (dim <= 0)
    return NULL;
  // Allocate memory
  VecFloat *that = (VecFloat*)malloc(sizeof(VecFloat));
  //If we could allocate memory
  if (that != NULL) {
    // Allocate memory for values
    that->_val = (float*)malloc(sizeof(float) * dim);
    // If we couldn't allocate memory
    if (that->_val == NULL) {
      // Free memory
      free(that);
      // Stop here
      return NULL;
    }
    // Set the default values
    that->_dim = dim;
    for (int i = dim; i--;)
      that->_val[i] = 0.0;
  }
  // Return the new VecFloat
  return that;
}

// Clone the VecFloat
// Return NULL if we couldn't clone the VecFloat
VecFloat* VecFloatClone(VecFloat *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Create a clone
  VecFloat *clone = VecFloatCreate(that->_dim);
  // If we could create the clone
  if (clone != NULL) {
    // Clone the properties
    for (int i = that->_dim; i--;)
      clone->_val[i] = that->_val[i];
  }
  // Return the clone
  return clone;
}

// Load the VecFloat from the stream
// If the VecFloat is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
int VecFloatLoad(VecFloat **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    VecFloatFree(that);
  }
  // Read the number of dimension
  int dim;
  int ret = fscanf(stream, "%d", &dim);
  // If we coudln't fscanf
  if (ret == EOF)
    return 4;
  if (dim <= 0)
    return 3;
  // Allocate memory
  *that = VecFloatCreate(dim);
  // If we coudln't allocate memory
  if (*that == NULL) {
    return 2;
  }
  // Read the values
  for (int i = 0; i < dim; ++i) {
    ret = fscanf(stream, "%f", (*that)->_val + i);
    // If we coudln't fscanf
    if (ret == EOF)
      return 4;
  }
  // Return success code
  return 0;
}

// Save the VecFloat to the stream
// Return 0 upon success, or:
// 1: invalid arguments
// 2: fprintf error
int VecFloatSave(VecFloat *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the dimension
  int ret = fprintf(stream, "%d ", that->_dim);
  // If we coudln't fprintf
  if (ret < 0)
    return 2;
  // Save the values
  for (int i = 0; i < that->_dim; ++i) {
    ret = fprintf(stream, "%f ", that->_val[i]);
    // If we coudln't fprintf
    if (ret < 0)
      return 2;
  }
  fprintf(stream, "\n");
  // If we coudln't fprintf
  if (ret < 0)
    return 2;
  // Return success code
  return 0;
}

// Free the memory used by a VecFloat
// Do nothing if arguments are invalid
void VecFloatFree(VecFloat **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  free((*that)->_val);
  free(*that);
  *that = NULL;
}

// Print the VecFloat on 'stream' with 'prec' digit precision
// Do nothing if arguments are invalid
void VecFloatPrint(VecFloat *that, FILE *stream, int prec) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return;
  // Create the format string
  char format[20] = {'\0'};
  sprintf(format, "%%.%df", prec);
  // Print the values
  fprintf(stream, "<");
  for (int i = 0; i < that->_dim; ++i) {
    fprintf(stream, format, that->_val[i]);
    if (i < that->_dim - 1)
      fprintf(stream, ",");
  }
  fprintf(stream, ">");
}
void VecFloatPrintDef(VecFloat *that, FILE *stream) {
  VecFloatPrint(that, stream, 3);
}

// Return the i-th value of the VecFloat
// Index starts at 0
// Return 0.0 if arguments are invalid
float VecFloatGet(VecFloat *that, int i) {
  // Check argument
  if (that == NULL || i < 0 || i >= that->_dim)
    return 0.0;
  // Return the value
  return that->_val[i];
}

// Set the i-th value of the VecFloat to v
// Index starts at 0
// Do nohting if arguments are invalid
void VecFloatSet(VecFloat *that, int i, float v) {
  // Check argument
  if (that == NULL || i < 0 || i >= that->_dim)
    return;
  // Set the value
  that->_val[i] = v;
}

// Return the dimension of the VecFloat
// Return 0 if arguments are invalid
int VecFloatDim(VecFloat *that) {
  // Check argument
  if (that == NULL)
    return 0;
  // Return the dimension
  return that->_dim;
}

// Copy the values of 'w' in 'that' (must have same dimensions)
// Do nothing if arguments are invalid
void VecFloatCopy(VecFloat *that, VecFloat *w) {
  // Check argument
  if (that == NULL || w == NULL || that->_dim != w->_dim)
    return;
  // Copy the values
  memcpy(that->_val, w->_val, sizeof(float) * that->_dim);
}

// Return the norm of the VecFloat
// Return 0.0 if arguments are invalid
float VecFloatNorm(VecFloat *that) {
  // Check argument
  if (that == NULL)
    return 0.0;
  // Declare a variable to calculate the norm
  float ret = 0.0;
  // Calculate the norm
  for (int iDim = that->_dim; iDim--;)
    ret += pow(that->_val[iDim], 2.0);
  ret = sqrt(ret);
  // Return the result
  return ret;
}

// Normalise the VecFloat
// Do nothing if arguments are invalid
void VecFloatNormalise(VecFloat *that) {
  // Check argument
  if (that == NULL)
    return;
  // Normalise
  float norm = VecNorm(that);
  for (int iDim = that->_dim; iDim--;)
    that->_val[iDim] /= norm;
}

// Return the distance between the VecFloat 'that' and 'tho'
// Return NaN if arguments are invalid
// If dimensions are different, missing ones are considered to 
// be equal to 0.0
float VecFloatDist(VecFloat *that, VecFloat *tho) {
  // Check argument
  if (that == NULL || tho == NULL)
    return NAN;
  // Declare a variable to calculate the distance
  float ret = 0.0;
  for (int iDim = that->_dim; iDim--;)
    ret += pow(VecGet(that, iDim) - VecGet(tho, iDim), 2.0);
  ret = sqrt(ret);
  // Return the distance
  return ret;
}

// Return true if the VecFloat 'that' is equal to 'tho'
// Return false if arguments are invalid
// If dimensions are different, missing ones are considered to 
// be equal to 0.0
bool VecFloatIsEqual(VecFloat *that, VecFloat *tho) {
  // Check argument
  if (that == NULL || tho == NULL)
    return false;
  // For each component
  for (int iDim = that->_dim; iDim--;)
    // If the values of this components are different
    if (fabs(VecGet(that, iDim) - VecGet(tho, iDim)) > PBMATH_EPSILON)
      // Return false
      return false;
  // Return true
  return true;
}

// Calculate (that * a + tho * b) and store the result in 'that'
// Do nothing if arguments are invalid
// 'tho' can be null, in which case it is consider to be the null vector
// If 'tho' is not null it must be of same dimension as 'that'
void VecFloatOp(VecFloat *that, float a, VecFloat *tho, float b) {
  // Check argument
  if (that == NULL)
    return;
  // Calculate 
  VecFloat *res = VecFloatGetOp(that, a, tho, b);
  // If we could calculate
  if (res != NULL) {
    // Copy the result in 'that'
    VecFloatCopy(that, res);
    // Free memory
    VecFloatFree(&res);
  }
}

// Return a VecFloat equal to (that * a + tho * b)
// Return NULL if arguments are invalid
// 'tho' can be null, in which case it is consider to be the null vector
// If 'tho' is not null it must be of same dimension as 'that'
VecFloat* VecFloatGetOp(VecFloat *that, float a, 
  VecFloat *tho, float b) {
  // Check argument
  if (that == NULL || (tho != NULL && that->_dim != tho->_dim))
    return NULL;
  // Declare a variable to memorize the result
  VecFloat *res = VecFloatCreate(that->_dim);
  // If we could allocate memory
  if (res != NULL) {
    // For each component
    for (int iDim = that->_dim; iDim--;) {
      // Calculate
      res->_val[iDim] = a * that->_val[iDim];
      if (tho != NULL)
        res->_val[iDim] += b * tho->_val[iDim];
    }
  }
  // Return the result
  return res;
}

// Rotate CCW 'that' by 'theta' radians and store the result in 'that'
// Do nothing if arguments are invalid
void VecFloatRot2D(VecFloat *that, float theta) {
  // Check argument
  if (that == NULL || that->_dim != 2)
    return;
  // Calculate 
  VecFloat *res = VecFloatGetRot2D(that, theta);
  // If we could calculate
  if (res != NULL) {
    // Copy the result in 'that'
    VecFloatCopy(that, res);
    // Free memory
    VecFloatFree(&res);
  }
}

// Return a VecFloat equal to 'that' rotated CCW by 'theta' radians
// Return NULL if arguments are invalid
VecFloat* VecFloatGetRot2D(VecFloat *that, float theta) {
  // Check argument
  if (that == NULL || that->_dim != 2)
    return NULL;
  // Declare a variable to memorize the result 
  VecFloat *res = VecFloatCreate(that->_dim);
  // If we could allocate memory
  if (res != NULL) {
    // Calculate
    res->_val[0] = 
      cos(theta) * that->_val[0] - sin(theta) * that->_val[1];
    res->_val[1] = 
      sin(theta) * that->_val[0] + cos(theta) * that->_val[1];
  }
  // Return the result
  return res;
}

// Return the dot product of 'that' and 'tho'
// Return 0.0 if arguments are invalid
float VecFloatDotProd(VecFloat *that, VecFloat *tho) {
  // Check arguments
  if (that == NULL || tho == NULL || that->_dim != tho->_dim)
    return 0.0;
  // Declare a variable to memorize the result
  float res = 0.0;
  // Calculate
  for (int iDim = that->_dim; iDim--;)
    res += that->_val[iDim] * tho->_val[iDim];
  // Return the result
  return res;
}

// Return the angle of the rotation making 'that' colinear to 'tho'
// Return 0.0 if arguments are invalid
float VecFloatAngleTo2D(VecFloat *that, VecFloat *tho) {
  // Check arguments
  if (that == NULL || tho == NULL || 
    VecDim(that) != 2 || VecDim(tho) != 2)
    return 0.0;
  // Declare a variable to memorize the result
  float theta = 0.0;
  // Calculate the angle
  VecFloat *v = VecClone(that);
  if (v == NULL)
    return 0.0;
  VecFloat *w = VecClone(tho);
  if (w == NULL) {
    VecFree(&v);
    return 0.0;
  }
  if (VecNorm(v) < PBMATH_EPSILON || VecNorm(v) < PBMATH_EPSILON) {
    VecFree(&v);
    VecFree(&w);
    return 0.0;
  }
  VecNormalise(v);
  VecNormalise(w);
  float m[2];
  if (fabs(VecGet(v, 0)) > fabs(VecGet(v, 1))) {
    m[0] = (VecGet(w, 0) + VecGet(w, 1) * VecGet(v, 1) / VecGet(v, 0))/
      (VecGet(v, 0) + pow(VecGet(v, 1), 2.0) / VecGet(v, 0));
    m[1] = (m[0] * VecGet(v, 1) - VecGet(w, 1)) / VecGet(v, 0);
  } else {
    m[1] = (VecGet(w, 0) - VecGet(w, 1) * VecGet(v, 0) / VecGet(v, 1))/
      (VecGet(v, 1) + pow(VecGet(v, 0), 2.0) / VecGet(v, 1));
    m[0] = (VecGet(w, 1) + m[1] * VecGet(v, 0)) / VecGet(v, 1);
  }
  // Due to numerical imprecision m[0] may be slightly out of [-1,1]
  // which makes acos return NaN, prevent this
  if (m[0] < -1.0)
    theta = PBMATH_PI;
  else if (m[0] > 1.0)
    theta = 0.0;
  else
    theta = acos(m[0]);
  if (sin(theta) * m[1] > 0.0)
    theta *= -1.0;
  // Free memory
  VecFree(&v);
  VecFree(&w);
  // Return the result
  return theta;
}

// -------------- MatFloat

// ================= Define ==================

// ================ Functions implementation ====================

// Create a new MatFloat of dimension 'dim' (nbcol, nbline)
// Values are initalized to 0.0, 'dim' must be a VecShort of dimension 2
// Return NULL if we couldn't create the MatFloat
MatFloat* MatFloatCreate(VecShort *dim) {
  // Check argument
  if (dim == NULL || VecDim(dim) != 2)
    return NULL;
  // Allocate memory
  MatFloat *that = (MatFloat*)malloc(sizeof(MatFloat));
  //If we could allocate memory
  if (that != NULL) {
    // Set the dimension
    that->_dim = VecClone(dim);
    if (that->_dim == NULL) {
      // Free memory
      free(that);
      // Stop here
      return NULL;
    }
    // Allocate memory for values
    int d = VecGet(dim, 0) * VecGet(dim, 1);
    that->_val = (float*)malloc(sizeof(float) * d);
    // If we couldn't allocate memory
    if (that->_val == NULL) {
      // Free memory
      free(that);
      // Stop here
      return NULL;
    }
    // Set the default values
    for (int i = d; i--;)
      that->_val[i] = 0.0;
  }
  // Return the new MatFloat
  return that;
}

// Set the MatFloat to the identity matrix
// The matrix must be a square matrix
// Do nothing if arguments are invalid
void MatFloatSetIdentity(MatFloat *that) {
  // Check argument
  if (that == NULL || VecGet(that->_dim, 0) != VecGet(that->_dim, 1))
    return;
  // Set the values
  VecShort *i = VecShortCreate(2);
  if (i != NULL) {
    for (VecSet(i, 0, 0); VecGet(i, 0) < VecGet(that->_dim, 0);
      VecSet(i, 0, VecGet(i, 0) + 1)) {
      for (VecSet(i, 1, 0); VecGet(i, 1) < VecGet(that->_dim, 1);
        VecSet(i, 1, VecGet(i, 1) + 1)) {
        if (VecGet(i, 0) == VecGet(i, 1))
          MatSet(that, i, 1.0);
        else
          MatSet(that, i, 0.0);
      }
    }
  }
  VecFree(&i);
}

// Clone the MatFloat
// Return NULL if we couldn't clone the MatFloat
MatFloat* MatFloatClone(MatFloat *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Create a clone
  MatFloat *clone = MatFloatCreate(that->_dim);
  // If we could create the clone
  if (clone != NULL) {
    // Clone the properties
    VecCopy(clone->_dim, that->_dim);
    int d = VecGet(that->_dim, 0) * VecGet(that->_dim, 1);
    for (int i = d; i--;)
      clone->_val[i] = that->_val[i];
  }
  // Return the clone
  return clone;
}

// Load the MatFloat from the stream
// If the MatFloat is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
int MatFloatLoad(MatFloat **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    MatFloatFree(that);
  }
  // Read the number of dimension
  int dim[2];
  int ret = fscanf(stream, "%d %d", dim , dim + 1);
  // If we coudln't fscanf
  if (ret == EOF)
    return 4;
  if (dim[0] <= 0 || dim[1] <= 0)
    return 3;
  // Allocate memory
  VecShort *d = VecShortCreate(2);
  VecSet(d, 0, dim[0]);
  VecSet(d, 1, dim[1]);
  *that = MatFloatCreate(d);
  // If we coudln't allocate memory
  if (*that == NULL)
    return 2;
  // Read the values
  int nbVal = dim[0] * dim[1];
  for (int i = 0; i < nbVal; ++i) {
    ret = fscanf(stream, "%f", (*that)->_val + i);
    // If we coudln't fscanf
    if (ret == EOF)
      return 4;
  }
  // Free memory
  VecFree(&d);
  // Return success code
  return 0;
}

// Save the MatFloat to the stream
// Return 0 upon success, or
// 1: invalid arguments
// 2: fprintf error
int MatFloatSave(MatFloat *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the dimension
  int ret = fprintf(stream, "%d %d\n", VecGet(that->_dim, 0), 
    VecGet(that->_dim, 1));
  // If we coudln't fprintf
  if (ret < 0)
    return 2;
  // Save the values
  for (int i = 0; i < VecGet(that->_dim, 1); ++i) {
    for (int j = 0; j < VecGet(that->_dim, 0); ++j) {
      ret = fprintf(stream, "%f ", 
        that->_val[i * VecGet(that->_dim, 0) + j]);
      // If we coudln't fprintf
      if (ret < 0)
        return 2;
    }
    ret = fprintf(stream, "\n");
    // If we coudln't fprintf
    if (ret < 0)
      return 2;
  }
  // Return success code
  return 0;
}

// Free the memory used by a MatFloat
// Do nothing if arguments are invalid
void MatFloatFree(MatFloat **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  VecFree(&((*that)->_dim));
  free((*that)->_val);
  free(*that);
  *that = NULL;
}

// Print the MatFloat on 'stream' with 'prec' digit precision
// Do nothing if arguments are invalid
void MatFloatPrint(MatFloat *that, FILE *stream, int prec) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return;
  // Create the format string
  char format[20] = {'\0'};
  sprintf(format, "%%.%df", prec);
  // Print the values
  fprintf(stream, "[");
  for (int i = 0; i < VecGet(that->_dim, 1); ++i) {
    if (i > 0)
      fprintf(stream, " ");
    for (int j = 0; j < VecGet(that->_dim, 0); ++j) {
      fprintf(stream, format, 
        that->_val[i * VecGet(that->_dim, 0) + j]);
      if (j < VecGet(that->_dim, 0) - 1)
        fprintf(stream, ",");
    }
    if (i < VecGet(that->_dim, 1) - 1)
      fprintf(stream, "\n");
  }
  fprintf(stream, "]");
}
void MatFloatPrintDef(MatFloat *that, FILE *stream) {
  MatFloatPrint(that, stream, 3);
}

// Return the value at index 'i' of the MatFloat
// Index starts at 0, i must be a VecShort of dimension 2
// Return 0.0 if arguments are invalid
float MatFloatGet(MatFloat *that, VecShort *i) {
  // Check argument
  if (that == NULL || i == NULL || VecDim(i) != 2 || 
    VecGet(i, 0) < 0 || VecGet(i, 0) >= VecGet(that->_dim, 0) ||
    VecGet(i, 1) < 0 || VecGet(i, 1) >= VecGet(that->_dim, 1))
    return 0.0;
  // Return the value
  return 
    that->_val[VecGet(i, 1) * VecGet(that->_dim, 0) + VecGet(i, 0)];
}

// Set the value at index 'i' of the MatFloat to 'v'
// Index starts at 0, 'i' must be a VecShort of dimension 2
// Do nothing if arguments are invalid
void MatFloatSet(MatFloat *that, VecShort *i, float v) {
  // Check argument
  if (that == NULL || i == NULL || VecDim(i) != 2 || 
    VecGet(i, 0) < 0 || VecGet(i, 0) >= VecGet(that->_dim, 0) ||
    VecGet(i, 1) < 0 || VecGet(i, 1) >= VecGet(that->_dim, 1))
    return;
  // Set the value
  that->_val[VecGet(i, 1) * VecGet(that->_dim, 0) + VecGet(i, 0)] = v;
}

// Return a VecShort of dimension 2 containing the dimension of 
// the MatFloat
// Return NULL if arguments are invalid
VecShort* MatFloatDim(MatFloat *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Return the dimension
  return VecClone(that->_dim);
}

// Return the inverse matrix of 'that'
// The matrix must be a square matrix
// Return null if arguments are invalids
MatFloat* MatFloatInv(MatFloat *that) {
  // Check arguments
  if (that == NULL || VecGet(that->_dim, 0) != VecGet(that->_dim, 1))
    return NULL;
  // Allocate memory for the pivot
  VecShort *pivot = VecShortCreate(2);
  if (pivot == NULL)
    return NULL;
  // Allocate memory for the result
  MatFloat *res = MatFloatCreate(that->_dim);
  // If we could allocate memory
  if (res != NULL) {
    // If the matrix is of dimension 1x1
    if (VecGet(that->_dim, 0) == 1) {
      MatSet(res, pivot, 1.0 / MatGet(that, pivot));
    // Else, the matrix dimension is greater than 1x1
    } else {
      // Set the result to the identity
      MatSetIdentity(res);
      // Clone the original matrix
      MatFloat *copy = MatClone(that);
      // If we couldn't clone
      if (copy == NULL) {
        MatFree(&res);
        return NULL;
      }
      // Allocate memory for the index to manipulate the matrix
      VecShort *index = VecShortCreate(2);
      // If we couldn't allocate memory
      if (index == NULL) {
        MatFree(&res);
        MatFree(&copy);
        return NULL;
      }
      // For each pivot
      for (VecSet(pivot, 0, 0), VecSet(pivot, 1, 0);
        VecGet(pivot, 0) < VecGet(that->_dim, 0);
        VecSet(pivot, 0, VecGet(pivot, 0) + 1), 
        VecSet(pivot, 1, VecGet(pivot, 1) + 1)) {
        // If the pivot is null
        if (MatGet(copy, pivot) < PBMATH_EPSILON) {
          // Search a line where the value under the pivot is not null
          VecCopy(index, pivot);
          VecSet(index, 1, 0);
          while (VecGet(index, 1) < VecGet(that->_dim, 1) &&
            fabs(MatGet(copy, index)) < PBMATH_EPSILON)
            VecSet(index, 1, VecGet(index, 1) + 1);
          // If there is no line where the pivot is not null
          if (VecGet(index, 1) >= VecGet(that->_dim, 1)) {
            // The system has no solution
            // Free memory
            MatFree(&copy);
            VecFree(&index);
            MatFree(&res);
            MatFree(&copy);
            // Stop here
            return NULL;
          }
          // Add the line where the pivot is not null to the line
          // of the pivot to un-nullify it
          VecSet(index, 0, 0);
          VecSet(pivot, 0, 0);
          while (VecGet(index, 0) < VecGet(that->_dim, 0)) {
            MatSet(copy, pivot, 
              MatGet(copy, pivot) + MatGet(copy, index));
            MatSet(res, pivot, 
              MatGet(res, pivot) + MatGet(res, index));
            VecSet(index, 0, VecGet(index, 0) + 1);
            VecSet(pivot, 0, VecGet(pivot, 0) + 1);
          }
          // Reposition the pivot
          VecSet(pivot, 0, VecGet(pivot, 1));
        }
        // Divide the values by the pivot
        float p = MatGet(copy, pivot);
        VecSet(pivot, 0, 0);
        while (VecGet(pivot, 0) < VecGet(that->_dim, 0)) {
          MatSet(copy, pivot, MatGet(copy, pivot) / p);
          MatSet(res, pivot, MatGet(res, pivot) / p);
          VecSet(pivot, 0, VecGet(pivot, 0) + 1);
        }
        // Reposition the pivot
        VecSet(pivot, 0, VecGet(pivot, 1));
        // Nullify the values below the pivot
        VecSet(pivot, 0, 0);
        VecSet(index, 1, VecGet(pivot, 1) + 1);
        while (VecGet(index, 1) < VecGet(that->_dim, 1)) {
          VecSet(index, 0, VecGet(pivot, 1));
          p = MatGet(copy, index);
          VecSet(index, 0, 0);
          while (VecGet(index, 0) < VecGet(that->_dim, 0)) {
            MatSet(copy, index, 
              MatGet(copy, index) - MatGet(copy, pivot) * p);
            MatSet(res, index, 
              MatGet(res, index) - MatGet(res, pivot) * p);
            VecSet(pivot, 0, VecGet(pivot, 0) + 1);
            VecSet(index, 0, VecGet(index, 0) + 1);
          }
          VecSet(pivot, 0, 0);
          VecSet(index, 0, 0);
          VecSet(index, 1, VecGet(index, 1) + 1);
        }
        // Reposition the pivot
        VecSet(pivot, 0, VecGet(pivot, 1));
      }
      // Now the matrix is triangular, move back through the pivots
      // to make it diagonal
      for (; VecGet(pivot, 0) >= 0;
        VecSet(pivot, 0, VecGet(pivot, 0) - 1), 
        VecSet(pivot, 1, VecGet(pivot, 1) - 1)) {
        // Nullify the values above the pivot by substracting the line
        // of the pivot
        VecSet(pivot, 0, 0);
        VecSet(index, 1, VecGet(pivot, 1) - 1);
        while (VecGet(index, 1) >= 0) {
          VecSet(index, 0, VecGet(pivot, 1));
          float p = MatGet(copy, index);
          VecSet(index, 0, 0);
          while (VecGet(index, 0) < VecGet(that->_dim, 0)) {
            MatSet(copy, index, 
              MatGet(copy, index) - MatGet(copy, pivot) * p);
            MatSet(res, index, 
              MatGet(res, index) - MatGet(res, pivot) * p);
            VecSet(pivot, 0, VecGet(pivot, 0) + 1);
            VecSet(index, 0, VecGet(index, 0) + 1);
          }
          VecSet(pivot, 0, 0);
          VecSet(index, 0, 0);
          VecSet(index, 1, VecGet(index, 1) - 1);
        }
        // Reposition the pivot
        VecSet(pivot, 0, VecGet(pivot, 1));
      }
      // Free memory
      MatFree(&copy);
      VecFree(&index);
    }
  }
  // Free memory
  VecShortFree(&pivot);
  // Return the result
  return res;
}

// Return the product of matrix 'that' and vector 'v'
// Number of colum of 'that' must equal dimension of 'v'
// Return null if arguments are invalids
VecFloat* MatFloatProdVecFloat(MatFloat *that, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL || VecGet(that->_dim, 0) != VecDim(v))
    return NULL;
  // Declare a variable to memorize the index in the matrix
  VecShort *i = VecShortCreate(2);
  if (i == NULL)
    return NULL;
  // Allocate memory for the solution
  VecFloat *ret = VecFloatCreate(VecGet(that->_dim, 1));
  // If we could allocate memory
  if (ret != NULL) {
    for (VecSet(i, 0, 0); VecGet(i, 0) < VecGet(that->_dim, 0);
      VecSet(i, 0, VecGet(i, 0) + 1)) {
      for (VecSet(i, 1, 0); VecGet(i, 1) < VecGet(that->_dim, 1);
        VecSet(i, 1, VecGet(i, 1) + 1)) {
        VecSet(ret, VecGet(i, 1), VecGet(ret, 
          VecGet(i, 1)) + VecGet(v, VecGet(i, 0)) * MatGet(that, i));
      }
    }
  }
  // Free memory
  VecFree(&i);
  // Return the result
  return ret;
}

// Return the product of matrix 'that' by matrix 'tho'
// Number of columns of 'that' must equal number of line of 'tho'
// Return null if arguments are invalids
MatFloat* MatFloatProdMatFloat(MatFloat *that, MatFloat *tho) {
  // Check arguments
  if (that == NULL || tho == NULL || 
    VecGet(that->_dim, 0) != VecGet(tho->_dim, 1))
    return NULL;
  // Declare 3 variables to memorize the index in the matrix
  VecShort *i = VecShortCreate(2);
  if (i == NULL)
    return NULL;
  VecShort *j = VecShortCreate(2);
  if (j == NULL) {
    VecFree(&i);
    return NULL;
  }
  VecShort *k = VecShortCreate(2);
  if (k == NULL) {
    VecFree(&i);
    VecFree(&j);
    return NULL;
  }
  // Allocate memory for the solution
  VecSet(i, 0, VecGet(tho->_dim, 0));
  VecSet(i, 1, VecGet(that->_dim, 1));
  MatFloat *ret = MatFloatCreate(i);
  // If we could allocate memory
  if (ret != NULL) {
    for (VecSet(i, 0, 0); VecGet(i, 0) < VecGet(tho->_dim, 0);
      VecSet(i, 0, VecGet(i, 0) + 1)) {
      for (VecSet(i, 1, 0); VecGet(i, 1) < VecGet(that->_dim, 1);
        VecSet(i, 1, VecGet(i, 1) + 1)) {
        for (VecSet(j, 0, 0), VecSet(j, 1, VecGet(i, 1)),
          VecSet(k, 0, VecGet(i, 0)), VecSet(k, 1, 0); 
          VecGet(j, 0) < VecGet(that->_dim, 0);
          VecSet(j, 0, VecGet(j, 0) + 1), 
          VecSet(k, 1, VecGet(k, 1) + 1)) {
          MatSet(ret, i, MatGet(ret, i) + 
            MatGet(that, j) * MatGet(tho, k));
        }
      }
    }
  }
  // Free memory
  VecFree(&i);
  VecFree(&j);
  VecFree(&k);
  // Return the result
  return ret;
}

// -------------- Gauss

// ================= Define ==================

// ================ Functions implementation ====================

// Create a new Gauss of mean 'mean' and sigma 'sigma'
// Return NULL if we couldn't create the Gauss
Gauss* GaussCreate(float mean, float sigma) {
  // Allocate memory
  Gauss *that = (Gauss*)malloc(sizeof(Gauss));
  // If we could allocate memory
  if (that != NULL) {
    // Set properties
    that->_mean = mean;
    that->_sigma = sigma;
  }
  // REturn the new Gauss
  return that;
}

// Free the memory used by a Gauss
// Do nothing if arguments are invalid
void GaussFree(Gauss **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  free(*that);
  *that = NULL;
}

// Return the value of the Gauss 'that' at 'x'
// Return 0.0 if the arguments are invalid
float GaussGet(Gauss *that, float x) {
  // Check arguments
  if (that == NULL)
    return 0.0;
  // Calculate the value
  float a = 1.0 / (that->_sigma * sqrt(2.0 * PBMATH_PI));
  float ret = a * exp(-1.0 * pow(x - that->_mean, 2.0) / 
    (2.0 * pow(that->_sigma, 2.0)));
  // Return the value
  return ret;
}

// Return a random value (in ]0.0, 1.0[)according to the 
// Gauss distribution 'that'
// random() must have been called before calling this function
// Return 0.0 if the arguments are invalid
float GaussRnd(Gauss *that) {
  // Check arguments
  if (that == NULL)
    return 0.0;
  // Declare variable for calcul
  float v1,v2,s;
  // Calculate the value
  do {
    v1 = (rnd() - 0.5) * 2.0;
    v2 = (rnd() - 0.5) * 2.0;
    s = v1 * v1 + v2 * v2;
  } while (s >= 1.0);
  // Return the value
  float ret = 0.0;
  if (s > PBMATH_EPSILON)
    ret = v1 * sqrt(-2.0 * log(s) / s);
  return ret * that->_sigma + that->_mean;
}

// -------------- Smoother

// ================= Define ==================

// ================ Functions implementation ====================

// Return the order 1 smooth value of 'x'
// if x < 0.0 return 0.0
// if x > 1.0 return 1.0
float SmoothStep(float x) {
  if (x <= 0.0)
    return 0.0;
  else if (x >= 1.0)
    return 1.0;
  else
    return x * x * (3.0 - 2.0 * x);
}

// Return the order 2 smooth value of 'x'
// if x < 0.0 return 0.0
// if x > 1.0 return 1.0
float SmootherStep(float x) {
  if (x <= 0.0)
    return 0.0;
  else if (x >= 1.0)
    return 1.0;
  else
    return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}

// -------------- Shapoid

// ================= Define ==================

const char *ShapoidTypeString[4] = {
  (const char*)"InvalidShapoid", (const char*)"Facoid", 
  (const char*)"Spheroid", (const char*)"Pyramidoid"};

// ================ Functions implementation ====================

// Create a Shapoid of dimension 'dim' and type 'type', default values:
// _pos = null vector
// _axis[d] = unit vector along dimension d
// Return NULL if arguments are invalid or malloc failed
Shapoid* ShapoidCreate(int dim, ShapoidType type) {
  // Check argument
  if (dim < 0 || type == ShapoidTypeInvalid)
    return NULL;
  // Allocate memory
  Shapoid *that = (Shapoid*)malloc(sizeof(Shapoid));
  // If we could allocate memory
  if (that != NULL) {
    // Set the dimension and type
    that->_type = type;
    that->_dim = dim;
    // Allocate memory for position
    that->_pos = VecFloatCreate(dim);
    // If we couldn't allocate memory
    if (that->_pos == NULL) {
      free(that);
      return NULL;
    }
    // Allocate memory for array of axis
    that->_axis = (VecFloat**)malloc(sizeof(VecFloat*) * dim);
    if (that->_axis == NULL) {
      free(that);
      return NULL;
    }
    // Allocate memory for each axis
    for (int iAxis = 0; iAxis < dim; ++iAxis) {
      // Allocate memory for position
      that->_axis[iAxis] = VecFloatCreate(dim);
      // If we couldn't allocate memory
      if (that->_axis[iAxis] == NULL) {
        ShapoidFree(&that);
        return NULL;
      }
      // Set value of the axis
      VecSet(that->_axis[iAxis], iAxis, 1.0);
    }
  }
  return that;
}

// Clone a Shapoid
// Return NULL if couldn't clone
Shapoid* ShapoidClone(Shapoid *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Create a clone
  Shapoid *clone = ShapoidCreate(that->_dim, that->_type);
  if (clone != NULL) {
    // Set the position and axis of the clone
    ShapoidSetPos(clone, that->_pos);
    for (int iAxis = clone->_dim; iAxis--;)
      ShapoidSetAxis(clone, iAxis, that->_axis[iAxis]);
  }
  // Return the clone
  return clone;
}

// Free memory used by a Shapoid
// Do nothing if arguments are invalid
void ShapoidFree(Shapoid **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  for (int iAxis = 0; iAxis < (*that)->_dim; ++iAxis)
    VecFree((*that)->_axis + iAxis);
  free((*that)->_axis);
  VecFree(&((*that)->_pos));
  free(*that);
  *that = NULL;
}

// Load the Shapoid from the stream
// If the VecFloat is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
int ShapoidLoad(Shapoid **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    ShapoidFree(that);
  }
  // Read the dimension and type
  int dim;
  int ret = fscanf(stream, "%d", &dim);
  // If we coudln't fscanf
  if (ret == EOF)
    return 4;
  if (dim <= 0)
    return 3;
  ShapoidType type;
  ret = fscanf(stream, "%u", &type);
  // If we coudln't fscanf
  if (ret == EOF)
    return 4;
  // Allocate memory
  *that = ShapoidCreate(dim, type);
  // If we coudln't allocate memory
  if (*that == NULL) {
    return 2;
  }
  // Read the values
  ret = VecFloatLoad(&((*that)->_pos), stream);
  if (ret != 0)
    return ret;
  for (int iAxis = 0; iAxis < dim; ++iAxis) {
    ret = VecFloatLoad((*that)->_axis + iAxis, stream);
    if (ret != 0)
      return ret;
  }
  // Return success code
  return 0;
}

// Save the Shapoid to the stream
// Return 0 upon success, or
// 1: invalid arguments
// 2: fprintf error
int ShapoidSave(Shapoid *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the dimension and type
  int ret = fprintf(stream, "%d %u\n", that->_dim, that->_type);
  // If we coudln't fprintf
  if (ret < 0)
    return 2;
  // Save the position and axis
  ret = VecFloatSave(that->_pos, stream);
  if (ret != 0)
    return ret;
  for (int iAxis = 0; iAxis < that->_dim; ++iAxis) {
    ret = VecFloatSave(that->_axis[iAxis], stream);
    if (ret != 0)
      return ret;
  }
  // Return success code
  return 0;
}

// Print the Shapoid on 'stream'
// Do nothing if arguments are invalid
void ShapoidPrint(Shapoid *that, FILE *stream) {
  // Check arugments
  if (that == NULL || stream == NULL)
    return;
  // Print the Shapoid
  fprintf(stream, "Type: %s\n", ShapoidTypeString[that->_type]);
  fprintf(stream, "Dim: %d\n", that->_dim);
  fprintf(stream, "Pos: ");
  VecPrint(that->_pos, stream);
  fprintf(stream, "\n");
  for (int iAxis = 0; iAxis < that->_dim; ++iAxis) {
    fprintf(stream, "Axis(%d): ", iAxis);
    VecPrint(that->_axis[iAxis], stream);
    fprintf(stream, "\n");
  }
}

// Get the dimension of the Shapoid
// Return 0 if arguments are invalid
int ShapoidGetDim(Shapoid *that) {
  // Check arugments
  if (that == NULL)
    return 0;
  // Return the dimension
  return that->_dim;
}

// Get the dimension of the Shapoid
// Return 0 if arguments are invalid
ShapoidType ShapoidGetType(Shapoid *that) {
  // Check arugments
  if (that == NULL)
    return 0;
  // Return the type
  return that->_type;
}

// Get the type of the Shapoid as a string
// Return a pointer to a constant string (not to be freed)
// Return the string for ShapoidTypeInvalid if arguments are invalid
const char* ShapoidGetTypeAsString(Shapoid *that) {
  // Check arugments
  if (that == NULL)
    return ShapoidTypeString[ShapoidTypeInvalid];
  // Return the type
  return ShapoidTypeString[that->_type];
}

// Return a VecFloat equal to the position of the Shapoid
// Return NULL if arguments are invalid
VecFloat* ShapoidGetPos(Shapoid *that) {
  // Check arugments
  if (that == NULL)
    return NULL;
  // Return a clone of the position
  return VecClone(that->_pos);
}

// Return a VecFloat equal to the 'dim'-th axis of the Shapoid
// Return NULL if arguments are invalid
VecFloat* ShapoidGetAxis(Shapoid *that, int dim) {
  // Check arugments
  if (that == NULL || dim < 0 || dim >= that->_dim)
    return NULL;
  // Return a clone of the axis
  return VecClone(that->_axis[dim]);
}

// Set the position of the Shapoid to 'pos'
// Do nothing if arguments are invalid
void ShapoidSetPos(Shapoid *that, VecFloat *pos) {
  // Check arugments
  if (that == NULL || pos == NULL)
    return;
  // Set the position
  VecCopy(that->_pos, pos);
}

// Set the 'dim'-th axis of the Shapoid to 'v'
// Do nothing if arguments are invalid
void ShapoidSetAxis(Shapoid *that, int dim, VecFloat *v) {
  // Check arugments
  if (that == NULL || v == NULL)
    return;
  // Set the axis
  VecCopy(that->_axis[dim], v);
}

// Translate the Shapoid by 'v'
// Do nothing if arguments are invalid
void ShapoidTranslate(Shapoid *that, VecFloat *v) {
  // Check arugments
  if (that == NULL || v == NULL)
    return;
  // Translate the position
  VecOp(that->_pos, 1.0, v, 1.0);
}

// Scale the Shapoid by 'v' (each axis is multiplied by v[iAxis])
// Do nothing if arguments are invalid
void ShapoidScale(Shapoid *that, VecFloat *v) {
  // Check arugments
  if (that == NULL || v == NULL)
    return;
  // Scale each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecOp(that->_axis[iAxis], VecGet(v, iAxis), NULL, 0.0);
}

// Rotate the Shapoid of dimension 2 by 'theta'
// Do nothing if arguments are invalid
void ShapoidRotate2D(Shapoid *that, float theta) {
  // Check arugments
  if (that == NULL)
    return;
  // Rotate each axis
  for (int iAxis = that->_dim; iAxis--;)
    VecRot2D(that->_axis[iAxis], theta);
}

// Convert the coordinates of 'pos' from standard coordinate system 
// toward the Shapoid coordinates system
// Return null if the arguments are invalid
VecFloat* ShapoidImportCoord(Shapoid *that, VecFloat *pos) {
  // Check arguments
  if (that == NULL || pos == NULL || ShapoidGetDim(that) != VecDim(pos))
    return NULL;
  // Create a matrix for the linear system solver
  VecShort *dim = VecShortCreate(2);
  if (dim == NULL)
    return NULL;
  VecSet(dim, 0, that->_dim);
  VecSet(dim, 1, that->_dim);
  MatFloat *mat = MatFloatCreate(dim);
  if (mat == NULL) {
    VecFree(&dim);
    return NULL;
  }
  // Set the values of the matrix
  for (VecSet(dim, 0, 0); VecGet(dim, 0) < that->_dim; 
    VecSet(dim, 0, VecGet(dim, 0) + 1)) {
    for (VecSet(dim, 1, 0); VecGet(dim, 1) < that->_dim; 
      VecSet(dim, 1, VecGet(dim, 1) + 1)) {
      MatSet(mat, dim, VecGet(that->_axis[VecGet(dim, 0)], 
        VecGet(dim, 1)));
    }
  }
  VecFloat *v = VecGetOp(pos, 1.0, that->_pos, -1.0);
  if (v == NULL) {
    VecFree(&dim);
    MatFree(&mat);
    return NULL;
  }
  // Create the linear system solver and solve it
  EqLinSys *sys = EqLinSysCreate(mat, v);
  VecFloat *res = EqLinSysSolve(sys);
  // Free memory
  VecFree(&v);
  VecFree(&dim);
  EqLinSysFree(&sys);
  MatFree(&mat);
  // return the result
  return res;
}

// Convert the coordinates of 'pos' from the Shapoid coordinates system 
// toward standard coordinate system
// Return null if the arguments are invalid
VecFloat* ShapoidExportCoord(Shapoid *that, VecFloat *pos) {
  // Check arguments
  if (that == NULL || pos == NULL || ShapoidGetDim(that) != VecDim(pos))
    return NULL;
  // Allocate memory for the result
  VecFloat *res = VecClone(that->_pos);
  // If we could allocate memory
  if (res != NULL) {
    for (int dim = that->_dim; dim--;)
      VecOp(res, 1.0, that->_axis[dim], VecGet(pos, dim));
  }
  // Return the result
  return res;
}

// Return true if 'pos' is inside the Shapoid
// Else return false
bool ShapoidIsPosInside(Shapoid *that, VecFloat *pos) {
  // Check arguments
  if (that == NULL || pos == NULL || ShapoidGetDim(that) != VecDim(pos))
    return false;
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat *coord = ShapoidImportCoord(that, pos);
  // If we couldn't get the coordinates
  if (coord == NULL)
    // Stop here
    return false;
  // Declare a variable to memorize the result
  bool ret = false;
  // If the Shapoid is a Facoid
  if (that->_type == ShapoidTypeFacoid) {
    // pos is in the Shapoid if all the coord in Shapoid coord 
    // system are in [0.0, 1.0]
    ret = true;
    for (int dim = that->_dim; dim-- && ret == true;) {
      float v = VecGet(coord, dim);
      if (v < 0.0 || v > 1.0)
        ret = false;
    }
  // Else, if the Shapoid is a Pyramidoid
  } else if (that->_type == ShapoidTypePyramidoid) {
    // pos is in the Shapoid if all the coord in Shapoid coord 
    // system are in [0.0, 1.0] and their sum is in [0.0, 1.0]
    ret = true;
    float sum = 0.0;
    for (int dim = that->_dim; dim-- && ret == true;) {
      float v = VecGet(coord, dim);
      sum += v;
      if (v < 0.0 || v > 1.0)
        ret = false;
    }
    if (ret == true && sum > 1.0)
      ret = false;
  // Else, if the Shapoid is a Spheroid
  } else if (that->_type == ShapoidTypeSpheroid) {
    // pos is in the Shapoid if its norm is in [0.0, 0.5]
    float norm = VecNorm(coord);
    if (norm <= 0.5)
      ret = true;
  }
  // Free memory
  VecFloatFree(&coord);
  // Return the result
  return ret;
}

// Get a bounding box of the Shapoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
// Return null if the argument are invalid.
Shapoid* ShapoidGetBoundingBoxThat(Shapoid *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Declare a variable to memorize the result
  Shapoid *res = FacoidCreate(ShapoidGetDim(that));
  if (res != NULL) {
    // If the Shapoid is a Facoid
    if (that->_type == ShapoidTypeFacoid) {
      // For each axis
      for (int dim = that->_dim; dim--;) {
        // Declare a variable to memorize the bound of the interval on 
        // this axis
        float bound[2];
        bound[0] = bound[1] = VecGet(that->_pos, dim);
        // For each parameter
        for (int param = that->_dim; param--;) {
          // Get the value of the axis influencing the current dimension
          float v = VecGet(that->_axis[param], dim);
          // If the value is negative, update the minimum bound
          if (v < 0.0)
            bound[0] += v;
          // Else, if the value is negative, update the minimum bound
          else
            bound[1] += v;
        }
        // Memorize the result
        VecSet(res->_pos, dim, bound[0]);
        VecSet(res->_axis[dim], dim, bound[1] - bound[0]);
      }
    // Else, if the Shapoid is a Pyramidoid
    } else  if (that->_type == ShapoidTypePyramidoid) {
      // For each axis
      for (int dim = that->_dim; dim--;) {
        // Declare a variable to memorize the bound of the interval on 
        // this axis
        float bound[2];
        bound[0] = bound[1] = VecGet(that->_axis[0], dim);
        // For each parameter
        for (int param = that->_dim; param--;) {
          // Get the value of the axis influencing the current dimension
          float v = VecGet(that->_axis[param], dim);
          // Search the min and max values
          if (v < bound[0])
            bound[0] = v;
          if (v > bound[1])
            bound[1] = v;
        }
        // Memorize the result
        if (bound[0] < 0.0) {
          VecSet(res->_pos, dim, VecGet(that->_pos, dim) + bound[0]);
          VecSet(res->_axis[dim], dim, bound[1] - bound[0]);
        } else {
          VecSet(res->_pos, dim, VecGet(that->_pos, dim));
          VecSet(res->_axis[dim], dim, bound[1]);
        }
      }
    // Else, if the Shapoid is a Spheroid
    } else  if (that->_type == ShapoidTypeSpheroid) {
      // In case of a Spheroid, things get complicate
      // We'll approximate the bounding box of the Spheroid 
      // with the one of the same Spheroid viewed as a Facoid
      // and simply take care that the _pos is at the center of the 
      // Spheroid
      // For each axis
      for (int dim = that->_dim; dim--;) {
        // Declare a variable to memorize the bound of the interval on 
        // this axis
        float bound[2];
        bound[0] = VecGet(that->_pos, dim);
        // Correct position
        // For each parameter
        for (int param = that->_dim; param--;) {
          // Get the value of the axis influencing the current dimension
          float v = VecGet(that->_axis[param], dim);
          // Correct the pos
          bound[0] -= 0.5 * v;
        }
        bound[1] = bound[0];
        // For each parameter
        for (int param = that->_dim; param--;) {
          // Get the value of the axis influencing the current dimension
          float v = VecGet(that->_axis[param], dim);
          // If the value is negative, update the minimum bound
          if (v < 0.0)
            bound[0] += v;
          // Else, if the value is negative, update the minimum bound
          else
            bound[1] += v;
        }
        // Memorize the result
        VecSet(res->_pos, dim, bound[0]);
        VecSet(res->_axis[dim], dim, bound[1] - bound[0]);
      }
    } else {
      // In any case of invalid shapoid type return NULL
      ShapoidFree(&res);
    }
  }
  // Return the result
  return res;
}

// Get the bounding box of a set of Facoid. The bounding box is aligned
// on the standard coordinate system (its axis are colinear with
// the axis of the standard coordinate system).
// The bounding box is returned as a Facoid, which position is
// at the minimum value along each axis.
// Return null if the arguments are invalid or the shapoid in the set
// don't have all the same dimension.
Shapoid* ShapoidGetBoundingBoxSet(GSet *set) {
  // Check arguments
  if (set == NULL)
    return NULL;
  // Declare a variable for the result
  Shapoid *res = NULL;
  // Declare a pointer to the elements of the set
  GSetElem *elem = set->_head;
  // Loop on element of the set
  while (elem != NULL) {
    // Declare a pointer to the Facoid
    Shapoid *shapoid = (Shapoid*)(elem->_data);
    // If it's the first Facoid in the set
    if (res == NULL) {
      // Get the bounding box of this shapoid
      res = ShapoidGetBoundingBox(shapoid);
      // If we couldn't get the bounding box
      if (res == NULL)
        return NULL;
    // Else, this is not the first Shapoid in the set
    } else {
      // Ensure the Facoids have all the same dimension
      if (shapoid->_dim != res->_dim) {
        ShapoidFree(&res);
        return NULL;
      }
      // Get the bounding box of this shapoid
      Shapoid *bound = ShapoidGetBoundingBox(shapoid);
      // If we couldn't get the bounding box
      if (bound == NULL) {
        ShapoidFree(&res);
        return NULL;
      }
      // For each dimension
      for (int iDim = res->_dim; iDim--;) {
        // Update the bounding box
        if (VecGet(bound->_pos, iDim) < VecGet(res->_pos, iDim))
          VecSet(res->_pos, iDim, VecGet(bound->_pos, iDim));
        if (VecGet(bound->_pos, iDim) + 
          VecGet(bound->_axis[iDim], iDim) >
          VecGet(res->_pos, iDim) + 
          VecGet(res->_axis[iDim], iDim))
          VecSet(res->_axis[iDim], iDim, 
          VecGet(bound->_pos, iDim) + 
          VecGet(bound->_axis[iDim], iDim) -
          VecGet(res->_pos, iDim));
      }
      // Free memory used by the bounding box
      ShapoidFree(&bound);
    }
    // Move to the next element
    elem = elem->_next;
  }
  // Return the result
  return res;
}

// Get a GSet of BCurves approximating the Shapoid 'that'
// 'that' must be of dimension 2
// Return null if arguments are invalid
GSet* ShapoidGetApproxBCurve2D(Shapoid *that) {
  // Check arguments
  if (that == NULL || ShapoidGetDim(that) != 2)
    return NULL;
  // Declare a GSet to memorize the result
  GSet *set = GSetCreate();
  // If the shapoid is a Facoid
  if (ShapoidGetType(that) == ShapoidTypeFacoid) {
    VecFloat *A = VecGetOp(that->_pos, 1.0, that->_axis[0], 1.0);
    VecFloat *B = VecGetOp(that->_pos, 1.0, that->_axis[1], 1.0);
    VecFloat *C = VecGetOp(A, 1.0, that->_axis[1], 1.0);
    BCurve *curve = NULL;
    if (A != NULL && B != NULL && C != NULL) {
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, that->_pos);
        BCurveSet(curve, 1, A);
        GSetAppend(set, curve);
      }
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, A);
        BCurveSet(curve, 1, C);
        GSetAppend(set, curve);
      }
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, C);
        BCurveSet(curve, 1, B);
        GSetAppend(set, curve);
      }
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, B);
        BCurveSet(curve, 1, that->_pos);
        GSetAppend(set, curve);
      }
    }
    VecFree(&A);
    VecFree(&B);
    VecFree(&C);
  // Else, if the shapoid is a Pyramidoid
  } else if (ShapoidGetType(that) == ShapoidTypePyramidoid) {
    VecFloat *A = VecGetOp(that->_pos, 1.0, that->_axis[0], 1.0);
    VecFloat *B = VecGetOp(that->_pos, 1.0, that->_axis[1], 1.0);
    BCurve *curve = NULL;
    if (A != NULL && B != NULL) {
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, that->_pos);
        BCurveSet(curve, 1, A);
        GSetAppend(set, curve);
      }
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, A);
        BCurveSet(curve, 1, B);
        GSetAppend(set, curve);
      }
      curve = BCurveCreate(1, 2);
      if (curve != NULL) {
        BCurveSet(curve, 0, B);
        BCurveSet(curve, 1, that->_pos);
        GSetAppend(set, curve);
      }
    }
    VecFree(&A);
    VecFree(&B);
  // Else, if the shapoid is a Spheroid
  } else if (ShapoidGetType(that) == ShapoidTypeSpheroid) {
    // Approximate each quarter of the Spheroid with BCurves
    // Declare a variable to memorize the angular position on the 
    // Spheroid surface
    float theta = 0.0;
    // Declare a variable to memorize the delta of angular position
    float deltaTheta = PBMATH_HALFPI / 3.0;
    // Declare a GSet to memorize the 4 points of the point cloud
    // used to calculate the BCurve approximating the quarter of
    // Spheroid
    GSet *pointCloud = GSetCreate();
    if (pointCloud != NULL) {
      // Loop until we have made a full turn around the Spheroid
      for (int iCurve = 4; iCurve--;) {
        // For each point of the point cloud
        for (int iPoint = 4; iPoint--;) {
          // Declare a variable to memorize the coordinates of the point
          VecFloat *point = VecFloatCreate(2);
          // If we could allocate memory
          if (point != NULL) {
            // Calculate the coordinates of the current point in the
            // Spheroid coordinate system
            VecSet(point, 0, 0.5 * cos(theta));
            VecSet(point, 1, 0.5 * sin(theta));
            // Add the point converted to standard coordinate system 
            // to the point cloud
            VecFloat *pointConvert = ShapoidExportCoord(that, point);
            GSetAppend(pointCloud, pointConvert);
            VecFree(&point);
          }
          // Increment the angular position
          theta += deltaTheta;
        }
        BCurve *curve = BCurveFromCloudPoint(pointCloud);
        // If we could get the BCurve
        if (curve != NULL)
          // Append the curve to the set of curve to be drawm
          GSetAppend(set, curve);
        // Free memory
        GSetElem *elem = pointCloud->_head;
        while (elem != NULL) {
          VecFloatFree((VecFloat**)(&(elem->_data)));
          elem = elem->_next;
        }
        // Empty the point cloud
        GSetFlush(pointCloud);
        // We need to decrement theta because the first point of the
        // next curve will be the last point of the current curve
        theta -= deltaTheta;
      }
      GSetFree(&pointCloud);
    }
  }
  // Return the GSet containing the result
  return set;
}

// Get the depth value in the Shapoid of 'pos'
// The depth is defined as follow: the point with depth equals 1.0 is 
// the farthest point from the surface of the Shapoid (inside it),
// points with depth equals to 0.0 are point on the surface of the
// Shapoid. Depth is continuous and derivable over the volume of the
// Shapoid
// Return 0.0 if arguments are invalid, or pos is outside the Shapoid
float ShapoidGetPosDepth(Shapoid *that, VecFloat *pos) {
  // Check arguments
  if (that == NULL || pos == NULL || ShapoidGetDim(that) != VecDim(pos))
    return 0.0;
  // Get the coordinates of pos in the Shapoid coordinate system
  VecFloat *coord = ShapoidImportCoord(that, pos);
  // If we couldn't get the coordinates
  if (coord == NULL)
    // Stop here
    return 0.0;
  // Declare a variable to memorize the result
  float ret = 0.0;
  // If the Shapoid is a Facoid
  if (that->_type == ShapoidTypeFacoid) {
    ret = 1.0;
    for (int dim = that->_dim; dim-- && ret > PBMATH_EPSILON;) {
      float v = VecGet(coord, dim);
      if (v < 0.0 || VecGet(coord, dim) > 1.0)
        ret = 0.0;
      else
        ret *= 1.0 - pow(0.5 - v, 2.0) * 4.0;
    }
  // Else, if the Shapoid is a Pyramidoid
  } else if (that->_type == ShapoidTypePyramidoid) {
    ret = 1.0;
    float sum = 0.0;
    bool flag = true;
    for (int dim = that->_dim; dim-- && ret > PBMATH_EPSILON;) {
      float v = VecGet(coord, dim);
      sum += v;
      if (v < 0.0 || v > 1.0)
        flag = false;
    }
    if (flag == true && sum > 1.0)
      flag = false;
    if (flag == false)
      ret = 0.0;
    else {
      ret = 1.0;
      for (int dim = ShapoidGetDim(that); dim--;) {
        float z = 0.0;
        for (int d = ShapoidGetDim(that); d--;)
          if (d != dim)
            z += VecGet(coord, d);
        ret *= 
          (1.0 - 4.0 * pow(0.5 - VecGet(coord, dim) / (1.0 - z), 2.0));
      }
    }
  // Else, if the Shapoid is a Spheroid
  } else if (that->_type == ShapoidTypeSpheroid) {
    float norm = VecNorm(coord);
    if (norm <= 0.5)
      ret = 1.0 - norm * 2.0;
  }
  // Free memory
  VecFloatFree(&coord);
  // Return the result
  return ret;
}

// Get the center of the shapoid in standard coordinate system
// Return null if arguments are invalid
VecFloat* ShapoidGetCenter(Shapoid *that) {
  // Check arguments
  if (that == NULL)
    return NULL;
  // Declare a variable to memorize the result in Shapoid 
  // coordinate system
  VecFloat *coord = VecFloatCreate(ShapoidGetDim(that));
  // If we could allocate memory
  if (coord != NULL) {
    // For each dimension
    for (int dim = ShapoidGetDim(that); dim--;) {
      if (ShapoidGetType(that) == ShapoidTypeFacoid)
        VecSet(coord, dim, 0.5);
      else if (ShapoidGetType(that) == ShapoidTypePyramidoid)
        VecSet(coord, dim, 1.0 / (1.0 + ShapoidGetDim(that)));
      else if (ShapoidGetType(that) == ShapoidTypeSpheroid)
        VecSet(coord, dim, 0.0);
    }
  }
  // Convert the cooridnate in standard cooridnate system
  VecFloat *res = ShapoidExportCoord(that, coord);
  // Return the result
  return res;
}

// -------------- Conversion functions

// ================ Functions implementation ====================

// Convert radians to degrees
float ConvRad2Deg(float rad) {
  return 360.0 * rad / PBMATH_TWOPI;
}

// Convert degrees to radians
float ConvDeg2Rad(float deg) {
  return PBMATH_TWOPI * deg / 360.0;
}

// -------------- EqLinSys

// ================ Functions implementation ====================

// Create a new EqLinSys with matrix 'm' and vector 'v'
// The dimension of 'v' must be equal to the number of column of 'm'
// The matrix 'm' must be a square matrix
// Return NULL if we couldn't create the EqLinSys
EqLinSys* EqLinSysCreate(MatFloat *m, VecFloat *v) {
  // Check arguments
  if (m == NULL || v == NULL || VecGet(m->_dim, 0) != VecDim(v) ||
    VecGet(m->_dim, 0) != VecGet(m->_dim, 1))
    return NULL;
  // Allocate memory
  EqLinSys *that = (EqLinSys*)malloc(sizeof(EqLinSys));
  // If we could allocate memory
  if (that != NULL) {
    that->_M = MatClone(m);
    that->_V = VecClone(v);
    if (that->_M == NULL || that->_V == NULL) {
      EqLinSysFree(&that);
      return NULL;
    }
  }
  // Return the new EqLinSys
  return that;
}

// Free the memory used by the EqLinSys
// Do nothing if arguments are invalid
void EqLinSysFree(EqLinSys **that) {
  // Check arguments
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  MatFree(&((*that)->_M));
  VecFree(&((*that)->_V));
  free(*that);
  *that = NULL;
}

// Solve the EqLinSys _M.x = _V
// Return the solution vector, or null if there is no solution or the 
// arguments are invalid
VecFloat* EqLinSysSolve(EqLinSys *that) {
  // Check the argument
  if (that == NULL)
    return NULL;
  // Declare a variable to memorize the solution
  VecFloat *ret = NULL;
  // Inverse the matrix
  MatFloat *inv = MatInv(that->_M);
  // If we could inverse the matrix
  if (inv != NULL) {
    // Calculate the solution
    ret = MatProd(inv, that->_V);
  }
  // Free memory
  MatFree(&inv);
  // Return the solution vector
  return ret;
}

