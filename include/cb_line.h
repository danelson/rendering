// Dan Nelson and Hieu Phan
// Project 3
// September 29, 2011
// cb_line.h

#ifndef CB_LINE_H

#define CB_LINE_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// Line
typedef struct {
  int zBuffer; // whether to use the z-buffer. default to true(1)
  Point a; // starting point
  Point b; // ending point
} Line;


/*****************************************
 *			LINE FUNCTIONS		         * 
 *****************************************/
void Line_set2D(Line *l, double x0, double y0, double x1, double y1);
void Line_set(Line *l, Point ta, Point tb) ;
void Line_zBuffer(Line *l, int flag);
void Line_normalize(Line *l);
void Line_draw(Line *l, Image *src, Color c);
void Line_drawDashed(Line *l, Image *src, Color c, int gap, int gapSize);
void Line_drawSmoothly(Line *l, Image *src, Color c);





#endif