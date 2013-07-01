// Dan Nelson and Hieu Phan
// Project 5
// October 12, 2011
// cb_matrix.h

#ifndef CB_VIEW_H

#define CB_VIEW_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// VIEW 2D
typedef struct {
  Point vrp; // center of view rectangle in world coordinates
  double dx; // width of the view rectangle in world coordinates
  Vector x; // orientation angle expressed as a normalized vector
  int screenx; // number of columns in the output image
  int screeny; // number of rows in the output image
} View2D;

// VIEW 3D
typedef struct {
  Point vrp;
  Vector vpn;
  Vector vup;
  double d;
  double du;
  double dv;
  double f;
  double b;
  int screenx;
  int screeny;
} View3D;


/*****************************************
 *			2D VIEWING FUNCTIONS		 * 
 *****************************************/
void View2D_set( View2D *view, Point *vrp, double dx, Vector *x, int screenx, int screeny);
void Matrix_setView2D(Matrix *vtm, View2D *view);


/*****************************************
 *			3D VIEWING FUNCTIONS		 * 
 *****************************************/
void Matrix_setView3D(Matrix *vtm, View3D *view);


#endif



