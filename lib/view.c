/* Dan Nelson
 * Graphics Package
 * view.c
 * Functions for view
 */


#include "cb_graphics.h"


/******************
VIEW FUNCTIONS
******************/
/*
typedef struct {
  Point vrp; // center of view rectangle in world coordinates
  double dx; // width of the view rectangle in world coordinates
  Vector x; // orientation angle expressed as a normalized vector
  int screenx; // number of columns in the output image
  int screeny; // number of rows in the output image
} View2D;
*/

/* Sets the fields in the View2D structure */
void View2D_set( View2D *view, Point *vrp, double dx, Vector *x, int screenx, int screeny) {
  view->vrp = *vrp;
  view->dx = dx;
  view->x = *x;
  view->screenx = screenx;
  view->screeny = screeny;
}

/* Sets vtm to be the view transformation defined by the 2DView structure. */
void Matrix_setView2D(Matrix *vtm, View2D *view) {
  double dy = (view->dx * view->screeny)/view->screenx;
  
  Matrix_identity(vtm);
  Matrix_translate2D(vtm,-(view->vrp.val[0]),-(view->vrp.val[1]));
  Matrix_rotateZ(vtm,view->x.v[0],-(view->x.v[1]));
  Matrix_scale2D(vtm,view->screenx/view->dx,-(view->screeny/dy));
  Matrix_translate2D(vtm,view->screenx/2,view->screeny/2);
}

/* */
void Matrix_setView3D(Matrix *vtm, View3D *view) {
  
  Vector u, vpn, vup;
  double b,d;
  
  // copy vectors
  Vector_copy(&vup, &view->vup);
  Vector_copy(&vpn, &view->vpn);
  
  // initialize vtm to the identity
  Matrix_identity(vtm);
  
  // calculate u vector for the view reference coordinate system
  Vector_cross(&vup, &vpn, &u);
  
  // recalculate vup so reference coordinates are orthogonal
  Vector_cross(&vpn, &u, &vup);
  
  // translate vrp to the origin
  Matrix_translate(vtm,
                   -(view->vrp.val[0]),
                   -(view->vrp.val[1]),
                   -(view->vrp.val[2]));
                   
  printf("VRP to origin\n");
  Matrix_print(vtm, stdout);
  
  //normalize u, vup, and vpn
  Vector_normalize(&u);
  Vector_normalize(&vup);
  Vector_normalize(&vpn);
  
  // align the axes
  Matrix_rotateXYZ(vtm, &u, &vup, &vpn);
  printf("Align axes\n");
  Matrix_print(vtm, stdout);
  
  // translate the cop to the origin
  Matrix_translate(vtm, 0, 0, view->d);
  printf("COP to origin\n");
  Matrix_print(vtm, stdout);
  
  // update the back clip plane
  b = view->d + view->b;
  
  // scale the canonical view volume
  Matrix_scale(vtm, 2.0*view->d/(b*view->du), 2.0*view->d/(b*view->dv), 1.0/b);
  printf("Scale CVV\n");
  Matrix_print(vtm, stdout);
  
  // update the focal length
  d = view->d / b;
  
  // project the scene onto the view plane
  Matrix_perspective(vtm, d);
  printf("After perspective\n");
  Matrix_print(vtm, stdout);
  
  // scale to the image size
  Matrix_scale2D(vtm,-view->screenx/(2*d),-view->screeny/(2*d));
  printf("Scale image cords\n");
  Matrix_print(vtm, stdout);
  
  // negate the x and y axis and translate the origin to the center of the image
  Matrix_translate2D(vtm,view->screenx/2,view->screeny/2);
  printf("After final translation\n");
  Matrix_print(vtm, stdout);
}

