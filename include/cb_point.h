// Dan Nelson and Hieu Phan
// Project 3
// September 23, 2011
// cb_point.h

#ifndef CB_POINT_H

#define CB_POINT_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// Point
typedef struct {
  double val[4];
} Point;


/*****************************************
 *			POINT FUNCTIONS		         * 
 *****************************************/
 
void Point_set2D(Point *p, double x, double y);
void Point_set(Point *p, double x, double y, double z);
void Point_draw(Point *p, Image *src, Color c);
void Point_drawf(Point *p, Image *src, FPixel fp);
void Point_copy(Point *to, Point *from);
void Point_normalize(Point *p);
double Point_dist2(Point *p1, Point *p2);
double Point_dist(Point *p1, Point *p2);
void Point_print(Point *p, FILE *fp);


#endif