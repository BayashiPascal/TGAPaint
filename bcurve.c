// ============ BCURVE.C ================

// ================= Include =================

#include "bcurve.h"

// ================= Define ==================

// ================ Functions implementation ====================

// Create a new BCurve of order 'order' and dimension 'dim'
// Return NULL if we couldn't create the BCurve
BCurve* BCurveCreate(int order, int dim) {
  // Check arguments
  if (order < 0 || dim < 1)
    return NULL;
  // Allocate memory
  BCurve *that = (BCurve*)malloc(sizeof(BCurve));
  //If we could allocate memory
  if (that != NULL) {
    // Set the values
    that->_dim = dim;
    that->_order = order;
    // Allocate memory for the array of control points
    that->_ctrl = (VecFloat**)malloc(sizeof(VecFloat*) * (order + 1));
    // If we couldn't allocate memory
    if (that->_ctrl == NULL) {
      // Free memory
      free(that);
      // Stop here
      return NULL;
    }
    // For each control point
    for (int iCtrl = 0; iCtrl < order + 1; ++iCtrl) {
      // Allocate memory
      that->_ctrl[iCtrl] = VecFloatCreate(dim);
      // If we couldn't allocate memory
      if (that->_ctrl[iCtrl] == NULL) {
        // Free memory
        BCurveFree(&that);
        // Stop here
        return NULL;
      }
    }
  }
  // Return the new BCurve
  return that;
}

// Clone the BCurve
// Return NULL if we couldn't clone the BCurve
BCurve* BCurveClone(BCurve *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Allocate memory for the clone
  BCurve *clone = (BCurve*)malloc(sizeof(BCurve));
  // If we could allocate memory
  if (clone != NULL) {
    // Clone the properties
    clone->_dim = that->_dim;
    clone->_order = that->_order;
    // Allocate memory for the array of control points
    clone->_ctrl = (VecFloat**)malloc(sizeof(VecFloat*) * 
      (clone->_order + 1));
    // If we couldn't allocate memory
    if (that->_ctrl == NULL) {
      // Free memory
      free(clone);
      // Stop here
      return NULL;
    }
    // For each control point
    for (int iCtrl = 0; iCtrl < clone->_order + 1; ++iCtrl) {
      // Clone the control point
      clone->_ctrl[iCtrl] = VecClone(that->_ctrl[iCtrl]);
      // If we couldn't clone the control point
      if (clone->_ctrl[iCtrl] == NULL) {
        // Free memory
        BCurveFree(&clone);
        // Stop here
        return NULL;
      }
    }
  }
  // Return the clone
  return clone;
}

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
// 5: VecLoad error
int BCurveLoad(BCurve **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    BCurveFree(that);
  }
  // Read the order and dimension
  int order;
  int dim;
  int ret = fscanf(stream, "%d %d", &order, &dim);
  // If we couldn't read
  if (ret == EOF) {
    return 4;
  }
  // Allocate memory
  *that = BCurveCreate(order, dim);
  // If we coudln't allocate memory
  if (*that == NULL) {
    return 2;
  }
  // For each control point
  for (int iCtrl = 0; iCtrl < (order + 1); ++iCtrl) {
    // Load the control point
    ret = VecLoad((*that)->_ctrl + iCtrl, stream);
    // If we couldn't read the control point or the conrtol point
    // is not of the correct dimension
    if (ret != 0 || VecDim((*that)->_ctrl[iCtrl]) != (*that)->_dim) {
      // Free memory
      BCurveFree(that);
      // Stop here
      return 5;
    }
  }
  // Return success code
  return 0;
}

// Save the BCurve to the stream
// Return 0 upon success, or
// 1: invalid arguments
// 2: fprintf error
// 3: VecSave error
int BCurveSave(BCurve *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the order and dimension
  int ret = fprintf(stream, "%d %d\n", that->_order, that->_dim);
  // If the fprintf failed
  if (ret < 0) {
    // Stop here
    return 2;
  }
  // For each control point
  for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl) {
    // Save the control point
    ret = VecSave(that->_ctrl[iCtrl], stream);
    // If we couldn't save the control point
    if (ret != 0) {
      // Stop here
      return 3;
    }
  }
  // Return success code
  return 0;
}

// Free the memory used by a BCurve
// Do nothing if arguments are invalid
void BCurveFree(BCurve **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // If there are control points
  if ((*that)->_ctrl != NULL) {
    // For each control point
    for (int iCtrl = 0; iCtrl < (*that)->_order + 1; ++iCtrl) {
      // Free the control point
      VecFree((*that)->_ctrl + iCtrl);
    }
  }
  // Free the array of control points
  free((*that)->_ctrl);
  // Free memory
  free(*that);
  *that = NULL;
}

// Print the BCurve on 'stream'
// Do nothing if arguments are invalid
void BCurvePrint(BCurve *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return;
  // Print the order and dim
  fprintf(stream, "order(%d) dim(%d) ", that->_order, that->_dim);
  // For each control point
  for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl) {
    VecPrint(that->_ctrl[iCtrl], stream);
    fprintf(stream, " ");
  }
}

// Set the value of the iCtrl-th control point to v
// Do nothing if arguments are invalid
void BCurveSet(BCurve *that, int iCtrl, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL || iCtrl < 0 || 
    iCtrl > that->_order || VecDim(v) != BCurveDim(that))
    return;
  // Set the values
  VecCopy(that->_ctrl[iCtrl], v);
}

// Get the value of the BCurve at paramater 'u' (in [0.0, 1.0])
// Return NULL if arguments are invalid or malloc failed
// if 'u' < 0.0 it is replaced by 0.0
// if 'u' > 1.0 it is replaced by 1.0
VecFloat* BCurveGet(BCurve *that, float u) {
  // Check arguments
  if (that == NULL)
    return NULL;
  if (u < 0.0)
    u = 0.0;
  if (u > 1.0)
    u = 1.0;
  // Allocate memory for the result
  VecFloat *v = VecFloatCreate(that->_dim);
  // If we couldn't allocate memory
  if (v == NULL)
    return NULL;
  // Declare a variable for calcul
  float *val = (float*)malloc(sizeof(float) * (that->_order + 1));
  // Loop on dimension
  for (int dim = that->_dim; dim--;) {
    // Initialise the temporary variable with the value in current
    // dimension of the control points
    for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl)
      val[iCtrl] = VecGet(that->_ctrl[iCtrl], dim);
    // Loop on order
    int subOrder = that->_order;
    while (subOrder != 0) {
      // Loop on sub order
      for (int order = 0; order < subOrder; ++order) {
        val[order] = (1.0 - u) * val[order] + u * val[order + 1];
      }
      --subOrder;
    }
    // Set the value for the current dim
    VecSet(v, dim, val[0]);
  }
  // Free memory
  free(val);
  // Return the result
  return v;
}

// Get the order of the BCurve
// Return -1 if argument is invalid
int BCurveOrder(BCurve *that) {
  // Check arguments
  if (that == NULL)
    return -1;
  return that->_order;
}

// Get the dimension of the BCurve
// Return 0 if argument is invalid
int BCurveDim(BCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0;
  return that->_dim;
}

// Get the approximate length of the BCurve (sum of dist between
// control points)
// Return 0.0 if argument is invalid
float BCurveApproxLen(BCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0.0;
  // Declare a variable to calculate the length
  float res = 0.0;
  // Calculate the length
  for (int iCtrl = 0; iCtrl < that->_order; ++iCtrl)
    res += VecDist(that->_ctrl[iCtrl], that->_ctrl[iCtrl + 1]);
  // Return the length
  return res;
}

// Rotate the curve CCW by 'theta' radians relatively to the origin
// Do nothing if arguments are invalid
void BCurveRot2D(BCurve *that, float theta) {
  // Check arguments
  if (that == NULL || that->_dim != 2)
    return;
  // For each control point
  for (int iCtrl = 0; iCtrl <= that->_order; ++iCtrl) {
    // Rotate the control point
    VecRot2D(that->_ctrl[iCtrl], theta);
  }
}

