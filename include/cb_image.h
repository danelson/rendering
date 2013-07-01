// Dan Nelson and Hieu Phan
// Project 4
// September 23, 2011
// cb_image.h

#ifndef CB_IMAGE_H

#define CB_IMAGE_H

#include "stdio.h";

/*****************************************
 *				STRUCTURES				 * 
 *****************************************/
// structure that holds pixel information
typedef struct {
  float rgb[3];
  float a;
  float z;
} FPixel;

// structure that holds image information
typedef struct { 
  long rows; 
  long cols; 
  FPixel *data;
} Image;

// Color 
typedef struct {
  float c[3];
} Color;
 

/*****************************************
 *			 IMAGE FUNCTIONS		     * 
 *****************************************/
// CONSTRUCTORS
Image *Image_create(int rows, int cols);
int Image_alloc(Image *src, int rows, int cols);

// DESTRUCTORS
void Image_clear(Image *src);
void Image_free(Image *src);

// I/O
Image *Image_readPPM(char *filename);
void Image_readPPM2(char *filename, Image *image);
void Image_writePPM(Image *src, char* filename);

// ACCESS
FPixel Image_getf(Image *src, int r, int c);
float Image_getc(Image *src, int r, int c, int b);
float Image_geta(Image *src, int r, int c);
float Image_getz(Image *src, int r, int c);
FPixel Image_get1D(Image *src, int i);
void Image_reset(Image *src);

// ACCESS
void Image_setf(Image *src, int r, int c, FPixel p);
void Image_setc(Image *src, int r, int c, int b, float val);
void Image_seta(Image *src, int r, int c, float val);
void Image_setz(Image *src, int r, int c, float val);
void Image_set1D(Image *src, FPixel p, int i);


/*****************************************
 *			 COLOR FUNCTIONS		     * 
 *****************************************/
void Color_set(Color *color, float c0, float c1, float c2);
void Color_copy(Color *to, Color *from);
Color Image_getColor( Image *src, int r, int c );
void Image_setColor( Image *src, int r, int c, Color val );
void Image_setColorWB( Image *src, int r, int c, Color val, double b);
void Color_print(Color *color, FILE *fp);
void Color_sum(Color *a, Color *b, Color *c);


#endif
