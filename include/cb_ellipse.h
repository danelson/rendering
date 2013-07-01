// Dan Nelson and Hieu Phan
// Project 3
// September 29, 2011
// cb_ellipse.h

#ifndef CB_ELLIPSE_H

#define CB_ELLIPSE_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/
 
// Ellipse
typedef struct {
  double ra; // major axis radius
  double rb; // minor axis radius
  Point c; // center
  //double a; // (optional) angle of major axis relative to the X-axis
} Ellipse;


/*****************************************
 *			ELLIPSE FUNCTIONS		     * 
 *****************************************/
void Ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void Ellipse_plotPoints(Image *src, int cx, int cy, int x, int y, Color color);
void Ellipse_draw(Ellipse *e, Image *src, Color color);
void Ellipse_drawFill(Ellipse *e, Image *src, Color color);
void Ellipse_fillPoints(Image *src, int cx, int cy, int x, int y, Color color);



#endif