// *************** TGAFONT.C ***************

// ================ Functions declaration ====================

// Create the curves of each characters for the default font
void TGAFontCreateDefault(TGAFont *font);

// Get the next position form 'p' incremented by one tabulation
// of 'font'
float TGAFontGetNextPosByTab(TGAFont *font, float p);

// ================ Functions implementation ==================

// Create a TGAFont with set of character 'font', 
// _fontSize = 18.0, _space[0] = _space[1] = 3.0, 
// _scale[0] = 0.5, _scale[1] = 1.0, _anchor = tgaFrontAnchorTopLeft
// _dir = <1.0, 0.0>, _tabSize = _fontSize
// Return NULL if it couldn't create
TGAFont* TGAFontCreate(tgaFont font) {
  // Allocate memory
  TGAFont *ret = (TGAFont*)malloc(sizeof(TGAFont));
  // If we could allocate memory
  if (ret != NULL) {
    // Set the default size
    ret->_size = 18.0;
    // Set the default tab size
    ret->_tabSize = ret->_size;
    // Set the default space
    ret->_space = VecFloatCreate(2);
    if (ret->_space == NULL) {
      free(ret);
      return NULL;
    }
    VecSet(ret->_space, 0, 3.0);
    VecSet(ret->_space, 1, 3.0);
    // Set the default scale
    ret->_scale = VecFloatCreate(2);
    if (ret->_scale == NULL) {
      VecFree(&(ret->_space));
      free(ret);
      return NULL;
    }
    VecSet(ret->_scale, 0, 1.0);
    VecSet(ret->_scale, 1, 1.0);
    // Set the default anchor
    ret->_anchor = tgaFontAnchorTopLeft;
    // Set the default orientation
    ret->_right = VecFloatCreate(2);
    if (ret->_right == NULL) {
      VecFree(&(ret->_space));
      VecFree(&(ret->_scale));
      free(ret);
      return NULL;
    }
    VecSet(ret->_right, 0, 1.0);
    VecSet(ret->_right, 1, 0.0);
    // For each character
    for (int iChar = 256; iChar--;) {
      // By default set this character definition as empty (no curves)
      ret->_char[iChar]._curve = SCurveCreate(2);
      if (ret->_char[iChar]._curve == NULL) {
        VecFree(&(ret->_space));
        VecFree(&(ret->_scale));
        VecFree(&(ret->_right));
        free(ret);
        return NULL;
      }
    }
    // If the requested font is the default one
    if (font == tgaFontDefault)
      // Create the default font characters' curves
      TGAFontCreateDefault(ret);
  }
  // Return the created font
  return ret;
}

// Free memory used by TGAFont
// Do nothing if arguments are invalid
void TGAFreeFont(TGAFont **font) {
  // If the argument are invalid, stop here
  if (font == NULL || *font == NULL)
    return;
  // Free the memory
  for (int iChar = 256; iChar--;)
    SCurveFree(&((*font)->_char[iChar]._curve));
  VecFree(&((*font)->_scale));
  VecFree(&((*font)->_space));
  VecFree(&((*font)->_right));
  free(*font);
  *font = NULL;
}

// Set the font size of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSize(TGAFont *font, float v) {
  if (font == NULL || v <= 0.0)
    return;
  font->_size = v;
}

// Set the font tab size of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetTabSize(TGAFont *font, float v) {
  if (font == NULL || v <= 0.0)
    return;
  font->_tabSize = v;
}

// Set the font scale of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetScale(TGAFont *font, VecFloat *v) {
  // If the argument are invalid, stop here
  if (font == NULL || v == NULL)
    return;
  // Set the scale
  VecCopy(font->_scale, v);
}

// Set the font spacing of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSpace(TGAFont *font, VecFloat *v) {
  // If the argument are invalid, stop here
  if (font == NULL || v == NULL)
    return;
  // Set the space
  VecCopy(font->_space, v);
}

// Set the anchor of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetAnchor(TGAFont *font, tgaFontAnchor v) {
  // If the argument are invalid, stop here
  if (font == NULL)
    return;
  // Set the anchor
  font->_anchor = v;  
}

// Set the right direction of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetRight(TGAFont *font, VecFloat *v) {
  // If the argument are invalid, stop here
  if (font == NULL || v == NULL)
    return;
  // Set the right direction
  VecCopy(font->_right, v); 
  // Ensure its normalized
  VecNormalise(font->_right);
}

// Get the next position form 'p' incremented by one tabulation
// of 'font'
float TGAFontGetNextPosByTab(TGAFont *font, float p) {
  return (floor(p / font->_tabSize) + 1.0) * font->_tabSize;
}

// Get the angle of the right vector of the font with the abciss
// Return 0.0 if the arguments are invalid or memory allocation failed
float TGAFontGetAngleWithAbciss(TGAFont *font) {
  if (font == NULL)
    return 0.0;
  VecFloat *abciss = VecFloatCreate(2);
  if (abciss == NULL)
    return 0.0;
  VecSet(abciss, 0, 1.0); VecSet(abciss, 1, 0.0);
  float theta = VecAngleTo2D(abciss, font->_right);
  VecFree(&abciss);
  return theta;
}

// Get the bounding box as a facoid of order 2 and dim 2 in pixels
// of the block of text representing string 's' printed with 'font'
// Return NULL if arguments are invalid
Shapoid* TGAFontGetStringBound(TGAFont *font, unsigned char *s) {
  // Check arguments
  if (font == NULL)
    return NULL;
  // Declare a variable to memorize the height of lines and the max
  // width of a line in pixels
  VecFloat *dim = VecFloatCreate(2);
  // If we couldn't allocate memory
  if (dim == NULL)
    return NULL;
  // Declare a variable for the result
  Shapoid *res = FacoidCreate(2);
  // If we couldn't allocate memory
  if (res == NULL)
    return NULL;
  // Declare a variable to memorize the total heights of the lines
  float height = 0.0;
  // If the string is not empty
  if (s != NULL) {
    // Initialise the dimensions
    VecSet(dim, 0, 0.0);
    VecSet(dim, 1, font->_size * VecGet(font->_scale, 1));
    // Declare a variable to memorize the length of the current line
    float l = 0.0;
    // Declare a variable to memorize if we are at the beginning 
    // of the line
    bool flagStart = true;
    // For each character
    int nb = strlen((char*)s);
    for (int iChar = 0; iChar < nb; ++iChar) {
      // If this character is a line return
      if (s[iChar] == '\n') {
        // Increment height
        float h = font->_size * VecGet(font->_scale, 1) + 
          VecGet(font->_space, 1);
        height += h;
        VecSet(dim, 1, VecGet(dim, 1) + h);
        // Reset the length of line
        l = 0.0;
        // Reset the flag 
        flagStart = true;
      // Else, if this character is a tabulation
      } else if (s[iChar] == '\t') {
        // Increment length to the next tab
        l = TGAFontGetNextPosByTab(font, l);
        // If the current line is longer than the longest one
        if (VecGet(dim, 0) < l)
          // Update the length of the
          VecSet(dim, 0, l);
      // Else, for others character
      } else {
        // If it's not the first char
        if (flagStart == false)
          // Add the space between character
          l += VecGet(font->_space, 0);
        // Update the flag of beginning of line
        flagStart = false;
        // Increment the length of the current line
        l += font->_size * VecGet(font->_scale, 0);
        // If the current line is longer than the longest one
        if (VecGet(dim, 0) < l)
          // Update the length
          VecSet(dim, 0, l);
      }
    }
  }
  // Scale the Facoid
  ShapoidScale(res, dim);
  // Reposition the Facoid according to the anchor
  switch (font->_anchor) {
    case tgaFontAnchorTopLeft:
      VecSet(res->_pos, 1, VecGet(res->_pos, 1) - VecGet(dim, 1));
      break;
    case tgaFontAnchorTopCenter:
      VecSet(res->_pos, 1, VecGet(res->_pos, 1) - VecGet(dim, 1));
      VecSet(res->_pos, 0, -0.5 * VecGet(dim, 0));
      break;
    case tgaFontAnchorTopRight:
      VecSet(res->_pos, 1, VecGet(res->_pos, 1) - VecGet(dim, 1));
      VecSet(res->_pos, 0, -1.0 * VecGet(dim, 0));
      break;
    case tgaFontAnchorCenterLeft:
      VecSet(res->_pos, 1, 
        VecGet(res->_pos, 1) - 0.5 * VecGet(dim, 1));
      break;
    case tgaFontAnchorCenterCenter:
      VecSet(res->_pos, 1, 
        VecGet(res->_pos, 1) - 0.5 * VecGet(dim, 1));
      VecSet(res->_pos, 0, -0.5 * VecGet(dim, 0));
      break;
    case tgaFontAnchorCenterRight:
      VecSet(res->_pos, 1, 
        VecGet(res->_pos, 1) - 0.5 * VecGet(dim, 1));
      VecSet(res->_pos, 0, -1.0 * VecGet(dim, 0));
      break;
    case tgaFontAnchorBottomLeft:
      break;
    case tgaFontAnchorBottomCenter:
      VecSet(res->_pos, 0, -0.5 * VecGet(dim, 0));
      break;
    case tgaFontAnchorBottomRight:
      VecSet(res->_pos, 0, -1.0 * VecGet(dim, 0));
      break;
    default:
      break;
  }
  // Rotate the Facoid
  float theta = TGAFontGetAngleWithAbciss(font);
  ShapoidRotate2D(res, theta);
  // The rotation must also be applied to the position which may be
  // not at the origin
  VecRot2D(res->_pos, theta);
  // Free memory
  VecFloatFree(&dim);
  // Return the result
  return res;
}

// Function to initialize the curves of one char
void TGAFontInitChar(TGAChar *ch, int nbCurve, float *c) {
  BCurve *curve = BCurveCreate(3, 2);
  if (curve != NULL) {
    for (int iCurve = nbCurve; iCurve--;) {
      for (int iCtrl = 4; iCtrl--;)
        for (int dim = 2; dim--;)
          VecSet(curve->_ctrl[iCtrl], dim, 
            c[iCurve * 8 + iCtrl * 2 + dim]);
      SCurveAdd(ch->_curve, curve);
    }
  }
  BCurveFree(&curve);
}

// Create the curves of each characters for the default font
void TGAFontCreateDefault(TGAFont *font) {
  TGAChar *ch = NULL;
  ch = font->_char + 'A';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.0,0.0,0.0,0.18,0.32,1.0,0.5,1.0,
        0.5,1.0,0.68,1.0,1.0,0.18,1.0,0.0,
        0.15,0.5,0.15,0.5,0.85,0.5,0.85,0.5
    });
  ch = font->_char + 'B';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.77,1.00,0.77,0.58,0.00,0.59,
        0.00,0.59,0.50,0.60,1.01,0.50,1.00,0.26,
        1.00,0.26,1.00,0.00,0.50,0.00,0.00,0.00
    });
  ch = font->_char + 'C';
  TGAFontInitChar(ch, 4,
    (float[]){
        1.00,0.67,1.00,0.82,1.00,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.81,0.00,0.50,
        0.00,0.50,0.00,0.18,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.17,1.00,0.33
    });
  ch = font->_char + 'D';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00,
        0.00,0.00,1.00,0.00,1.00,0.00,1.00,0.50,
        1.00,0.50,1.00,1.00,0.50,1.00,0.00,1.00,
        0.00,1.00,-0.11,1.00,0.00,0.00,0.00,0.00,
        0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'E';
  TGAFontInitChar(ch, 5,
    (float[]){
        1.00,1.00,1.00,1.00,0.12,1.01,0.06,0.95,
        0.06,0.95,-0.01,0.90,0.00,0.10,0.05,0.05,
        0.05,0.05,0.11,-0.01,1.00,0.00,1.00,0.00,
        1.00,0.00,1.00,0.00,0.00,0.00,0.00,0.00,
        0.00,0.50,0.00,0.50,0.50,0.50,0.50,0.50
    });
  ch = font->_char + 'F';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.50,0.00,0.50,0.50,0.50,0.50,0.50,
        1.00,1.00,1.00,1.00,0.12,1.01,0.06,0.95,
        0.06,0.95,-0.01,0.90,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'G';
  TGAFontInitChar(ch, 5,
    (float[]){
        1.00,0.84,1.00,1.00,0.74,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.81,0.00,0.50,
        0.00,0.50,0.00,0.18,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.50,1.00,0.50,
        1.00,0.50,1.00,0.50,0.50,0.50,0.50,0.50
    });
  ch = font->_char + 'H';
  TGAFontInitChar(ch, 3,
    (float[]){
        1.00,1.00,1.00,1.00,1.00,0.00,1.00,0.00,
        0.00,0.50,0.00,0.50,1.00,0.50,1.00,0.50,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'I';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00,
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00,
        0.10,1.00,0.10,1.00,0.90,1.00,0.90,1.00
    });
  ch = font->_char + 'J';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.66,1.00,0.66,1.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.33,0.00,0.50,
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00
    });
  ch = font->_char + 'K';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.50,0.54,0.50,0.00,1.00,0.00,1.00,0.00,
        0.00,0.50,0.00,0.50,0.00,0.50,0.33,0.50,
        0.33,0.50,0.67,0.51,1.00,1.00,1.00,1.00,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'L';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,1.00,0.00,1.00,0.00,0.12,0.05,0.05,
        0.05,0.05,0.08,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'M';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.00,1.00,0.34,0.67,0.50,0.67,
        0.50,0.67,0.66,0.67,1.00,1.00,1.00,1.00,
        1.00,1.00,1.00,1.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'N';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.33,1.00,0.66,0.00,1.00,0.00,
        1.00,0.00,1.00,0.00,1.00,1.00,1.00,1.00
    });
  ch = font->_char + 'O';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.50,1.00,1.00,1.00,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.50,
        0.00,0.50,0.00,1.00,0.00,1.00,0.50,1.00
    });
  ch = font->_char + 'P';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.50,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.33,0.50,0.33,0.00,0.33
    });
  ch = font->_char + 'Q';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.66,0.33,0.66,0.33,1.00,0.00,1.00,0.00,
        0.50,1.00,1.00,1.00,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.50,
        0.00,0.50,0.00,1.00,0.00,1.00,0.50,1.00
    });
  ch = font->_char + 'R';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.33,0.33,0.00,1.00,0.00,1.00,0.00,
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.50,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.33,0.50,0.33,0.00,0.33
    });
  ch = font->_char + 'S';
  TGAFontInitChar(ch, 5,
    (float[]){
        1.00,0.83,1.00,0.99,1.00,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.83,0.00,0.67,
        0.00,0.67,0.00,0.50,1.00,0.67,1.00,0.50,
        1.00,0.50,1.00,0.33,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.16,0.00,0.33
    });
  ch = font->_char + 'T';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00,
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00
    });
  ch = font->_char + 'U';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,1.00,0.00,0.50,0.01,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.51,1.00,1.00
    });
  ch = font->_char + 'V';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,1.00,0.00,1.00,0.34,0.00,0.50,0.00,
        0.50,0.00,0.67,0.00,1.00,1.00,1.00,1.00
    });
  ch = font->_char + 'W';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,1.00,0.00,1.00,0.16,0.00,0.33,0.00,
        0.33,0.00,0.50,0.00,0.50,0.50,0.50,0.50,
        0.50,0.50,0.50,0.50,0.50,0.00,0.66,0.00,
        0.66,0.00,0.82,0.00,1.00,1.00,1.00,1.00
    });
  ch = font->_char + 'X';
  TGAFontInitChar(ch, 4,
    (float[]){
        1.00,1.00,1.00,1.00,0.50,0.67,0.50,0.51,
        0.50,0.51,0.50,0.33,0.00,0.00,0.00,0.00,
        0.00,1.00,0.00,1.00,0.50,0.67,0.50,0.50,
        0.50,0.50,0.50,0.33,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'Y';
  TGAFontInitChar(ch, 3,
    (float[]){
        1.00,1.00,1.00,1.00,0.50,0.67,0.50,0.50,
        0.00,1.00,0.00,1.00,0.50,0.67,0.50,0.50,
        0.50,0.50,0.50,0.33,0.50,0.00,0.50,0.00
    });
  ch = font->_char + 'Z';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00,
        1.00,1.00,1.00,0.67,0.00,0.33,0.00,0.00,
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + '0';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.00,0.00,0.00,0.00,1.00,1.00,1.00,1.00,
        0.50,1.00,1.00,1.00,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.50,
        0.00,0.50,0.00,1.00,0.00,1.00,0.50,1.00
    });
  ch = font->_char + '1';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00,
        0.00,0.67,0.33,0.67,0.50,1.00,0.50,1.00,
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00
    });
  ch = font->_char + '2';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.67,0.00,1.00,0.34,1.00,0.50,1.00,
        0.50,1.00,0.66,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.50,0.00,0.33,0.00,0.00,
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + '3';
  TGAFontInitChar(ch, 6,
    (float[]){
        0.00,0.67,0.00,0.83,0.00,1.00,0.50,1.00,
        0.50,1.00,1.00,1.00,1.00,0.83,1.00,0.67,
        1.00,0.67,1.00,0.50,0.50,0.50,0.50,0.50,
        0.50,0.50,0.50,0.50,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.16,0.00,0.33
    });
  ch = font->_char + '4';
  TGAFontInitChar(ch, 3,
    (float[]){
        1.00,0.33,1.00,0.33,0.00,0.33,0.00,0.33,
        0.00,0.33,0.50,0.50,0.66,1.00,0.66,1.00,
        0.66,1.00,0.66,1.00,0.66,0.00,0.66,0.00
    });
  ch = font->_char + '5';
  TGAFontInitChar(ch, 5,
    (float[]){
        1.00,1.00,1.00,1.00,0.33,1.00,0.33,1.00,
        0.33,1.00,0.33,1.00,0.00,0.67,0.00,0.67,
        0.00,0.67,0.00,0.67,1.00,1.01,1.00,0.33,
        1.00,0.33,1.00,0.00,0.67,0.00,0.50,0.00,
        0.50,0.00,0.33,0.00,0.00,0.16,0.00,0.33
    });
  ch = font->_char + '6';
  TGAFontInitChar(ch, 6,
    (float[]){
        0.00,0.33,0.00,0.50,0.33,0.50,0.50,0.50,
        0.50,0.50,0.67,0.50,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.16,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.33,0.00,0.50,
        0.00,0.50,0.00,1.00,0.50,1.00,0.50,1.00,
        0.50,1.00,0.50,1.00,1.00,1.00,1.00,0.67
    });
  ch = font->_char + '7';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00,
        1.00,1.00,1.00,1.00,0.33,0.67,0.33,0.00
    });
  ch = font->_char + '8';
  TGAFontInitChar(ch, 6,
    (float[]){
        0.50,1.00,1.00,1.00,1.00,0.67,0.50,0.67,
        0.50,0.67,0.33,0.67,0.00,0.50,0.00,0.33,
        0.00,0.33,0.00,0.00,0.33,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.00,1.00,0.33,
        1.00,0.33,1.00,0.50,0.66,0.67,0.50,0.67,
        0.50,0.67,0.00,0.67,0.00,1.00,0.50,1.00
    });
  ch = font->_char + '9';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.33,0.00,0.50,0.00,1.00,0.00,1.00,0.50,
        1.00,0.50,1.00,1.00,0.66,1.00,0.50,1.00,
        0.50,1.00,0.33,1.00,0.00,1.00,0.00,0.67,
        0.00,0.67,0.00,0.50,0.33,0.50,0.50,0.50,
        0.50,0.50,0.67,0.50,1.00,0.50,1.00,0.67
    });
  ch = font->_char + '!';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.50,0.18,0.44,0.18,0.44,0.07,0.50,0.07,
        0.50,0.07,0.56,0.07,0.56,0.18,0.50,0.18,
        0.50,1.00,0.50,1.00,0.50,0.33,0.50,0.33
    });
  ch = font->_char + '"';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.66,1.00,0.66,1.00,0.66,0.75,0.66,0.75,
        0.33,1.00,0.33,1.00,0.33,0.75,0.33,0.75
    });
  ch = font->_char + '\'';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.25,1.00,0.25,1.00,0.25,0.49,0.00,0.50
    });
  ch = font->_char + '#';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.75,1.00,0.75,1.00,0.66,0.00,0.66,0.00,
        0.33,1.00,0.33,1.00,0.25,0.00,0.25,0.00,
        0.00,0.25,0.00,0.25,1.00,0.25,1.00,0.25,
        0.00,0.67,0.00,0.67,1.00,0.67,1.00,0.67
    });
  ch = font->_char + '$';
  TGAFontInitChar(ch, 6,
    (float[]){
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00,
        1.00,0.83,1.00,0.99,1.00,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.83,0.00,0.67,
        0.00,0.67,0.00,0.50,1.00,0.67,1.00,0.50,
        1.00,0.50,1.00,0.33,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.16,0.00,0.33
    });
  ch = font->_char + '%';
  TGAFontInitChar(ch, 9,
    (float[]){
        0.75,0.50,1.00,0.50,1.00,0.50,1.00,0.25,
        1.00,0.25,1.00,0.00,1.00,0.00,0.75,0.00,
        0.75,0.00,0.50,0.00,0.50,0.00,0.50,0.25,
        0.50,0.25,0.50,0.50,0.50,0.50,0.75,0.50,
        0.25,1.00,0.50,1.00,0.50,1.00,0.50,0.75,
        0.50,0.75,0.50,0.50,0.50,0.50,0.25,0.50,
        0.25,0.50,0.00,0.50,0.00,0.50,0.00,0.75,
        0.00,0.75,0.00,1.00,0.00,1.00,0.25,1.00,
        0.00,0.00,0.00,0.00,1.00,1.00,1.00,1.00
    });
  ch = font->_char + '&';
  TGAFontInitChar(ch, 6,
    (float[]){
        1.00,0.00,1.00,0.33,0.76,0.67,0.50,0.67,
        0.50,0.67,0.00,0.66,0.00,1.00,0.50,1.00,
        0.50,1.00,1.00,1.00,1.00,0.67,0.50,0.67,
        0.50,0.67,0.33,0.67,0.00,0.50,0.00,0.33,
        0.00,0.33,0.00,0.00,0.33,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.17,1.00,0.50
    });
  ch = font->_char + '(';
  TGAFontInitChar(ch, 1,
    (float[]){
        1.00,1.00,0.75,0.75,0.75,0.25,1.00,0.00
    });
  ch = font->_char + ')';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.00,1.00,0.25,0.75,0.25,0.25,0.00,0.00
    });
  ch = font->_char + '=';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,0.33,0.00,0.33,1.00,0.33,1.00,0.33,
        0.00,0.67,0.00,0.67,1.00,0.67,1.00,0.67
    });
  ch = font->_char + '~';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.00,0.50,0.33,0.75,0.66,0.25,1.00,0.50
    });
  ch = font->_char + '`';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.75,1.00,0.75,1.00,0.75,0.49,1.00,0.50
    });
  ch = font->_char + '{';
  TGAFontInitChar(ch, 2,
    (float[]){
        1.00,1.00,0.75,1.00,1.00,0.50,0.75,0.50,
        0.75,0.50,1.00,0.50,0.76,0.00,1.00,0.00
    });
  ch = font->_char + '}';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,1.00,0.25,1.00,0.00,0.50,0.25,0.50,
        0.25,0.50,-0.02,0.50,0.25,0.00,0.00,0.00
    });
  ch = font->_char + '*';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,0.00,0.00,0.00,1.00,1.00,1.00,1.00,
        0.00,1.00,0.00,1.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + '+';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,0.50,0.00,0.50,1.00,0.50,1.00,0.50,
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00
    });
  ch = font->_char + '<';
  TGAFontInitChar(ch, 2,
    (float[]){
        1.00,1.00,1.00,1.00,0.00,0.50,0.00,0.50,
        0.00,0.50,0.00,0.50,1.00,0.00,1.00,0.00
    });
  ch = font->_char + '>';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,1.00,0.00,1.00,1.00,0.50,1.00,0.50,
        1.00,0.50,1.00,0.50,0.00,0.00,0.00,0.00
    });
  ch = font->_char + '?';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.00,0.67,0.00,1.00,0.34,1.00,0.50,1.00,
        0.50,1.00,0.66,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.33,0.50,0.66,0.50,0.33,
        0.50,0.18,0.44,0.18,0.44,0.07,0.50,0.07,
        0.50,0.07,0.56,0.07,0.56,0.18,0.50,0.18
    });
  ch = font->_char + '.';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.13,0.25,0.00,0.25,0.00,0.00,0.13,0.00,
        0.13,0.00,0.25,0.00,0.25,0.25,0.13,0.25
    });
  ch = font->_char + ',';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.25,0.18,0.25,0.18,0.25,-0.33,0.00,-0.32
    });
  ch = font->_char + '/';
  TGAFontInitChar(ch, 1,
    (float[]){
        1.00,1.00,1.00,1.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + '\\';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.00,1.00,0.00,1.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + '[';
  TGAFontInitChar(ch, 3,
    (float[]){
        1.00,1.00,1.00,1.00,0.75,1.00,0.75,1.00,
        0.75,1.00,0.75,1.00,0.75,0.00,0.75,0.00,
        0.75,0.00,0.75,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + ']';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,1.00,0.00,1.00,0.25,1.00,0.25,1.00,
        0.25,1.00,0.25,1.00,0.25,0.0,0.25,0.0,
        0.25,0.0,0.25,0.0,0.00,0.0,0.00,0.0
    });
  ch = font->_char + '-';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.00,0.50,0.00,0.50,1.00,0.50,1.00,0.50
    });
  ch = font->_char + '|';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00
    });
  ch = font->_char + '_';
  TGAFontInitChar(ch, 1,
    (float[]){
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00,
    });
  ch = font->_char + ';';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.25,0.47,0.18,0.47,0.18,0.36,0.25,0.36,
        0.25,0.36,0.30,0.36,0.30,0.47,0.25,0.47,
        0.25,0.18,0.25,0.18,0.25,-0.33,0.00,-0.32,
    });
  ch = font->_char + ':';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.50,0.72,0.44,0.72,0.44,0.61,0.50,0.61,
        0.50,0.61,0.56,0.61,0.56,0.72,0.50,0.72,
        0.50,0.39,0.44,0.39,0.44,0.28,0.50,0.28,
        0.50,0.28,0.56,0.28,0.56,0.39,0.50,0.39
    });
  ch = font->_char + 'a';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.66,0.67,0.25,0.67,0.00,0.66,0.00,0.33,
        0.00,0.33,0.00,0.00,0.26,0.01,0.49,0.01,
        0.49,0.01,0.74,0.01,0.75,0.33,0.75,0.67,
        0.75,0.67,0.75,0.25,0.75,0.01,1.00,0.00
    });
  ch = font->_char + 'b';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,1.00,0.00,0.50,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.33,1.00,0.50,
        1.00,0.50,1.00,0.67,0.59,0.67,0.42,0.67,
        0.42,0.67,0.25,0.67,0.06,0.58,0.06,0.33
    });
  ch = font->_char + 'c';
  TGAFontInitChar(ch, 4,
    (float[]){
        1.00,0.50,1.00,0.67,0.67,0.67,0.50,0.67,
        0.50,0.67,0.33,0.67,0.00,0.66,0.00,0.33,
        0.00,0.33,0.00,0.00,0.34,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.00,1.00,0.25
    });
  ch = font->_char + 'd';
  TGAFontInitChar(ch, 4,
    (float[]){
        1.00,1.00,1.01,0.50,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.33,0.00,0.50,
        0.00,0.50,0.00,0.67,0.44,0.66,0.59,0.66,
        0.59,0.66,0.75,0.66,0.95,0.59,0.95,0.34
    });
  ch = font->_char + 'e';
  TGAFontInitChar(ch, 6,
    (float[]){
        1.00,0.25,1.00,0.00,0.66,0.00,0.50,0.00,
        0.50,0.00,0.34,0.00,0.00,0.00,0.00,0.33,
        0.00,0.33,0.00,0.66,0.33,0.67,0.50,0.67,
        0.50,0.67,0.67,0.67,1.00,0.67,1.00,0.50,
        1.00,0.50,1.00,0.33,0.67,0.33,0.50,0.33,
        0.50,0.33,0.33,0.33,0.00,0.33,0.00,0.33
    });
  ch = font->_char + 'f';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.50,0.00,0.50,0.66,0.50,0.66,0.50,
        1.00,0.75,1.00,1.00,0.75,1.00,0.50,1.00,
        0.50,1.00,0.25,1.00,0.25,0.83,0.25,0.67,
        0.25,0.67,0.25,0.50,0.25,0.00,0.25,0.00
    });
  ch = font->_char + 'g';
  TGAFontInitChar(ch, 6,
    (float[]){
        1.00,0.33,1.00,0.00,0.67,0.00,0.50,0.00,
        0.50,0.00,0.33,0.00,0.00,-0.01,0.00,0.33,
        0.00,0.33,0.00,0.67,0.25,0.67,0.50,0.67,
        0.50,0.67,0.75,0.67,1.00,0.66,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,-0.33,0.50,-0.33,
        0.50,-0.33,0.41,-0.33,0.33,-0.33,0.33,-0.33
    });
  ch = font->_char + 'h';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.33,0.25,0.67,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.25,1.00,0.00,1.00,0.00,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'i';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.25,0.87,0.19,0.87,0.19,0.76,0.25,0.76,
        0.25,0.76,0.31,0.76,0.31,0.87,0.25,0.87,
        0.00,0.00,0.25,0.00,0.25,0.42,0.25,0.50,
        0.25,0.50,0.25,0.25,0.26,0.00,0.50,0.00,
        0.50,0.00,0.72,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'j';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.75,0.87,0.69,0.87,0.69,0.76,0.75,0.76,
        0.75,0.76,0.81,0.76,0.81,0.87,0.76,0.87,
        0.00,0.00,0.00,-0.33,0.33,-0.33,0.50,-0.33,
        0.50,-0.33,0.75,-0.33,0.75,0.33,0.75,0.50,
        0.75,0.50,0.75,0.33,0.76,0.00,1.00,0.00
    });
  ch = font->_char + 'k';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.50,0.25,0.67,1.00,0.75,1.00,0.50,
        1.00,0.50,1.00,0.25,0.50,0.33,0.00,0.33,
        0.00,0.33,0.32,0.33,0.75,0.25,1.00,0.00,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'l';
  TGAFontInitChar(ch, 6,
    (float[]){
        0.00,0.00,0.25,0.00,0.25,0.34,0.25,0.50,
        0.25,0.50,0.25,0.66,0.25,1.00,0.50,1.00,
        0.50,1.00,0.66,1.00,0.75,1.00,0.75,0.76,
        0.75,0.76,0.75,0.51,0.50,0.33,0.25,0.33,
        0.25,0.33,0.26,0.00,0.33,0.00,0.66,0.00,
        0.66,0.00,0.76,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'm';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.00,0.00,
        0.00,0.25,0.00,0.59,0.25,0.67,0.33,0.67,
        0.33,0.67,0.50,0.66,0.50,0.00,0.50,0.00,
        0.50,0.00,0.50,0.00,0.50,0.67,0.74,0.67,
        0.74,0.67,1.00,0.67,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'n';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.00,0.00,
        0.00,0.25,0.00,0.50,0.25,0.67,0.66,0.67,
        0.66,0.67,1.00,0.67,1.00,0.24,1.00,0.00
    });
  ch = font->_char + 'o';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.50,0.67,1.00,0.67,1.00,0.66,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,-0.01,0.00,0.33,
        0.00,0.33,0.00,0.67,0.00,0.67,0.50,0.67
    });
  ch = font->_char + 'p';
  TGAFontInitChar(ch, 5,
    (float[]){
        0.00,-0.33,0.00,-0.33,0.00,0.16,0.00,0.33,
        0.00,0.33,0.00,0.50,0.00,0.67,0.50,0.67,
        0.50,0.67,1.00,0.67,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.16,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.00
    });
  ch = font->_char + 'q';
  TGAFontInitChar(ch, 5,
    (float[]){
        1.00,0.00,1.00,0.00,0.75,0.00,0.50,0.00,
        0.50,0.00,0.25,0.00,0.00,-0.01,0.00,0.33,
        0.00,0.33,0.00,0.67,0.25,0.67,0.50,0.67,
        0.50,0.67,0.75,0.67,1.00,0.66,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,-0.33,1.00,-0.33
    });
  ch = font->_char + 'r';
  TGAFontInitChar(ch, 2, 
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.00,0.00,
        0.00,0.33,0.25,0.67,1.00,1.00,1.00,0.50
    });
  ch = font->_char + 's';
  TGAFontInitChar(ch, 5,
    (float[]){
        1.00,0.50,1.00,0.66,1.00,0.67,0.50,0.67,
        0.50,0.67,0.00,0.67,0.00,0.66,0.00,0.50,
        0.00,0.50,0.00,0.33,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.16,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.08,0.00,0.25
    });
  ch = font->_char + 't';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.00,0.25,0.00,0.25,0.17,0.25,0.25,
        0.00,0.67,0.00,0.67,0.50,0.67,0.50,0.67,
        0.25,1.00,0.25,1.00,0.25,0.33,0.25,0.25,
        0.25,0.25,0.25,0.01,0.50,0.00,1.00,0.00
    });
  ch = font->_char + 'u';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.67,0.00,0.33,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.33,1.00,0.67,
        1.00,0.67,1.00,0.33,1.00,0.00,1.00,0.00
    });
  ch = font->_char + 'v';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,0.67,0.00,0.67,0.34,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.67,1.00,0.67
    });
  ch = font->_char + 'w';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.67,0.00,0.67,0.16,0.00,0.33,0.00,
        0.33,0.00,0.50,0.00,0.50,0.50,0.50,0.50,
        0.50,0.50,0.50,0.50,0.50,0.00,0.66,0.00,
        0.66,0.00,0.82,0.00,1.00,0.67,1.00,0.67
    });
  ch = font->_char + 'x';
  TGAFontInitChar(ch, 4,
    (float[]){
        0.00,0.00,0.25,0.00,0.51,0.24,0.50,0.33,
        0.50,0.33,0.50,0.41,0.76,0.67,1.00,0.67,
        0.00,0.67,0.25,0.67,0.50,0.41,0.50,0.33,
        0.50,0.33,0.50,0.25,0.75,0.00,1.00,0.00
    });
  ch = font->_char + 'y';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.66,0.00,
        1.00,0.67,1.00,0.67,0.82,0.33,0.66,0.00,
        0.66,0.00,0.50,-0.33,0.50,-0.33,0.25,-0.33
    });
  ch = font->_char + 'z';
  TGAFontInitChar(ch, 3,
    (float[]){
        0.00,0.67,0.00,0.67,1.00,0.67,1.00,0.67,
        1.00,0.67,1.00,0.50,0.00,0.25,0.00,0.00,
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00
    });
  ch = font->_char + '@';
  TGAFontInitChar(ch, 8,
    (float[]){
        0.61,0.66,0.36,0.66,0.21,0.65,0.21,0.45,
        0.21,0.45,0.21,0.25,0.36,0.25,0.51,0.25,
        0.51,0.25,0.66,0.25,0.67,0.45,0.67,0.66,
        0.67,0.66,0.66,0.40,0.66,0.25,0.82,0.25,
        0.82,0.25,0.97,0.24,0.94,0.72,0.75,0.79,
        0.75,0.79,0.56,0.85,0.36,0.84,0.25,0.78,
        0.25,0.78,0.03,0.66,0.05,0.21,0.25,0.11,
        0.25,0.11,0.45,0.01,0.67,0.07,0.75,0.13
    });
  ch = font->_char + '^';
  TGAFontInitChar(ch, 2,
    (float[]){
        0.00,0.75,0.00,0.75,0.50,1.00,0.50,1.00,
        0.50,1.00,0.50,1.00,1.00,0.75,1.00,0.75
    });
}

