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
// Return NULL if it couldn't create
TGAFont* TGAFontCreate(tgaFont font) {
  // Allocate memory
  TGAFont *ret = (TGAFont*)malloc(sizeof(TGAFont));
  // If we could allocate memory
  if (ret != NULL) {
    // Set the default size
    ret->_size = 18.0;
    // Set the default space
    ret->_space[0] = ret->_space[1] = 3.0;
    // Set the default scale
    ret->_scale[0] = 0.5; ret->_scale[1] = 1.0;
    // Set the default anchor
    ret->_anchor = tgaFontAnchorTopLeft;
    // For each character
    for (int iChar = 256; iChar--;)
      // By default set this character definition as empty (no curves)
      ret->_char[iChar]._nbCurve = 0;
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

// Set the font scale of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetScale(TGAFont *font, float *v) {
  // If the argument are invalid, stop here
  if (font == NULL || v == NULL)
    return;
  // Set the scale
  font->_scale[0] = v[0];  
  font->_scale[1] = v[1];  
}

// Set the font spacing of TGAFont 'font' to 'v'
// Do nothing if arguments are invalid
void TGAFontSetSpace(TGAFont *font, float *v) {
  // If the argument are invalid, stop here
  if (font == NULL || v == NULL)
    return;
  // Set the space
  font->_space[0] = v[0];  
  font->_space[1] = v[1];  
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

// Get the next position form 'p' incremented by one tabulation
// of 'font'
float TGAFontGetNextPosByTab(TGAFont *font, float p) {
  return (floor(p / font->_tabSize) + 1.0) * font->_tabSize;
}

// Get the dimension in pixels of the block of text representing 
// string 's' printed with 'font'
// Return the dimension in float[2] 'dim', return {-1, -1} if arguments
// are invalid
void TGAFontGetStringSize(TGAFont *font, unsigned char *s, float *dim) {
  // Check arguments
  if (font == NULL || dim == NULL) {
    dim[0] = dim[1] = -1.0;
    return;
  }
  // If the string is empty
  if (s == NULL) {
    // Dimensions are null
    dim[0] = dim[1] = 0.0;
  // Else, the string is not empty
  } else {
    // Initialise the dimension;
    dim[0] = 0.0;
    dim[1] = font->_size * font->_scale[1];
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
        dim[1] += font->_size * font->_scale[1] + font->_space[1];
        // Reset the length of line
        l = 0.0;
        // Reset the flag 
        flagStart = true;
      // Else, if this character is a tabulation
      } else if (s[iChar] == '\t') {
        // Increment length to the next tab
        l = TGAFontGetNextPosByTab(font, l);
        // If the current line is longer than the longest one
        if (dim[0] < l)
          // Update the length of the
          dim[0] = l;
      // Else, for others character
      } else {
        // If it's not the first char
        if (flagStart == false)
          // Add the space between character
          l += font->_space[0];
        // Update the flag of beginning of line
        flagStart = false;
        // Increment the length of the current line
        l += font->_size * font->_scale[0];
        // If the current line is longer than the longest one
        if (dim[0] < l)
          // Update the length
          dim[0] = l;
      }
    }
  }
}

// Create the curves of each characters for the default font
void TGAFontCreateDefault(TGAFont *font) {
  TGAChar *ch = NULL;
  ch = font->_char + 'A';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.0,0.0,0.0,0.18,0.32,1.0,0.5,1.0,
        0.5,1.0,0.68,1.0,1.0,0.18,1.0,0.0,
        0.15,0.5,0.15,0.5,0.85,0.5,0.85,0.5
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'B';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.77,1.00,0.77,0.58,0.00,0.59,
        0.00,0.59,0.50,0.60,1.01,0.50,1.00,0.26,
        1.00,0.26,1.00,0.00,0.50,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'C';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.67,1.00,0.82,1.00,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.81,0.00,0.50,
        0.00,0.50,0.00,0.18,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.17,1.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'D';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00,
        0.00,0.00,1.00,0.00,1.00,0.00,1.00,0.50,
        1.00,0.50,1.00,1.00,0.50,1.00,0.00,1.00,
        0.00,1.00,-0.11,1.00,0.00,0.00,0.00,0.00,
        0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'E';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.12,1.01,0.06,0.95,
        0.06,0.95,-0.01,0.90,0.00,0.10,0.05,0.05,
        0.05,0.05,0.11,-0.01,1.00,0.00,1.00,0.00,
        1.00,0.00,1.00,0.00,0.00,0.00,0.00,0.00,
        0.00,0.50,0.00,0.50,0.50,0.50,0.50,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'F';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.50,0.00,0.50,0.50,0.50,0.50,0.50,
        1.00,1.00,1.00,1.00,0.12,1.01,0.06,0.95,
        0.06,0.95,-0.01,0.90,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'G';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.84,1.00,1.00,0.74,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.81,0.00,0.50,
        0.00,0.50,0.00,0.18,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.50,1.00,0.50,
        1.00,0.50,1.00,0.50,0.50,0.50,0.50,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'H';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,1.00,0.00,1.00,0.00,
        0.00,0.50,0.00,0.50,1.00,0.50,1.00,0.50,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'I';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00,
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00,
        0.10,1.00,0.10,1.00,0.90,1.00,0.90,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'J';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.66,1.00,0.66,1.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.33,0.00,0.50,
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'K';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.50,0.54,0.50,0.00,1.00,0.00,1.00,0.00,
        0.00,0.50,0.00,0.50,0.00,0.50,0.33,0.50,
        0.33,0.50,0.67,0.51,1.00,1.00,1.00,1.00,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'L';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,0.00,0.12,0.05,0.05,
        0.05,0.05,0.08,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'M';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.00,1.00,0.34,0.67,0.50,0.67,
        0.50,0.67,0.66,0.67,1.00,1.00,1.00,1.00,
        1.00,1.00,1.00,1.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'N';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.33,1.00,0.66,0.00,1.00,0.00,
        1.00,0.00,1.00,0.00,1.00,1.00,1.00,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'O';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.50,1.00,1.00,1.00,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.50,
        0.00,0.50,0.00,1.00,0.00,1.00,0.50,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'P';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.50,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.33,0.50,0.33,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'Q';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.66,0.33,0.66,0.33,1.00,0.00,1.00,0.00,
        0.50,1.00,1.00,1.00,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.50,
        0.00,0.50,0.00,1.00,0.00,1.00,0.50,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'R';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.33,0.33,0.00,1.00,0.00,1.00,0.00,
        0.00,0.00,0.00,0.00,0.00,1.00,0.00,1.00,
        0.00,1.00,0.50,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.33,0.50,0.33,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'S';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.83,1.00,0.99,1.00,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.83,0.00,0.67,
        0.00,0.67,0.00,0.50,1.00,0.67,1.00,0.50,
        1.00,0.50,1.00,0.33,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.16,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'T';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00,
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'U';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,0.50,0.01,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.51,1.00,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'V';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,0.34,0.00,0.50,0.00,
        0.50,0.00,0.67,0.00,1.00,1.00,1.00,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'W';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,0.16,0.00,0.33,0.00,
        0.33,0.00,0.50,0.00,0.50,0.50,0.50,0.50,
        0.50,0.50,0.50,0.50,0.50,0.00,0.66,0.00,
        0.66,0.00,0.82,0.00,1.00,1.00,1.00,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'X';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.50,0.67,0.50,0.51,
        0.50,0.51,0.50,0.33,0.00,0.00,0.00,0.00,
        0.00,1.00,0.00,1.00,0.50,0.67,0.50,0.50,
        0.50,0.50,0.50,0.33,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'Y';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.50,0.67,0.50,0.50,
        0.00,1.00,0.00,1.00,0.50,0.67,0.50,0.50,
        0.50,0.50,0.50,0.33,0.50,0.00,0.50,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'Z';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00,
        1.00,1.00,1.00,0.67,0.00,0.33,0.00,0.00,
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '0';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,1.00,1.00,1.00,1.00,
        0.50,1.00,1.00,1.00,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.50,
        0.00,0.50,0.00,1.00,0.00,1.00,0.50,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '1';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00,
        0.00,0.67,0.33,0.67,0.50,1.00,0.50,1.00,
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '2';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,1.00,0.34,1.00,0.50,1.00,
        0.50,1.00,0.66,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.50,0.00,0.33,0.00,0.00,
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '3';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.83,0.00,1.00,0.50,1.00,
        0.50,1.00,1.00,1.00,1.00,0.83,1.00,0.67,
        1.00,0.67,1.00,0.50,0.50,0.50,0.50,0.50,
        0.50,0.50,0.50,0.50,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.16,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '4';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.33,1.00,0.33,0.00,0.33,0.00,0.33,
        0.00,0.33,0.50,0.50,0.66,1.00,0.66,1.00,
        0.66,1.00,0.66,1.00,0.66,0.00,0.66,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '5';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.33,1.00,0.33,1.00,
        0.33,1.00,0.33,1.00,0.00,0.67,0.00,0.67,
        0.00,0.67,0.00,0.67,1.00,1.01,1.00,0.33,
        1.00,0.33,1.00,0.00,0.67,0.00,0.50,0.00,
        0.50,0.00,0.33,0.00,0.00,0.16,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '6';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.33,0.00,0.50,0.33,0.50,0.50,0.50,
        0.50,0.50,0.67,0.50,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.16,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.33,0.00,0.50,
        0.00,0.50,0.00,1.00,0.50,1.00,0.50,1.00,
        0.50,1.00,0.50,1.00,1.00,1.00,1.00,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '7';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,1.00,1.00,1.00,1.00,
        1.00,1.00,1.00,1.00,0.33,0.67,0.33,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '8';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        0.50,1.00,1.00,1.00,1.00,0.67,0.50,0.67,
        0.50,0.67,0.33,0.67,0.00,0.50,0.00,0.33,
        0.00,0.33,0.00,0.00,0.33,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.00,1.00,0.33,
        1.00,0.33,1.00,0.50,0.66,0.67,0.50,0.67,
        0.50,0.67,0.00,0.67,0.00,1.00,0.50,1.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '9';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.33,0.00,0.50,0.00,1.00,0.00,1.00,0.50,
        1.00,0.50,1.00,1.00,0.66,1.00,0.50,1.00,
        0.50,1.00,0.33,1.00,0.00,1.00,0.00,0.67,
        0.00,0.67,0.00,0.50,0.33,0.50,0.50,0.50,
        0.50,0.50,0.67,0.50,1.00,0.50,1.00,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '!';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.50,0.18,0.44,0.18,0.44,0.07,0.50,0.07,
        0.50,0.07,0.56,0.07,0.56,0.18,0.50,0.18,
        0.50,1.00,0.50,1.00,0.50,0.33,0.50,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '"';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.66,1.00,0.66,1.00,0.66,0.75,0.66,0.75,
        0.33,1.00,0.33,1.00,0.33,0.75,0.33,0.75
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '\'';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.25,1.00,0.25,1.00,0.25,0.49,0.00,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '#';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.75,1.00,0.75,1.00,0.66,0.00,0.66,0.00,
        0.33,1.00,0.33,1.00,0.25,0.00,0.25,0.00,
        0.00,0.25,0.00,0.25,1.00,0.25,1.00,0.25,
        0.00,0.67,0.00,0.67,1.00,0.67,1.00,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '$';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00,
        1.00,0.83,1.00,0.99,1.00,1.00,0.50,1.00,
        0.50,1.00,0.00,1.00,0.00,0.83,0.00,0.67,
        0.00,0.67,0.00,0.50,1.00,0.67,1.00,0.50,
        1.00,0.50,1.00,0.33,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.16,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '%';
  ch->_nbCurve = 9;
  memcpy(ch->_curve, 
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
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '&';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.00,1.00,0.33,0.76,0.67,0.50,0.67,
        0.50,0.67,0.00,0.66,0.00,1.00,0.50,1.00,
        0.50,1.00,1.00,1.00,1.00,0.67,0.50,0.67,
        0.50,0.67,0.33,0.67,0.00,0.50,0.00,0.33,
        0.00,0.33,0.00,0.00,0.33,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.17,1.00,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '(';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,0.75,0.75,0.75,0.25,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + ')';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.25,0.75,0.25,0.25,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '=';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.33,0.00,0.33,1.00,0.33,1.00,0.33,
        0.00,0.67,0.00,0.67,1.00,0.67,1.00,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '~';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.50,0.33,0.75,0.66,0.25,1.00,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '`';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.75,1.00,0.75,1.00,0.75,0.49,1.00,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '{';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,0.75,1.00,1.00,0.50,0.75,0.50,
        0.75,0.50,1.00,0.50,0.76,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '}';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.25,1.00,0.00,0.50,0.25,0.50,
        0.25,0.50,-0.02,0.50,0.25,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '*';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,1.00,1.00,1.00,1.00,
        0.00,1.00,0.00,1.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '+';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.50,0.00,0.50,1.00,0.50,1.00,0.50,
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '<';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.00,0.50,0.00,0.50,
        0.00,0.50,0.00,0.50,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '>';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,1.00,0.50,1.00,0.50,
        1.00,0.50,1.00,0.50,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '?';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,1.00,0.34,1.00,0.50,1.00,
        0.50,1.00,0.66,1.00,1.00,1.00,1.00,0.67,
        1.00,0.67,1.00,0.33,0.50,0.66,0.50,0.33,
        0.50,0.18,0.44,0.18,0.44,0.07,0.50,0.07,
        0.50,0.07,0.56,0.07,0.56,0.18,0.50,0.18
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '.';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.13,0.25,0.00,0.25,0.00,0.00,0.13,0.00,
        0.13,0.00,0.25,0.00,0.25,0.25,0.13,0.25
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + ',';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.25,0.18,0.25,0.18,0.25,-0.33,0.00,-0.32
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '/';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '\\';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '[';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.00,1.00,0.75,1.00,0.75,1.00,
        0.75,1.00,0.75,1.00,0.75,0.00,0.75,0.00,
        0.75,0.00,0.75,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + ']';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,1.00,0.25,1.00,0.25,1.00,
        0.25,1.00,0.25,1.00,0.25,0.0,0.25,0.0,
        0.25,0.0,0.25,0.0,0.00,0.0,0.00,0.0
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '-';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.50,0.00,0.50,1.00,0.50,1.00,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '|';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.50,1.00,0.50,1.00,0.50,0.00,0.50,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '_';
  ch->_nbCurve = 1;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00,
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + ';';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.25,0.47,0.18,0.47,0.18,0.36,0.25,0.36,
        0.25,0.36,0.30,0.36,0.30,0.47,0.25,0.47,
        0.25,0.18,0.25,0.18,0.25,-0.33,0.00,-0.32,
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + ':';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.50,0.72,0.44,0.72,0.44,0.61,0.50,0.61,
        0.50,0.61,0.56,0.61,0.56,0.72,0.50,0.72,
        0.50,0.39,0.44,0.39,0.44,0.28,0.50,0.28,
        0.50,0.28,0.56,0.28,0.56,0.39,0.50,0.39
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'a';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.66,0.67,0.25,0.67,0.00,0.66,0.00,0.33,
        0.00,0.33,0.00,0.00,0.26,0.01,0.49,0.01,
        0.49,0.01,0.74,0.01,0.75,0.33,0.75,0.67,
        0.75,0.67,0.75,0.25,0.75,0.01,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'b';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,1.00,0.00,0.50,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.33,1.00,0.50,
        1.00,0.50,1.00,0.67,0.59,0.67,0.42,0.67,
        0.42,0.67,0.25,0.67,0.06,0.58,0.06,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'c';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.50,1.00,0.67,0.67,0.67,0.50,0.67,
        0.50,0.67,0.33,0.67,0.00,0.66,0.00,0.33,
        0.00,0.33,0.00,0.00,0.34,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.00,1.00,0.25
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'd';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        1.00,1.00,1.01,0.50,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.33,0.00,0.50,
        0.00,0.50,0.00,0.67,0.44,0.66,0.59,0.66,
        0.59,0.66,0.75,0.66,0.95,0.59,0.95,0.34
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'e';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.25,1.00,0.00,0.66,0.00,0.50,0.00,
        0.50,0.00,0.34,0.00,0.00,0.00,0.00,0.33,
        0.00,0.33,0.00,0.66,0.33,0.67,0.50,0.67,
        0.50,0.67,0.67,0.67,1.00,0.67,1.00,0.50,
        1.00,0.50,1.00,0.33,0.67,0.33,0.50,0.33,
        0.50,0.33,0.33,0.33,0.00,0.33,0.00,0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'f';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.50,0.00,0.50,0.66,0.50,0.66,0.50,
        1.00,0.75,1.00,1.00,0.75,1.00,0.50,1.00,
        0.50,1.00,0.25,1.00,0.25,0.83,0.25,0.67,
        0.25,0.67,0.25,0.50,0.25,0.00,0.25,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'g';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.33,1.00,0.00,0.67,0.00,0.50,0.00,
        0.50,0.00,0.33,0.00,0.00,-0.01,0.00,0.33,
        0.00,0.33,0.00,0.67,0.25,0.67,0.50,0.67,
        0.50,0.67,0.75,0.67,1.00,0.66,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,-0.33,0.50,-0.33,
        0.50,-0.33,0.41,-0.33,0.33,-0.33,0.33,-0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'h';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.33,0.25,0.67,1.00,1.00,1.00,0.50,
        1.00,0.50,1.00,0.25,1.00,0.00,1.00,0.00,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'i';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.25,0.87,0.19,0.87,0.19,0.76,0.25,0.76,
        0.25,0.76,0.31,0.76,0.31,0.87,0.25,0.87,
        0.00,0.00,0.25,0.00,0.25,0.42,0.25,0.50,
        0.25,0.50,0.25,0.25,0.26,0.00,0.50,0.00,
        0.50,0.00,0.72,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'j';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.75,0.87,0.69,0.87,0.69,0.76,0.75,0.76,
        0.75,0.76,0.81,0.76,0.81,0.87,0.76,0.87,
        0.00,0.00,0.00,-0.33,0.33,-0.33,0.50,-0.33,
        0.50,-0.33,0.75,-0.33,0.75,0.33,0.75,0.50,
        0.75,0.50,0.75,0.33,0.76,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'k';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.50,0.25,0.67,1.00,0.75,1.00,0.50,
        1.00,0.50,1.00,0.25,0.50,0.33,0.00,0.33,
        0.00,0.33,0.32,0.33,0.75,0.25,1.00,0.00,
        0.00,1.00,0.00,1.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'l';
  ch->_nbCurve = 6;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.25,0.00,0.25,0.34,0.25,0.50,
        0.25,0.50,0.25,0.66,0.25,1.00,0.50,1.00,
        0.50,1.00,0.66,1.00,0.75,1.00,0.75,0.76,
        0.75,0.76,0.75,0.51,0.50,0.33,0.25,0.33,
        0.25,0.33,0.26,0.00,0.33,0.00,0.66,0.00,
        0.66,0.00,0.76,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'm';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.00,0.00,
        0.00,0.25,0.00,0.59,0.25,0.67,0.33,0.67,
        0.33,0.67,0.50,0.66,0.50,0.00,0.50,0.00,
        0.50,0.00,0.50,0.00,0.50,0.67,0.74,0.67,
        0.74,0.67,1.00,0.67,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'n';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.00,0.00,
        0.00,0.25,0.00,0.50,0.25,0.67,0.66,0.67,
        0.66,0.67,1.00,0.67,1.00,0.24,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'o';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.50,0.67,1.00,0.67,1.00,0.66,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,-0.01,0.00,0.33,
        0.00,0.33,0.00,0.67,0.00,0.67,0.50,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'p';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        0.00,-0.33,0.00,-0.33,0.00,0.16,0.00,0.33,
        0.00,0.33,0.00,0.50,0.00,0.67,0.50,0.67,
        0.50,0.67,1.00,0.67,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.16,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.00,0.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'q';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.00,1.00,0.00,0.75,0.00,0.50,0.00,
        0.50,0.00,0.25,0.00,0.00,-0.01,0.00,0.33,
        0.00,0.33,0.00,0.67,0.25,0.67,0.50,0.67,
        0.50,0.67,0.75,0.67,1.00,0.66,1.00,0.33,
        1.00,0.33,1.00,0.00,1.00,-0.33,1.00,-0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'r';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.00,0.00,
        0.00,0.33,0.25,0.67,1.00,1.00,1.00,0.50
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 's';
  ch->_nbCurve = 5;
  memcpy(ch->_curve, 
    (float[]){
        1.00,0.50,1.00,0.66,1.00,0.67,0.50,0.67,
        0.50,0.67,0.00,0.67,0.00,0.66,0.00,0.50,
        0.00,0.50,0.00,0.33,1.00,0.50,1.00,0.33,
        1.00,0.33,1.00,0.16,1.00,0.00,0.50,0.00,
        0.50,0.00,0.00,0.00,0.00,0.08,0.00,0.25
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 't';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.25,0.00,0.25,0.17,0.25,0.25,
        0.00,0.67,0.00,0.67,0.50,0.67,0.50,0.67,
        0.25,1.00,0.25,1.00,0.25,0.33,0.25,0.25,
        0.25,0.25,0.25,0.01,0.50,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'u';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.33,0.00,0.00,0.50,0.00,
        0.50,0.00,1.00,0.00,1.00,0.33,1.00,0.67,
        1.00,0.67,1.00,0.33,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'v';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,0.34,0.00,0.50,0.00,
        0.50,0.00,0.66,0.00,1.00,0.67,1.00,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'w';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,0.16,0.00,0.33,0.00,
        0.33,0.00,0.50,0.00,0.50,0.50,0.50,0.50,
        0.50,0.50,0.50,0.50,0.50,0.00,0.66,0.00,
        0.66,0.00,0.82,0.00,1.00,0.67,1.00,0.67
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'x';
  ch->_nbCurve = 4;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.00,0.25,0.00,0.51,0.24,0.50,0.33,
        0.50,0.33,0.50,0.41,0.76,0.67,1.00,0.67,
        0.00,0.67,0.25,0.67,0.50,0.41,0.50,0.33,
        0.50,0.33,0.50,0.25,0.75,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'y';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,0.00,0.00,0.66,0.00,
        1.00,0.67,1.00,0.67,0.82,0.33,0.66,0.00,
        0.66,0.00,0.50,-0.33,0.50,-0.33,0.25,-0.33
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + 'z';
  ch->_nbCurve = 3;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.67,0.00,0.67,1.00,0.67,1.00,0.67,
        1.00,0.67,1.00,0.50,0.00,0.25,0.00,0.00,
        0.00,0.00,0.00,0.00,1.00,0.00,1.00,0.00
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '@';
  ch->_nbCurve = 8;
  memcpy(ch->_curve, 
    (float[]){
        0.61,0.66,0.36,0.66,0.21,0.65,0.21,0.45,
        0.21,0.45,0.21,0.25,0.36,0.25,0.51,0.25,
        0.51,0.25,0.66,0.25,0.67,0.45,0.67,0.66,
        0.67,0.66,0.66,0.40,0.66,0.25,0.82,0.25,
        0.82,0.25,0.97,0.24,0.94,0.72,0.75,0.79,
        0.75,0.79,0.56,0.85,0.36,0.84,0.25,0.78,
        0.25,0.78,0.03,0.66,0.05,0.21,0.25,0.11,
        0.25,0.11,0.45,0.01,0.67,0.07,0.75,0.13
    }, sizeof(float) * ch->_nbCurve * 8);
  ch = font->_char + '^';
  ch->_nbCurve = 2;
  memcpy(ch->_curve, 
    (float[]){
        0.00,0.75,0.00,0.75,0.50,1.00,0.50,1.00,
        0.50,1.00,0.50,1.00,1.00,0.75,1.00,0.75
    }, sizeof(float) * ch->_nbCurve * 8);
}

