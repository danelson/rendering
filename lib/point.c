/* Dan Nelson
 * Graphics Package
 * point.c
 */


#include "cb_graphics.h"

//////////////////////////////////
///////// POINT FUNCTIONS ////////
//////////////////////////////////



/* Set the first two values of the vector to x and y. 
 * Set the third value to zero and the fourth value to 1.0. */
void Point_set2D(Point *p, double x, double y) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = 0.0;
  p->val[3] = 1.0;
}

/* set the four values of the vector to x, y, z, and h, respectively */
void Point_set(Point *p, double x, double y, double z) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = 1.0;
}

/* draw the point into src using color c.*/
void Point_draw(Point *p, Image *src, Color c) {
  printf("point info: row: %lf col: %lf \n", p->val[0], p->val[1]);
  printf("image info: row: %ld, col: %ld \n", src->rows, src->cols);
  Image_setColor(src,p->val[1],p->val[0],c);
}

/* draw the point into src using FPixel p. */
void Point_drawf(Point *p, Image *src, FPixel fp) {
  int row = p->val[0];
  int col = p->val[1];
  Image_setf(src,row,col,fp);
}

/* copy the point */
void Point_copy(Point *to, Point *from) {
  to->val[0] = from->val[0];
  to->val[1] = from->val[1];
  to->val[2] = from->val[2];
  to->val[3] = from->val[3];
}

/* normalizes the point by its homogeneous coordinate
 * x = x/h, y = y/h, z = z/h
 */
void Point_normalize(Point *p) {
  p->val[0] = p->val[0] / p->val[3];
  p->val[1] = p->val[1] / p->val[3];
  //p->val[2] = p->val[2] / p->val[3];
  p->val[3] = 1.0;
}

// calculate the squared distance between two points
double Point_dist2(Point *p1, Point *p2) {
  return (p1->val[0]-p2->val[0])*(p1->val[0]-p2->val[0])
  		 	   + (p1->val[1]-p2->val[0])*(p1->val[1]-p2->val[1])
  			   + (p1->val[2]-p2->val[0])*(p1->val[2]-p2->val[2]);
}

// calculate the distance between two points
double Point_dist(Point *p1, Point *p2) {
  return sqrt( (p1->val[0]-p2->val[0])*(p1->val[0]-p2->val[0])
  		 	   + (p1->val[1]-p2->val[1])*(p1->val[1]-p2->val[1])
  			   + (p1->val[2]-p2->val[2])*(p1->val[2]-p2->val[2]) );
}

/* Print out the Vector in a beautiful form */
void Point_print(Point *p, FILE *fp) {
  fprintf(fp,"x: %lf, y: %lf, z: %lf \n",p->val[0],p->val[1],p->val[2]);
}





