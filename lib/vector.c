/* Dan Nelson
 * Graphics Package
 * vector.c
 * Functions for vectors
 */

 
#include "cb_graphics.h"



/******************
VECTOR FUNCTIONS
******************/

/* Set the vector to (x,y,z,0.0) */
void Vector_set(Vector *v, double x, double y, double z) {
  v->v[0] = x;
  v->v[1] = y;
  v->v[2] = z;
  v->v[3] = 0.0;
}


/* Print out the Vector in a beautiful form */
void Vector_print(Vector *v, FILE *fp) {
  fprintf(fp,"x: %lf, y: %lf, z: %lf \n",v->v[0],v->v[1],v->v[2]);
}


/* Copy the src Vector into the dest Vector */
void Vector_copy(Vector *dest, Vector *src) {
  int i;
  
  for (i = 0; i < 4; i++) {
    dest->v[i] = src->v[i];
  }  
}


/* Returns the Euclidean length of the Vector */
double Vector_length(Vector *v) {
  return sqrt(v->v[0]*v->v[0] + v->v[1]*v->v[1] + v->v[2]*v->v[2]);
}


/* Returns the vector squared */
double Vector_lenSquared(Vector *v) {
  return v->v[0]*v->v[0] + v->v[1]*v->v[1] + v->v[2]*v->v[2];
}


/* Normalize the Vector to unit length */
void Vector_normalize(Vector *v) {
  double l;
  l = Vector_length(v);
  if (l != 0) {
    v->v[0] = v->v[0] / l;
    v->v[1] = v->v[1] / l;
    v->v[2] = v->v[2] / l;
    v->v[3] = 0.0;
  }
}


/* Returns the scalar product of a and b */
double Vector_dot(Vector *a, Vector *b) {
  return a->v[0]*b->v[0] + a->v[1]*b->v[1] + a->v[2]*b->v[2];
}


/* Calculates the cross product of a and b and put the vector in c */
void Vector_cross(Vector *a, Vector *b, Vector *c) {
  c->v[0] = a->v[1]*b->v[2] - a->v[2]*b->v[1];
  c->v[1] = a->v[2]*b->v[0] - a->v[0]*b->v[2];
  c->v[2] = a->v[0]*b->v[1] - a->v[1]*b->v[0];
}


/* Calculate the sum of two vectors a and b and put it in c */
void Vector_sum(Vector *a, Vector *b, Vector *c) {
  c->v[0] = a->v[0] + b->v[0];
  c->v[1] = a->v[1] + b->v[1];
  c->v[2] = a->v[2] + b->v[2];
}


/* Multiply a vector a with a scalar and put the result in b */
void Vector_scale(Vector *a, Vector *b, double k) {
  b->v[0] = k * a->v[0];
  b->v[1] = k * a->v[1];
  b->v[2] = k * a->v[2];
}

