/* Dan Nelson
 * Graphics Package
 * matrix.c
 */


#include <string.h>
#include "cb_graphics.h"



/******************
 MATRIX FUNCTIONS
******************/

/* Print out the matrix in a pretty 4x4 arrangement with a blank line below */
void Matrix_print(Matrix *m, FILE *fp) {
  fprintf(fp, "[ %lf %lf %lf %lf \n",m->m[0],m->m[1],m->m[2],m->m[3]);
  fprintf(fp, "[ %lf %lf %lf %lf \n",m->m[4],m->m[5],m->m[6],m->m[7]);
  fprintf(fp, "[ %lf %lf %lf %lf \n",m->m[8],m->m[9],m->m[10],m->m[11]);
  fprintf(fp, "[ %lf %lf %lf %lf ]\n \n",m->m[12],m->m[13],m->m[14],m->m[15]);
}

/* Set the matrix to all zeros */
void Matrix_clear(Matrix *m) {
  int i;
  for (i=0;i<16;i++) {
    m->m[i] = 0;
  }
}


/* Set the matrix to the identity matrix */
void Matrix_identity(Matrix *m) {
  int i;
  for (i=1;i<15;i++) {
    m->m[i] = 0.0;
  }
  m->m[0] = m->m[5] = m->m[10] = m->m[15] = 1.0;
}


/* Return the element of the matrix at row r, column c. */
double Matrix_get(Matrix *m, int r, int c) {
  return m->m[r*4+c];
}

/* Set the element of the matrix at row r, column c to v */
void Matrix_set(Matrix *m, int r, int c, double v) {
  m->m[r*4+c] = v;
}


/* Copy the src matrix into the dest matrix */
void Matrix_copy(Matrix *dest, Matrix *src) {
  int i;
  for (i=0;i<16;i++) {
    dest->m[i] = src->m[i];
  }
}


/* Used for the transpose function */
#define swap_(a, b) { double tmp;  tmp = a; a = b; b = tmp; }

/* Transpose the matrix m in place */
void Matrix_transpose(Matrix *m) {
  swap_(m->m[1],m->m[4]);
  swap_(m->m[2],m->m[8]);
  swap_(m->m[3],m->m[12]); 
  swap_(m->m[6],m->m[9]);
  swap_(m->m[7],m->m[13]);
  swap_(m->m[11],m->m[14]);
}

/* Multiply left and right and put the result in m 
 * [M] = [left] * [right]                                    */
void Matrix_multiply(Matrix *left, Matrix *right, Matrix *m) {
  
  // Make sure that the function is written so that the result matrix 
  // can also be the left or right matrix.  
  Matrix temp;
  
  // FOR THE SAKE OF SPEED, THIS LOOKS LIKE A NIGHTMARE
  temp.m[0] = left->m[0]*right->m[0]+left->m[1]*right->m[4]+left->m[2]*right->m[8]+left->m[3]*right->m[12];
  temp.m[1] = left->m[0]*right->m[1]+left->m[1]*right->m[5]+left->m[2]*right->m[9]+left->m[3]*right->m[13];
  temp.m[2] = left->m[0]*right->m[2]+left->m[1]*right->m[6]+left->m[2]*right->m[10]+left->m[3]*right->m[14];
  temp.m[3] = left->m[0]*right->m[3]+left->m[1]*right->m[7]+left->m[2]*right->m[11]+left->m[3]*right->m[15];
  
  temp.m[4] = left->m[4]*right->m[0]+left->m[5]*right->m[4]+left->m[6]*right->m[8]+left->m[7]*right->m[12];
  temp.m[5] = left->m[4]*right->m[1]+left->m[5]*right->m[5]+left->m[6]*right->m[9]+left->m[7]*right->m[13];
  temp.m[6] = left->m[4]*right->m[2]+left->m[5]*right->m[6]+left->m[6]*right->m[10]+left->m[7]*right->m[14];
  temp.m[7] = left->m[4]*right->m[3]+left->m[5]*right->m[7]+left->m[6]*right->m[11]+left->m[7]*right->m[15];
  
  temp.m[8] = left->m[8]*right->m[0]+left->m[9]*right->m[4]+left->m[10]*right->m[8]+left->m[11]*right->m[12];
  temp.m[9] = left->m[8]*right->m[1]+left->m[9]*right->m[5]+left->m[10]*right->m[9]+left->m[11]*right->m[13];
  temp.m[10] = left->m[8]*right->m[2]+left->m[9]*right->m[6]+left->m[10]*right->m[10]+left->m[11]*right->m[14];
  temp.m[11] = left->m[8]*right->m[3]+left->m[9]*right->m[7]+left->m[10]*right->m[11]+left->m[11]*right->m[15];
  
  temp.m[12] = left->m[12]*right->m[0]+left->m[13]*right->m[4]+left->m[14]*right->m[8]+left->m[15]*right->m[12];
  temp.m[13] = left->m[12]*right->m[1]+left->m[13]*right->m[5]+left->m[14]*right->m[9]+left->m[15]*right->m[13];
  temp.m[14] = left->m[12]*right->m[2]+left->m[13]*right->m[6]+left->m[14]*right->m[10]+left->m[15]*right->m[14];
  temp.m[15] = left->m[12]*right->m[3]+left->m[13]*right->m[7]+left->m[14]*right->m[11]+left->m[15]*right->m[15];
  
  //printf("Debugging matrix multiply: \n");
  //Matrix_print(temp,stdout);
  //Matrix_print(left,stdout);
  //Matrix_print(right,stdout);

  memcpy(m,&temp,sizeof(Matrix));
}


/* Transform the point p by the matrix m and put the result in q. 
 * For this function, p and q need to be different variables. */
void Matrix_xformPoint(Matrix *m, Point *p, Point *q) {
  // do matrix multiplication [m] * [p]
  q->val[0] = m->m[0]*p->val[0] + m->m[1]*p->val[1] + m->m[2]*p->val[2] + m->m[3]*p->val[3];
  q->val[1] = m->m[4]*p->val[0] + m->m[5]*p->val[1] + m->m[6]*p->val[2] + m->m[7]*p->val[3];
  q->val[2] = m->m[8]*p->val[0] + m->m[9]*p->val[1] + m->m[10]*p->val[2] + m->m[11]*p->val[3];
  q->val[3] = m->m[12]*p->val[0] + m->m[13]*p->val[1] + m->m[14]*p->val[2] + m->m[15]*p->val[3];
}


/* Transform the vector p by the matrix m and put the result in q. 
 * For this function, p and q need to be different variables. */
void Matrix_xformVector(Matrix *m, Vector *p, Vector *q) {
  // do matrix multiplication [m] * [p]
  q->v[0] = m->m[0]*p->v[0] + m->m[1]*p->v[1] + m->m[2]*p->v[2] + m->m[3]*p->v[3];
  q->v[1] = m->m[4]*p->v[0] + m->m[5]*p->v[1] + m->m[6]*p->v[2] + m->m[7]*p->v[3];
  q->v[2] = m->m[8]*p->v[0] + m->m[9]*p->v[1] + m->m[10]*p->v[2] + m->m[11]*p->v[3];
  q->v[3] = m->m[12]*p->v[0] + m->m[13]*p->v[1] + m->m[14]*p->v[2] + m->m[15]*p->v[3];
}


/* Transform the points and surface normals (if they exist) 
 * in the Polygon p by the matrix m.                      */
void Matrix_xformPolygon(Matrix *m, Polygon *p) {
  int i;
  
  // the transform function only takes in 2 different variables
  // so we need a temp variable here
  Point temp;
  Vector tempV;
  
  for (i = 0; i < p->nVertex; i++) {
    memcpy(&temp, &(p->vertex[i]), sizeof(Point));
    Matrix_xformPoint(m,&temp,&(p->vertex[i]));
    memcpy(&tempV, &(p->normal[i]), sizeof(Point));
    Matrix_xformVector(m,&tempV,&(p->normal[i]));
  }
}

/* Transform the points in the Polyline p by the matrix m */
void Matrix_xformPolyline(Matrix *m, Polyline *p) {
  int i;
  
  // the transform function only takes in 2 different variables
  // so we need a temp variable here
  Point temp;
  //temp = malloc(sizeof(Point));
  
  for (i = 0; i < p->numVertex; i++) {
    // *** WE COULD SPEED UP THE PROGRAM HERE BY WRITING OUT THE WHOLE FUNTION ***
    Matrix_xformPoint(m,&(p->vertex[i]),&temp);
    p->vertex[i] = temp;
  }
  
  //free(temp);
}


/* Transform the points in line by the matrix m. */
void Matrix_xformLine(Matrix *m, Line *line) {
  Point temp;
  //temp = malloc(sizeof(Point));
  Matrix_xformPoint(m,&(line->a),&temp);
  line->a = temp;
  Matrix_xformPoint(m,&(line->b),&temp);
  line->b = temp;
}

/* Premultiply the matrix by a scale matrix parameterized by sx and sy. */
void Matrix_scale2D(Matrix *m, double sx, double sy) {
  // Construct the scaling matrix
  Matrix sMatrix;
  sMatrix.m[0] = sx;
  sMatrix.m[5] = sy;
  sMatrix.m[10] = sMatrix.m[15] = 1.0;
  sMatrix.m[1] = sMatrix.m[2] = sMatrix.m[3] = sMatrix.m[4]
    = sMatrix.m[6] = sMatrix.m[7] = sMatrix.m[8] = sMatrix.m[9]
    = sMatrix.m[11] = sMatrix.m[12] = sMatrix.m[13] = sMatrix.m[14] = 0.0;
  
  // Premultiply
  Matrix_multiply(&sMatrix,m,m);
}

/* Premultiply the matrix by a Z-axis rotation matrix parameterized by cos(θ) and sin(θ), 
 * where θ is the angle of rotation about the Z-axis.                                    */
void Matrix_rotateZ(Matrix *m, double cth, double sth) {
  // Construct the rotation matrix
  Matrix rMatrix;

  rMatrix.m[0] = cth;
  rMatrix.m[1] = -sth;
  rMatrix.m[4] = sth;
  rMatrix.m[5] = cth;
  rMatrix.m[10] = rMatrix.m[15] = 1.0;
  rMatrix.m[2] = rMatrix.m[3] = rMatrix.m[6] = rMatrix.m[7]
    = rMatrix.m[8] = rMatrix.m[9] = rMatrix.m[11] 
    = rMatrix.m[12] = rMatrix.m[13] = rMatrix.m[14] = 0.0;
  
  // Premultiply
  Matrix_multiply(&rMatrix,m,m);
}


/* Premultiply the matrix by a 2D translation matrix parameterized by tx and ty. */
void Matrix_translate2D(Matrix *m, double tx, double ty) {
  // Construct the translation matrix
  Matrix tMatrix;

  tMatrix.m[0] = tMatrix.m[5] = tMatrix.m[10] = tMatrix.m[15] = 1.0;
  tMatrix.m[3] = tx;
  tMatrix.m[7] = ty;
  tMatrix.m[1] = tMatrix.m[2] = tMatrix.m[4] = tMatrix.m[6]
    = tMatrix.m[8] = tMatrix.m[9] = tMatrix.m[11] = tMatrix.m[12]
    = tMatrix.m[13] = tMatrix.m[14] = 0.0;
    
  // Premultiply
  Matrix_multiply(&tMatrix,m,m);
}


/* Premultiply the matrix by a 2D shear matrix parameterized by shx and shy.*/
void Matrix_shear2D(Matrix *m, double shx, double shy){
  // Construct the shear matrix
  Matrix shMatrix;

  shMatrix.m[0] = shMatrix.m[5] = shMatrix.m[10] = shMatrix.m[15] = 1.0;
  shMatrix.m[1] = shx;
  shMatrix.m[4] = shy;
  shMatrix.m[2] = shMatrix.m[3] = shMatrix.m[6] = shMatrix.m[7]
    = shMatrix.m[8] = shMatrix.m[9] = shMatrix.m[11] = shMatrix.m[12]
    = shMatrix.m[13] = shMatrix.m[14] = 0.0;
    
  // Premultiply
  Matrix_multiply(&shMatrix,m,m);
}

#undef swap_;


/**************
 *	   3D     * 
 **************/
void Matrix_translate(Matrix *m, double tx, double ty, double tz) {
  // Construct the translation matrix
  Matrix tMatrix;

  tMatrix.m[0] = tMatrix.m[5] = tMatrix.m[10] = tMatrix.m[15] = 1.0;
  tMatrix.m[3] = tx;
  tMatrix.m[7] = ty;
  tMatrix.m[11] = tz;
  tMatrix.m[1] = tMatrix.m[2] = tMatrix.m[4] = tMatrix.m[6]
    = tMatrix.m[8] = tMatrix.m[9] = tMatrix.m[12]
    = tMatrix.m[13] = tMatrix.m[14] = 0.0;
    
  // Premultiply
  Matrix_multiply(&tMatrix,m,m);
}

void Matrix_scale(Matrix *m, double sx, double sy, double sz) {
  // Construct the scaling matrix
  Matrix sMatrix;
  
  sMatrix.m[0] = sx;
  sMatrix.m[5] = sy;
  sMatrix.m[10] = sz;
  sMatrix.m[15] = 1.0;
  sMatrix.m[1] = sMatrix.m[2] = sMatrix.m[3] = sMatrix.m[4]
    = sMatrix.m[6] = sMatrix.m[7] = sMatrix.m[8] = sMatrix.m[9]
    = sMatrix.m[11] = sMatrix.m[12] = sMatrix.m[13] = sMatrix.m[14] = 0.0;
  
  // Premultiply
  Matrix_multiply(&sMatrix,m,m);
}

void Matrix_rotateX(Matrix *m, double cth, double sth) {
  // Construct the rotation matrix
  Matrix rMatrix;

  rMatrix.m[0] = rMatrix.m[15] = 1.0;
  rMatrix.m[5] = cth;
  rMatrix.m[6] = -sth;
  rMatrix.m[9] = sth;
  rMatrix.m[10] = cth;
  rMatrix.m[1] = rMatrix.m[2] = rMatrix.m[3] = rMatrix.m[4] = rMatrix.m[7]
    = rMatrix.m[8] = rMatrix.m[11] = rMatrix.m[12] 
    = rMatrix.m[13] = rMatrix.m[14] = 0.0;
  
  // Premultiply
  Matrix_multiply(&rMatrix,m,m);
}


void Matrix_rotateY(Matrix *m, double cth, double sth) {
  // Construct the rotation matrix
  Matrix rMatrix;

  rMatrix.m[0] = cth;
  rMatrix.m[2] = sth;
  rMatrix.m[8] = -sth;
  rMatrix.m[10] = cth;
  rMatrix.m[5] = rMatrix.m[15] = 1.0;
  rMatrix.m[1] = rMatrix.m[3] = rMatrix.m[4] = rMatrix.m[6]
    = rMatrix.m[7] = rMatrix.m[9] = rMatrix.m[11] 
    = rMatrix.m[12] = rMatrix.m[13] = rMatrix.m[14] = 0.0;
  
  // Premultiply
  Matrix_multiply(&rMatrix,m,m);
}

void Matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w) {
  // Construct the rotation matrix
  Matrix rMatrix;

  rMatrix.m[0] = u->v[0];
  rMatrix.m[1] = u->v[1];
  rMatrix.m[2] = u->v[2];
  rMatrix.m[4] = v->v[0];
  rMatrix.m[5] = v->v[1];
  rMatrix.m[6] = v->v[2];
  rMatrix.m[8] = w->v[0];
  rMatrix.m[9] = w->v[1];
  rMatrix.m[10] = w->v[2];
  rMatrix.m[3] = rMatrix.m[7] = rMatrix.m[11] = rMatrix.m[12]
    = rMatrix.m[13] = rMatrix.m[14] = 0.0;
  rMatrix.m[15] = 1.0;
    
  // Premultiply
  Matrix_multiply(&rMatrix,m,m);
}

void Matrix_shearZ(Matrix *m, double shx, double shy) {
  // Construct the rotation matrix
  Matrix shMatrix;

  shMatrix.m[0] = shMatrix.m[5] = shMatrix.m[10] = shMatrix.m[15] = 1.0;
  shMatrix.m[2] = shx;
  shMatrix.m[6] = shy;
  shMatrix.m[1] = shMatrix.m[3] = shMatrix.m[4] = shMatrix.m[7]
    = shMatrix.m[8] = shMatrix.m[9] = shMatrix.m[11] 
    = shMatrix.m[12] = shMatrix.m[13] = shMatrix.m[14] = 0.0;
    
  // Premultiply
  Matrix_multiply(&shMatrix,m,m);
}

void Matrix_perspective(Matrix *m, double d) {
  // Construct the rotation matrix
  Matrix pMatrix;

  pMatrix.m[0] = pMatrix.m[5] = pMatrix.m[10] = 1.0;
  pMatrix.m[1] = pMatrix.m[2] = pMatrix.m[3] = pMatrix.m[4]
  	= pMatrix.m[6] = pMatrix.m[7] = pMatrix.m[8]
  	= pMatrix.m[9] = pMatrix.m[11] = pMatrix.m[12]
  	= pMatrix.m[13] = pMatrix.m[15] = 0.0;
  pMatrix.m[14] = 1.0/d;
    
  // Premultiply
  Matrix_multiply(&pMatrix,m,m);
}






