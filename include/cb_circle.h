// Dan Nelson and Hieu Phan
// Project 3
// September 29, 2011
// cb_circle.h

#ifndef CB_CIRCLE_H

#define CB_CIRCLE_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/
 
// Circle
typedef struct {
  double r; // radius
  Point c; // center
} Circle;


/*****************************************
 *			CIRCLE FUNCTIONS		     * 
 *****************************************/
 
void Circle_set(Circle *circle, Point tc, double tr);
void Circle_plotPoints(Image *src, int cx, int cy, int x, int y, Color color);
void Circle_draw(Circle *circle, Image *src, Color color);
void Circle_drawFill(Circle *circle, Image *src, Color color);
void Circle_drawXForm(Circle *circle, Image *src, Color color, Matrix* xForm);
void Circle_plotPointsXForm(Image *src, int cx, int cy, int x, int y, Color color, Matrix* xForm);
void Circle_fillPoints(Image *src, int cx, int cy, int x, int y, Color color);




#endif