/* Dan Nelson
 * Graphics Package
 * ellipse.c
 * Functions for ellipse
 */


#include "cb_graphics.h"



//////////////////////////////////
///////// ELLIPSE FUNCTIONS //////
//////////////////////////////////

/* initialize an ellipse to location tc and radii ta and tb. */
void Ellipse_set(Ellipse *e, Point tc, double ta, double tb) {
  e->c = tc;
  e->ra = ta;
  e->rb = tb;
}


/* Plots point on circle and its reflection */
void Ellipse_plotPoints(Image *src, int cx, int cy, int x, int y, Color color) {  
  Image_setColor(src, cy + y, cx + x, color); 
  Image_setColor(src, cy + y, cx - x - 1, color); 
  Image_setColor(src, cy - y - 1, cx + x, color); 
  Image_setColor(src, cy - y - 1, cx - x - 1, color); 
}


/* draw into src using color p */
void Ellipse_draw(Ellipse *e, Image *src, Color color){
  int cx, cy, Rx, Ry;
  int x, y, px, py, p;
  
  //gets the coordinates of the center
  cx = e->c.val[0];
  cy = e->c.val[1];
  //get the radii
  Rx = e->ra;
  Ry = e->rb;
  
  x = -1;
  y = -Ry;
  px = 2 * Ry * Ry;
  py = 2 * Rx * Rx * (-y);
  
  //Plot (x+cx, y+cy) and its four reflections
  Ellipse_plotPoints(src, cx, cy, x, y, color);
  
  p = Ry * Ry - Rx * Rx * Ry + Rx*Rx/4 + Ry*Ry + px;
  
  while (px < py) {
    x--;
    px = px + 2 * Ry * Ry;
    if (p < 0) {
      p = p + Ry * Ry + px;
    }
    else {
      y++;
      py = py - 2 * Rx * Rx;
      p = p + Ry * Ry + px - py;
    }
    //Plot (x+cx, y+cy) and its four reflections
    Ellipse_plotPoints(src, cx, cy, x, y, color); 
  }
  p = Ry*Ry * (x*x + x) + Rx*Rx * (y*y - 2*y + 1) -Rx*Rx * Ry*Ry+ Rx*Rx - py;
  
  while (y < 0) {
    y++;
    py = py - 2 * Rx * Rx;
    if (p > 0) {
      p = p + Rx * Rx - py;
    }
    else {
      x--;
      px = px + 2 * Ry * Ry;
      p = p + Rx * Rx - py + px;
    }
    //Plot (x+cx, y+cy) and its four reflections
    Ellipse_plotPoints(src, cx, cy, x, y, color);
  }
}


/* draws a filled ellipse */
void Ellipse_drawFill(Ellipse *e, Image *src, Color color) {
  int cx, cy, Rx, Ry;
  int x, y, px, py, p;
  
  //gets the coordinates of the center
  cx = e->c.val[0];
  cy = e->c.val[1];
  //get the radii
  Rx = e->ra;
  Ry = e->rb;
  
  x = -1;
  y = -Ry;
  px = 2 * Ry * Ry;
  py = 2 * Rx * Rx * (-y);
  
  //fill the ellipse by drawing 2 lines from found points
  Ellipse_fillPoints(src, cx, cy, x, y, color);
  
  p = Ry * Ry - Rx * Rx * Ry + Rx*Rx/4 + Ry*Ry + px;
  
  while (px < py) {
    x--;
    px = px + 2 * Ry * Ry;
    if (p < 0) {
      p = p + Ry * Ry + px;
    }
    else {
      y++;
      py = py - 2 * Rx * Rx;
      p = p + Ry * Ry + px - py;
    }
    //fill the ellipse by drawing 2 lines from found points
    Ellipse_fillPoints(src, cx, cy, x, y, color); 
  }
  p = Ry*Ry * (x*x + x) + Rx*Rx * (y*y - 2*y + 1) -Rx*Rx * Ry*Ry+ Rx*Rx - py;
  
  while (y < 0) {
    y++;
    py = py - 2 * Rx * Rx;
    if (p > 0) {
      p = p + Rx * Rx - py;
    }
    else {
      x--;
      px = px + 2 * Ry * Ry;
      p = p + Rx * Rx - py + px;
    }
    //fill the ellipse by drawing 2 lines from found points
    Ellipse_fillPoints(src, cx, cy, x, y, color);
  }
}


/* fill an ellipse by drawing lines */
void Ellipse_fillPoints(Image *src, int cx, int cy, int x, int y, Color color) {
  int i;
  
  for (i = cx+x; i < cx - x - 1; i++) {
    Image_setColor(src, cy + y, i, color);
    Image_setColor(src, cy - y - 1, i, color); 
  }
}
