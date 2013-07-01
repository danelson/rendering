/* Dan Nelson
 * Graphics Package
 * drawstate.c
 */


#include <string.h>
#include "cb_graphics.h"


// create a new DrawState structure and initialize the fields
DrawState *DrawState_create() {
  
  DrawState *ds = malloc(sizeof(DrawState));
  
  Color_set(&(ds->color), 1.0, 1.0, 1.0);
  Color_set(&(ds->body), 1.0, 1.0, 1.0);
  Color_set(&(ds->surface), 1.0, 1.0, 1.0);
  ds->surfaceCoeff = 0.0;
  ds->shade = ShadeGouraud;
  ds->zBufferFlag = 1;
  Point_set(&ds->viewer, 0.0, 0.0, 0.0);
  
  return ds;
}

// set the color field to c
void DrawState_setColor( DrawState *s, Color c ) {
  s->color = c;
}

// set the body field to c
void DrawState_setBody( DrawState *s, Color c ) {
  s->body = c;
}

// set the surface field to c
void DrawState_setSurface( DrawState *s, Color c ) {
  s->surface = c;
}

// set the surfaceCoeff field to f
void DrawState_setSurfaceCoeff( DrawState *s, float f ) {
  s->surfaceCoeff = f;
}

// copy the DrawState data
void DrawState_copy(DrawState *to, DrawState *from ) {
  Color_copy(&(to->color), &(from->color));
  Color_copy(&(to->body), &(from->body));
  Color_copy(&(to->surface), &( from->surface));
  Color_copy(&(to->flatColor),&(from->flatColor));
  to->shade = from->shade;
  Point_copy(&(to->viewer), &(from->viewer));
  to->surfaceCoeff = from->surfaceCoeff;
  to->zBufferFlag = from->zBufferFlag;
  to->tex = from->tex;
}

// read in the texture image and texture type
Texture *Texture_create(char *filename, TextureType textureType) {
  Texture *texture = malloc(sizeof(Texture));
  texture->img = Image_readPPM(filename);
  texture->textureType = textureType;  
  return texture;
}


// returns the color/value of a texture map given a bounding box.
Color Texture_value(Texture *tex, Point corner[4]) {
  Color color;
  Color temp;

  int x0,x1,y0,y1;
  float r,g,b;
  int i,j;
  
  
  x0 = (int) (corner[0].val[0]);
  x1 = (int) (corner[1].val[0]);
  y0 = (int) (corner[0].val[1]);
  y1 = (int) (corner[1].val[1]);
  
  for (i=0;i<4;i++) {
    x0 = (x0 > corner[i].val[0]) ? (int) (corner[i].val[0]) : x0;
    y0 = (y0 > corner[i].val[1]) ? (int) (corner[i].val[1]) : y0;
    x1 = (x1 < corner[i].val[0]) ? (int) (corner[i].val[0]) : x1;
    y1 = (y1 < corner[i].val[1]) ? (int) (corner[i].val[1]) : y1;    
  }
  r = g = b = 0.0;

  //printf("BOUNDING BOX %d %d %d %d \n",x0,y0,x1,y1);
  // for all the pixels in the bounding box get the total 
  // of each color
  for (i=x0;i<x1;i++) {
    for (j=y0;j<y1;j++) {
	  //printf("image size: %lu %lu \n", tex->img->rows, tex->img->cols);      	
      // get the color at this pixel and increment each channel
      temp = Image_getColor(tex->img, j, i);
      r += temp.c[0];
      g += temp.c[1];
      b += temp.c[2];
    }
  }
  
  // divide each color by the number of pixels
  color.c[0] = r / ((x1-x0)*(y1-y0));
  color.c[1] = g / ((x1-x0)*(y1-y0));
  color.c[2] = b / ((x1-x0)*(y1-y0));
  
  return color;
}
