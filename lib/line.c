/* Dan Nelson
 * Graphics Package
 * line.c
 */


#include "cb_graphics.h"

//////////////////////////////////
///////// LINE FUNCTIONS /////////
//////////////////////////////////

/* initialize a 2D line */
void Line_set2D(Line *l, double x0, double y0, double x1, double y1) {
	Point_set2D(&(l->a),x0,y0);
	Point_set2D(&(l->b),x1,y1);
	l->a.val[2] = 0.0;
	l->a.val[3] = 1.0;
	l->b.val[2] = 0.0;
	l->b.val[3] = 1.0;
	l->zBuffer = 1; //use zbuffer by default
}


/* initialize a line to ta and tb */
void Line_set(Line *l, Point ta, Point tb) {
	l->a = ta;
	l->b = tb;
	l->zBuffer = 1; //use zbuffer by default
}

/* normalizes the endpoints by their homogeneous coordinate */
void Line_normalize(Line *l) {
	l->a.val[0] = l->a.val[0] / l->a.val[3];
	l->a.val[1] = l->a.val[1] / l->a.val[3];
	//l->a.val[2] = l->a.val[2] / l->a.val[3]; DO NOT TOUCH z value
	l->a.val[3] = 1.0;
	l->b.val[0] = l->b.val[0] / l->b.val[3];
	l->b.val[1] = l->b.val[1] / l->b.val[3];
	//l->b.val[2] = l->b.val[2] / l->b.val[3]; DO NOT TOUCH z value
	l->b.val[3] = 1.0;
}


/* set the z-buffer flag to the given value. */
void Line_zBuffer(Line *l, int flag) {
	l->zBuffer = flag;
}

void Line_drawNoZBuffer(Line *l, Image *src, Color c);
void Line_drawNoZBuffer(Line *l, Image *src, Color c) {
  int x0,y0,x1,y1;
  int x,y,dx,dy,e;
  int i;
  int range;
  
  x0 = l->a.val[0];
  y0 = l->a.val[1];
  x1 = l->b.val[0];
  y1 = l->b.val[1];

  dx = x1 - x0;
  dy = y1 - y0;
 
  //make sure we are drawing from bottom point to top point of the line
  if (dy<0) {
    x = x1;
    y = y1;
    dy = -dy;
    dx = -dx;
  }
  else {
    x = x0;
    y = y0;
  }
      
  //first octant
  if (dx > 0) {
	if ((dx>=abs(dy)) && (dy!=0)) {
	  e = 3*dy - 2*dx;
	  for (i = 0; i<dx; i++) {
	    if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      while(e > 0) {
		    y++;
		    e = e - 2*dx;
		  }
		  x++;
		  e = e + 2*dy;
	    }
	    else {
		  Image_setColor(src, y, x, c);
		  while(e > 0) {
		    y++;
		    e = e - 2*dx;
		  }
		  x++;
		  e = e + 2*dy;
		}
	  }
	}
	//second octant
	else if ((dx<dy) && (dy!=0)) {
	  e = 3*dx - 2*dy;
	  for (i = 0; i<dy; i++) {
	    if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      while(e > 0) {
		    x++;
		    e = e - 2*dy;
		  }
		  y++;
		  e = e + 2*dx;
	    }
	    else {
	      Image_setColor(src, y, x, c);
		  while(e > 0) {
		    x++;
		    e = e - 2*dy;
		  }
		  y++;
		  e = e + 2*dx;
		}
	  }
	}
	else {
	  // make sure we start at the correct point
      x = x0;
      y = y0;
      //drawing from left to right in the first quadrant
      range = abs(dx);
      for (i = 0; i<range; i++) {
        if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      x++;
	    }
	    else {
          Image_setColor(src, y, x, c);
	      x++;
	    }
      }
	}
  }
  //3rd octant
  else if (dx < 0) {
	if ((abs(dx)<abs(dy)) && (dy!=0)) {
	  x--;
	  e = -3*dx - 2*dy;
	  for (i = 0; i<dy; i++) {
	    if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      while(e > 0) {
	        x--;
		    e = e - 2*dy;
		  }
		  y++;
		  e = e - 2*dx;
	    }
	    else {
	      Image_setColor(src, y, x, c);
	      while(e > 0) {
	        x--;
		    e = e - 2*dy;
		  }
		  y++;
		  e = e - 2*dx;
		}
	  }
	}
	//4th octant
	else if ((abs(dx)>=abs(dy)) && (dy!=0)){
	  x--;
	  e = 3*dy + 2*dx;
	  range = abs(dx);
	  for (i = 0; i<range; i++) {
	    if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      while(e > 0) {
		    y++;
		    e = e + 2*dx;
		  }
		  x--;
		  e = e + 2*dy;
	    }
	    else {
	      Image_setColor(src, y, x, c);
		  while(e > 0) {
		    y++;
		    e = e + 2*dx;
		  }
		  x--;
		  e = e + 2*dy;
		}
	  }
	}
	else {
	  // make sure we start at the correct point
      x = x0;
      y = y0;
      range = abs(dx);
      for (i = 0; i<range; i++) {
        if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      x--;
	    }
	    else {
          Image_setColor(src, y-1, x-1, c);
	      x--;
	    }
      }
    }
  }
  //vertical
  else if (dx == 0) {
    // make sure we start at the correct point
    y = y0;
    x = x0;
    // going down in 4th quadrant
    if (y0>y1) {
      for (i = 0; i<dy; i++) {
        if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      y--;
	    }
	    else {
          Image_setColor(src, y-1, x, c);
	      y--;
	    }
      }
    }
    // going up in 2nd quadrant
    else {
      for (i = 0; i<dy; i++) {
        if (x<0 || x>=src->cols || y<0 || y >=src->rows) {
	      y++;
	    }
	    else {
          Image_setColor(src, y, x-1, c);
	      y++;
	    }
      }
    }
  }  
}

void Line_drawZBuffer(Line *l, Image *src, Color c);
void Line_drawZBuffer(Line *l, Image *src, Color c) {
  int x0,y0,x1,y1;
  int x,y,dx,dy,dz,e;
  double z,z0,z1;
  double delta1z, current1z;
  int i;
  int range;
  delta1z = 0.0;
  
  printf("IN DRAW Z BUFFER .... \n");
  x0 = l->a.val[0];
  y0 = l->a.val[1];
  z0 = 1.0/l->a.val[2];
  x1 = l->b.val[0];
  y1 = l->b.val[1];
  z1 = 1.0/l->b.val[2];
  
 
  dx = x1 - x0;
  dy = y1 - y0;
  dz = z1 - z0;
 
  //make sure we are drawing from bottom point to top point of the line
  if (dy<0) {
    x = x1;
    y = y1;
    z = z1;
    dy = -dy;
    dx = -dx;
    dz = -dz;
  }
  else {
    x = x0;
    y = y0;
    z = z0;
  }
  

  current1z = z;
  
  if (dx > 0) {
	//first octant
	if ((dx>=abs(dy)) && (dy!=0)) {
	  e = 3*dy - 2*dx;
	  delta1z = dz/dx;
	  for (i = 0; i<dx; i++) {
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  printf("current1z %lf \n",current1z);
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
		  while(e > 0) {
		    y++;
		    e = e - 2*dx;
		  }
		  x++;
		  e = e + 2*dy;
	    }
	}
	//second octant
	else if ((dx<dy) && (dy!=0)) {
	  printf("before delta1z \n");
	  delta1z = dz/dy;
	  printf("after delta1z \n");
	  e = 3*dx - 2*dy;
	  for (i = 0; i<dy; i++) {
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  printf("current1z %lf \n",current1z);
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
		  while(e > 0) {
		    x++;
		    e = e - 2*dy;
		  }
		  y++;
		  e = e + 2*dx;
		}
	}
	else {
	  //make sure we start at the correct point
      x = x0;
      y = y0;
      //drawing from left to right in the first quadrant
      range = abs(dx);
      for (i = 0; i<range; i++) {
		  printf("current1z %lf \n",current1z);
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
	      x++;
	    }
	 }
  }
  //3rd octant
  else if (dx < 0) {
	if ((abs(dx)<abs(dy)) && (dy!=0)) {
	  printf("before delta1z \n");
	  delta1z = dz/dy;
	  printf("after delta1z \n");
	  x--;
	  e = -3*dx - 2*dy;
	  for (i = 0; i<dy; i++) {
		  printf("current1z %lf \n",current1z);
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
	      while(e > 0) {
	        x--;
		    e = e - 2*dy;
		  }
		  y++;
		  e = e - 2*dx;
		}
	}
	//4th octant
	else if ((abs(dx)>=abs(dy)) && (dy!=0)){
	  printf("after delta1z \n");
	  delta1z = dz/dx;
	  printf("after delta1z \n");
	  x--;
	  e = 3*dy + 2*dx;
	  range = abs(dx);
	  for (i = 0; i<range; i++) {
		  printf("current1z %lf \n",current1z);
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
		  while(e > 0) {
		    y++;
		    e = e + 2*dx;
		  }
		  x--;
		  e = e + 2*dy;
		}
	}
	else {
	  // make sure we start at the correct point
      x = x0;
      y = y0;
      range = abs(dx);
      for (i = 0; i<range; i++) {
		  printf("current1z %lf \n",current1z);
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
	      x--;
	    }
      }
    }
  //vertical
  else if (dx == 0) {
    // make sure we start at the correct point
    y = y0;
    x = x0;
    // going down in 4th quadrant
    if (y0>y1) {
      for (i = 0; i<dy; i++) {
		  printf("current1z %lf \n",current1z);
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  if (current1z > Image_getz(src, y, x)) {
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
	      y--;
	    }
     }
    // going up in 2nd quadrant
    else {
      for (i = 0; i<dy; i++) {
          printf("current1z %lf \n",current1z);
		  //if the current 1/z value > the current z-buffer value at (row, col)
		  if (current1z > Image_getz(src, y, x)) {
		    //printf("lighting up pixel...%d %d \n",y,x);
		    //update the current z-buffer value to the current 1/z value
		    Image_setz(src,y,x,current1z);
		    //draw the current color into the image at (row, col)
		    printf("before set COLOR WTF IS HAPPENING \n");
		    Image_setColor(src, y, x, c);
		    printf("after set COLOR I HATE MY LIFE \n");
		  }
		  //update the current 1/z value
		  current1z += delta1z;
	      y++;
	  }
    }
  }
}


/* Draw the line into src using color p.
 * If the z-buffer flag is set, the algorithm 
 * should take into account z-buffer values when drawing the line */
void Line_draw(Line *l, Image *src, Color c) {
  if (l->zBuffer == 1) {
    Line_drawZBuffer(l,src,c);
  }
  else {
    Line_drawNoZBuffer(l,src,c);
  }
}
 


/* Draw the dashed line into src using color p. 
 * If the z-buffer flag is set, the algorithm 
 * should take into account z-buffer values when drawing the line (not yet). 
 * the parameter int gap decides how dashed the line will be 
 * eg. if gap = 4 and gapsize = 2, there will be a gap of 2 pixels every 4 pixel */
void Line_drawDashed(Line *l, Image *src, Color c, int gap, int gapSize) {
	int x0,y0,x1,y1;
	int x,y,dx,dy,e;
	int i;
	
	x0 = l->a.val[0];
	y0 = l->a.val[1];
	x1 = l->b.val[0];
	y1 = l->b.val[1];
	
	dx = x1 - x0; 
	dy = y1 - y0;
	
	//make sure we are drawing from bottom point to top point of the line
	if (dy<0) {
		x = x1;
		y = y1;
		dy = -dy;
		dx = -dx;
	}
	else {
		x = x0;
		y = y0;
	}
	
	//first octant
	if ((dx>=abs(dy)) && (dx>0) && (dy!=0)) {
		e = 3*dy - 2*dx;
		for (i = 0; i<dx; i++) {
			if (i%(gap+gapSize) < gap) {
				Image_setColor(src, y, x, c);
			}
			while(e > 0) {
				y = y + 1;
				e = e - 2*dx;
			}
			x = x + 1;
			e = e + 2*dy;
		}
	}
	//second octant
	else if ((dx<dy) && (dx>0) && (dy!=0)) {
		e = 3*dx - 2*dy;
		for (i = 0; i<dy; i++) {
			if (i%(gap+gapSize) < gap) {
				Image_setColor(src, y, x, c);
			}
			while(e > 0) {
				x = x + 1;
				e = e - 2*dy;
			}
			y = y + 1;
			e = e + 2*dx;
		}
	}
	//3rd octant
	else if ((abs(dx)<abs(dy)) && (dx<0) && (dy!=0)) {
		x = x - 1;
		e = -3*dx - 2*dy;
		for (i = 0; i<dy; i++) {
			if (i%(gap+gapSize) < gap) {
				Image_setColor(src, y, x, c);
			}
			while(e > 0) {
				x = x - 1;
				e = e - 2*dy;
			}
			y = y + 1;
			e = e - 2*dx;
		}
	}
	//4th octant
	else if ((abs(dx)>=abs(dy)) && (dx<0) && (dy!=0)){
		x = x - 1;
		e = 3*dy + 2*dx;
		for (i = 0; i<abs(dx); i++) {
			if (i%(gap+gapSize) < gap) {
				Image_setColor(src, y, x, c);
			}
			while(e > 0) {
				y = y + 1;
				e = e + 2*dx;
			}
			x = x - 1;
			e = e + 2*dy;
		}
	}
	//vertical
	else if (dx==0) {
		// make sure we start at the correct point
		y = y0;
		x = x0;
		// going down in 4th quadrant
		if (y0>y1) {
			for (i = 0; i<dy; i++) {
				if (i%(gap+gapSize) < gap) {
					Image_setColor(src, y, x, c);
				}
				y = y-1;	
			}
		}
		// going up in 2nd quadrant
		else {
			for (i = 0; i<dy; i++) {
				if (i%(gap+gapSize) < gap) {
					Image_setColor(src, y, x, c);
				}
				y = y+1;	
			}
		}
	}
	//horizontal, dy=0
	else {    
		// make sure we start at the correct point
		x = x0;
		y = y0;
		//drawing from left to right in the first quadrant
		if (x1>x0) {
			for (i = 0; i<abs(dx); i++) {
				if (i%(gap+gapSize) < gap) {
					Image_setColor(src, y, x, c);
				}
				x = x+1;	
			}
		}
		//drawing from right to left in the third quadrant
		else {
			for (i = 0; i<abs(dx); i++) {
				if (i%(gap+gapSize) < gap) {
					Image_setColor(src, y, x, c);
				}
				x = x-1;	
			}
		}
	}
}

void swap(int x, int y);
void swap(int x, int y) {
	int temp;
	temp = y;
	y = x;
	x = temp;
}


#define ipart_(X) ((int)(X))
#define round_(X) ((int)(((double)(X))+0.5))
#define fpart_(X) (((double)(X))-(double)ipart_(X))
#define rfpart_(X) (1.0-fpart_(X))


// draws a line with anti aliasing
void Line_drawSmoothly(Line *l, Image *src, Color c) {
	int x0,y0,x1,y1;
	int x;
	
	x0 = l->a.val[0];
	y0 = l->a.val[1];
	x1 = l->b.val[0];
	y1 = l->b.val[1];
	
	double dx = (double)x1 - (double)x0; 
	double dy = (double)y1 - (double)y0;
	
	if (abs(dx) < abs(dy)) {                 
		swap(x0,y0);
		swap(x1,y1);
		swap(dx,dy);
	}
	
	if (x1 < x0) {
		swap(x0,x1);
		swap(y0,y0);
	}
	if ((dx != 0) && (dy != 0)) {
		double gradient = dy / dx;
		
		// handle first endpoint
		int xend = round_(x0);
		int yend = y0 + gradient * (xend - x0);
		
		double xgap = 1 - rfpart_(x1 + 0.5);
		int xpxl1 = xend; // this will be used in the main loop
		int ypxl1 = ipart_(yend);
		Image_setColorWB(src, ypxl1, xpxl1, c, rfpart_(yend) * xgap);
		Image_setColorWB(src, ypxl1+1, xpxl1, c, rfpart_(yend) * xgap);
		double intery = yend + gradient; // first y-intersection for the main loop
		
		// handle second endpoint
		xend = round_(x1);
		yend = y1 + gradient * (xend - x1);
		xgap = fpart_(x1 + 0.5);
		int xpxl2 = xend; //this will be used in the main loop
		int ypxl2 = ipart_(yend);
		Image_setColorWB(src, ypxl2, xpxl2, c, rfpart_(yend) * xgap);
		Image_setColorWB(src, ypxl1+2, xpxl2, c, fpart_(yend) * xgap);
		
		// main loop
		for (x = xpxl1 + 1; x<xpxl2 - 2; x++) { 
			Image_setColorWB(src, (int)(intery), x, c, rfpart_(intery));
			Image_setColorWB(src, (int)(intery)+1, x, c, fpart_(intery));
			intery = intery + gradient;
		}
	}
}
