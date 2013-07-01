// Dan Nelson and Hieu Phan
// Project 5
// October 12, 2011
// cb_polygon.h

#ifndef CB_VECTOR_H

#define CB_VECTOR_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// Vector
typedef struct {
  double v[4];
} Vector;


/*****************************************
 *			VECTOR FUNCTIONS		     * 
 *****************************************/
void Vector_set(Vector *v, double x, double y, double z);
void Vector_print(Vector *v, FILE *fp);
void Vector_copy(Vector *dest, Vector *src);
double Vector_length(Vector *v);
double Vector_lenSquared(Vector *v);
void Vector_normalize(Vector *v);
double Vector_dot(Vector *a, Vector *b);
void Vector_cross(Vector *a, Vector *b, Vector *c);
void Vector_sum(Vector *a, Vector *b, Vector *c);
void Vector_scale(Vector *a, Vector *b, double k);

#endif