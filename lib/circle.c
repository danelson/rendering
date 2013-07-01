/* Dan Nelson
 * Graphics Package
 * circle.c
 * Functions for circles
 */

 
#include "cb_graphics.h"


//////////////////////////////////
///////// CIRCLE FUNCTIONS ///////
//////////////////////////////////

/* initialize to center tc and radius tr */
void Circle_set(Circle *circle, Point tc, double tr) {
  circle->r = tr;
  circle->c = tc;
}


/* Draws the circle into src using color color */
void Circle_draw(Circle *circle, Image *src, Color color) {
  int cy, cx, radius;
  int x, y, p;
  
  radius = circle->r;  
  //gets the coordinates of the center
  cx = circle->c.val[0];
  cy = circle->c.val[1];
  
  x = -1;
  y = -radius;
  p = 1 - radius;
  
  //Plots (x + cx, y + cy) and reflect it around the circle
  Circle_plotPoints(src, cx, cy, x, y, color);
  
  while (x>y) {
    x--;
    if (p<0) {
      p = p + 1 - 2*x;
    }
    else {
      y++;
      p = p + 1 - 2*(x - y);
    }
    //Plots ( x + cx, y + cy ) and its reflections
    Circle_plotPoints(src, cx, cy, x, y, color);
  }  
}


/* Draws the circle into src using color color */
void Circle_drawXForm(Circle *circle, Image *src, Color color, Matrix* xForm) {
  int cy, cx, radius;
  int x, y, p;
  
  radius = circle->r;  
  //gets the coordinates of the center
  cx = circle->c.val[0];
  cy = circle->c.val[1];
  
  x = -1;
  y = -radius;
  p = 1 - radius;
  
  //Plots (x + cx, y + cy) and reflect it around the circle
  Circle_plotPointsXForm(src, cx, cy, x, y, color, xForm);
  
  while (x>y) {
    x--;
    if (p<0) {
      p = p + 1 - 2*x;
    }
    else {
      y++;
      p = p + 1 - 2*(x - y);
    }
    //Plots ( x + cx, y + cy ) and its reflections
    Circle_plotPointsXForm(src, cx, cy, x, y, color, xForm);
  }  
}


/* Plots point on circle and its reflection */
void Circle_plotPoints(Image *src, int cx, int cy, int x, int y, Color color) {  
  Image_setColor(src, cy + y, cx + x , color);
  Image_setColor(src, cy + y, cx - x - 1,  color);
  Image_setColor(src, cy - y - 1, cx + x, color);
  Image_setColor(src, cy - y - 1, cx - x - 1, color);
  Image_setColor(src, cy + x, cx + y, color);
  Image_setColor(src, cy + x, cx - y - 1, color);
  Image_setColor(src, cy - x - 1, cx + y, color);
  Image_setColor(src, cy - x - 1, cx - y - 1, color);
}

/* Plots point on circle and its reflection */
void Circle_plotPointsXForm(Image *src, int cx, int cy, int x, int y, Color color, Matrix* xForm) {  
  Point p;
  Point q;
  
  Point_set(&p, cy+y, cx+x, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy+y, cx-x-1, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy-y-1, cx+x, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy-y-1, cx-x-1, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy+x, cx+y, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy+x, cx-y-1, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy-x-1, cx+y, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
  Point_set(&p, cy-x-1, cx-y-1, 0);
  Matrix_xformPoint(xForm, &p, &q);
  Point_normalize(&q);
  Point_draw(&q, src, color);
  
}


/* draw a filled circle into src using color p */
void Circle_drawFill(Circle *circle, Image *src, Color color) {
  int cy, cx, radius;
  int x, y, p;
  
  radius = circle->r;  
  //gets the coordinates of the center
  cx = circle->c.val[0];
  cy = circle->c.val[1];
  
  x = -1;
  y = -radius;
  p = 1 - radius;
  
  //Plots (x + cx, y + cy) and reflect it around the circle
  Circle_fillPoints(src, cx, cy, x, y, color);
  
  while (x>y) {
    x--;
    if (p<0) {
      p = p + 1 - 2*x;
    }
    else {
      y++;
      p = p + 1 - 2*(x - y);
    }
    //Plots ( x + cx, y + cy ) and its reflections
    Circle_fillPoints(src, cx, cy, x, y, color);
  }
}


/* Fills the circle by drawing lines */
void Circle_fillPoints(Image *src, int cx, int cy, int x, int y, Color color) {  
  int i;
  printf("filling the circle: %d  %d \n",cx-x-1,cx+x);
  for (i = cx+x; i < cx - x - 1; i++) {
    printf("i: %d \n",i);
    Image_setColor(src, cy + y, i,  color);
    Image_setColor(src, cy - y - 1, i, color);
  }
  
  for (i = cx + y; i < cx - y - 1; i++) {
    Image_setColor(src, cy + x, i,  color);
    Image_setColor(src, cy - x - 1, i, color);
  }
}