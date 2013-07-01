// Dan Nelson and Hieu Phan
// Project 5
// October 12, 2011
// cb_matrix.h

#ifndef CB_MATRIX_H

#define CB_MATRIX_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// Matrix
typedef struct {
  double m[16];
} Matrix;



/*****************************************
 *			MATRIX FUNCTIONS		     * 
 *****************************************/
void Matrix_print(Matrix *m, FILE *fp);
void Matrix_clear(Matrix *m);
void Matrix_identity(Matrix *m);
double Matrix_get(Matrix *m, int r, int c);
void Matrix_set(Matrix *m, int r, int c, double v);
void Matrix_copy(Matrix *dest, Matrix *src);
void Matrix_transpose(Matrix *m);
void Matrix_multiply(Matrix *left, Matrix *right, Matrix *m);
void Matrix_xformPoint(Matrix *m, Point *p, Point *q);
void Matrix_xformVector(Matrix *m, Vector *p, Vector *q);
void Matrix_xformPolygon(Matrix *m, Polygon *p);
void Matrix_xformPolyline(Matrix *m, Polyline *p);
void Matrix_xformLine(Matrix *m, Line *line);
void Matrix_scale2D(Matrix *m, double sx, double sy);
void Matrix_rotateZ(Matrix *m, double cth, double sth);
void Matrix_translate2D(Matrix *m, double tx, double ty);
void Matrix_shear2D(Matrix *m, double shx, double shy);


/**************
 *	   3D     * 
 **************/
void Matrix_translate(Matrix *m, double tx, double ty, double tz);
void Matrix_scale(Matrix *m, double sx, double sy, double sz);
void Matrix_rotateX(Matrix *m, double cth, double sth);
void Matrix_rotateY(Matrix *m, double cth, double sth);
void Matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w);
void Matrix_shearZ(Matrix *m, double shx, double shy);
void Matrix_perspective(Matrix *m, double d);


#endif