/* Dan Nelson
 * Graphics Package
 * image.c
 * Functions for image control
 * Functions for fractals
 */


#include "cb_graphics.h"


///////////////////////////////
///////// CONSTRUCTORS ////////
///////////////////////////////

/* Allocates an Image structure and initializes the top level fields to appropriate values
 * Returns a pointer to the allocated Image structure
 * Returns a NULL pointer if the operation fails
 */
Image *Image_create(int rows, int cols) {
  Image *newImage = malloc(sizeof(Image));
  if (newImage != NULL){
    Image_alloc(newImage, rows, cols);
    return newImage;
  }
  else {
    return NULL;
  }
}


/* Allocates space for an Image structure and the image data given rows and columns
   Returns 0 if the operation is successful
   Returns 1 if the operation fails.
 */
int Image_alloc(Image *src, int rows, int cols) {    
  int i;
  src->data = malloc(sizeof(FPixel)*rows*cols);
  if (src->data == NULL) {
    return 1;
  }
  else {
    src->rows = rows;
    src->cols = cols;
    for (i=0;i<rows*cols;i++){
      src->data[i].a = 0.0;
      src->data[i].z = 1.0;  
    }
    return 0;
  }
}


///////////////////////////////
///////// DESTRUCTORS /////////
///////////////////////////////

/*
 * De-allocates image data and frees the Image structure
 */
void Image_free(Image *src) {
  Image_clear(src);
  free(src);
}


/* De-allocates image data and resets the Image structure fields
 * The function does not free the Image structure
 */
void Image_clear(Image *src) {
  if (src->data != NULL) {
    free(src->data);
  }
  src->data = NULL;
  src->rows = 0;
  src->cols = 0;
}


///////////////////////////////
/////////     I/O    //////////
///////////////////////////////

// reads a PPM image from the given filename. An op- tional extension is to 
// determine the image type from the filename and permit the use of different 
// file types. Initializes the zbuffer to 1.0. Returns a NULL pointer if the operation fails.
Image *Image_readPPM(char *filename) {
  // old structure
  Pixel *image_P;
  int rows,cols;
  int colors;
  int i,j;
  
  image_P = readPPM(&rows, &cols, &colors, filename);
  if(!filename) {
    fprintf(stderr, "Unable to read %s\n", filename);
    exit(-1);
  }
  
  // new structure
  Image *image;
  image = Image_create(rows, cols);
  
  for(i=0;i<rows;i++) {
  	for(j=0;j<cols;j++) {
      Image_setc( image, i, j, 0, image_P[i*cols+j].r/255.0);
      Image_setc( image, i, j, 1, image_P[i*cols+j].g/255.0);
      Image_setc( image, i, j, 2, image_P[i*cols+j].b/255.0);
    }
  }
    
  free(image_P);
  
  return image;
}



// reads a PPM image from the given filename to the given pointer. An op- tional extension is to 
// determine the image type from the filename and permit the use of different 
// file types. Initializes the zbuffer to 1.0. Returns a NULL pointer if the operation fails.
void Image_readPPM2(char *filename, Image *image) {
  // old structure
  Pixel *image_P;
  int rows,cols;
  int colors;
  int i,j;
  
  image_P = readPPM(&rows, &cols, &colors, filename);
  if(!filename) {
    fprintf(stderr, "Unable to read %s\n", filename);
    exit(-1);
  }
  
  image = Image_create(rows, cols);
  
  for(i=0;i<rows;i++) {
  	for(j=0;j<cols;j++) {
      Image_setc( image, i, j, 0, image_P[i*cols+j].r/255.0);
      Image_setc( image, i, j, 1, image_P[i*cols+j].g/255.0);
      Image_setc( image, i, j, 2, image_P[i*cols+j].b/255.0);
    }
  }
  
  free(image_P);  
}


/* Write image data from the Image struct to a ppm file
 */
void Image_writePPM(Image *src, char* filename) {
  Pixel *temp;
  temp = malloc(sizeof(Pixel) * (src->rows*src->cols));
  float r,g,b;
  int colors = 255;
  int i;
  for(i=0;i<src->rows*src->cols;i++) {
    r = Image_get1D(src,i).rgb[0];
    r = r > 1 ? 1 : r;
    temp[i].r = (unsigned char)(255.0*r);
    g = Image_get1D(src,i).rgb[1];
    g = g > 1 ? 1 : g;    
    temp[i].g = (unsigned char)(255.0*g);
    b = Image_get1D(src,i).rgb[2];
    b = b > 1 ? 1 : b;
    temp[i].b = (unsigned char)(255.0*b);
  }
  
  writePPM(temp, src->rows, src->cols, colors, filename);
  free(temp);
}



///////////////////////////////
/////////// ACCESS ////////////
///////////////////////////////
/* Returns an FPixel at location (r,c) in an image
 */
FPixel Image_getf(Image *src, int r, int c) {
  return src->data[r*src->cols+c];
}


/* Returns the color value of channel b at location (r,c) in an image
 */
float Image_getc(Image *src, int r, int c, int b) {
  return src->data[r*src->cols+c].rgb[b];
}


/* Returns the alpha value at location (r,c) in an image
 */
float Image_geta(Image *src, int r, int c) {
  return src->data[r*src->cols+c].a;
}


/* Returns the z value at location (r,c) in an image
 */
float Image_getz(Image *src, int r, int c) {
  if (r>=0 && r<src->rows && c>=0 && c<src->cols) {
    return src->data[r*src->cols+c].z;
  }
  else return 0;
}

/* Returns the pixel at location i in data array
 */
FPixel Image_get1D(Image *src, int i) {
  return src->data[i];
}


/* Sets the pixel at (r,c) in an image to Fpixel p
 */
void Image_setf(Image *src, int r, int c, FPixel p) {
  src->data[r*src->cols+c] = p;
}


/* Sets the color value of channel b at pixel (r,c) in image
 */
void Image_setc(Image *src, int r, int c, int b, float val) {
  src->data[r*src->cols+c].rgb[b] = val;
}


/* Sets the alpha value of pixel at (r,c) in image
 */
void Image_seta(Image *src, int r, int c, float val) {
  src->data[r*src->cols+c].a = val;
}


/* Sets the z value of pixel at (r,c) in image
 */
void Image_setz(Image *src, int r, int c, float val) {
  if (r>=0 && r<src->rows && c>=0 && c<src->cols) {
    src->data[r*src->cols+c].z = val;
  }
}


/* Sets the pixel at location i in data array to Fpixel p
 */
void Image_set1D(Image *src, FPixel p, int i) {
  src->data[i] = p;
}


///////////////////////////////
/////////// COLOR /////////////
///////////////////////////////

/* Copies the color data
 */
void Color_copy(Color *to, Color *from) {
  to->c[0] = from->c[0];
  to->c[1] = from->c[1];
  to->c[2] = from->c[2];
}




/* Returns a Color structure built from the pixel values
 */
Color Image_getColor(Image *src, int r, int c ) {
  Color p = {{0.0,0.0,0.0}};
  
  if (r>=0 && r<src->rows && c>=0 && c<src->cols) {
    p.c[0] = src->data[r*src->cols+c].rgb[0];
    p.c[1] = src->data[r*src->cols+c].rgb[1];
    p.c[2] = src->data[r*src->cols+c].rgb[2];
  }
  else {
   // printf("outside of image \n");
  }
  
  return p;
}

/* Sum up color a and b and store it in c
 */
void Color_sum(Color *a, Color *b, Color *c) {
  c->c[0] = a->c[0] + b->c[0];
  c->c[1] = a->c[1] + b->c[1];
  c->c[2] = a->c[2] + b->c[2];
}


/* Copies the Color data to the proper pixel
 */
void Image_setColor( Image *src, int r, int c, Color val ) {
  /*val.c[0] = val.c[0] > 1 ? 1 : val.c[0];
  val.c[1] = val.c[0] > 1 ? 1 : val.c[1];
  val.c[2] = val.c[0] > 1 ? 1 : val.c[2]; */

  if (r>=0 && r<src->rows && c>=0 && c<src->cols) {
    src->data[r*src->cols+c].rgb[0] = val.c[0];
    src->data[r*src->cols+c].rgb[1] = val.c[1];
    src->data[r*src->cols+c].rgb[2] = val.c[2];
  }
}


/* Set color With Brightness
 * b: brightness factor. 0<b<1
 */
void Image_setColorWB( Image *src, int r, int c, Color val, double b) {
  src->data[r*src->cols+c].rgb[0] = b*val.c[0];
  src->data[r*src->cols+c].rgb[1] = b*val.c[1];
  src->data[r*src->cols+c].rgb[2] = b*val.c[2];
}



void Image_reset(Image *src) {
  Color White = {{0.0,0.0,0.0}};
  int i,j;
  
  for (i=0;i<src->rows;i++)
    for (j=0;j<src->cols;j++) {
      Image_setColor(src,i,j,White);
      Image_setz(src,i,j,1.0);
    }

}


void Color_set(Color *color, float c0, float c1, float c2) {
 color->c[0] = c0;
 color->c[1] = c1;
 color->c[2] = c2;
}

void Color_print(Color *color, FILE *fp) {
  fprintf(fp, "Color: { %lf %lf %lf } \n", color->c[0], color->c[1], color->c[2]);
}
