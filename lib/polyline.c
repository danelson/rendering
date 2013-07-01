/* Dan Nelson
 * Graphics Package
 * polyline.c
 */


#include "cb_graphics.h"



//////////////////////////////////
///////// POLYLINE FUNCTIONS /////
//////////////////////////////////

/* returns an allocated Polyline pointer initialized so that numVertex is 0 and vertex is NULL. */
Polyline *Polyline_create(void) {
  Polyline *p = malloc(sizeof(Polyline));
  p->numVertex = 0;
  p->vertex = NULL;
  return p;
}

/* returns an allocated Polyline pointer with the vertex list initialized to the points in vlist */
Polyline *Polyline_init(int numV, Point *vlist) {
  int i;
  
  Polyline *p = (Polyline *)malloc(sizeof(Polyline));
  printf("polyline initting... \n");
  if (p != NULL) {
    printf("sucessfully allocated polyline pointer \n");
    p->numVertex = numV;
    p->vertex = (Point *)malloc(sizeof(Point)*numV); 
    if (p->vertex != NULL) {
      printf("sucessfully allocated vlist pointer \n");
      for (i = 0; i < numV; i++) {
        p->vertex[i] = vlist[i];
      }
      return p;
    }    
    else {
      printf("failed to init vlist pointer.. X_X \n");
      return NULL;
    }
  }
  else {
    printf("failed to init polyline pointer.. X_X \n");
    return NULL;
  }
}

/* frees the data and the polyline pointer */
void Polyline_clear(Polyline *p) {
  p->numVertex = 0;
  if (p->vertex != NULL) {
    free(p->vertex);
  }
  p->vertex = NULL;
}

/* initializes the existing Polyline to an empty Polyline */
void Polyline_setNULL(Polyline *p) {
  p->numVertex = 0;
  p->vertex = NULL;
}

/* initializes the vertex list to the points in vlist */
void Polyline_set(Polyline *p, int numV, Point *vlist) {
  int i;
  printf("start setting polyline...\n");
  if (p == NULL) printf("polyline pointer is NULL! \n");
  if (p->vertex != NULL) {
    printf("freeing p->vertex \n");
    free(p->vertex);
    printf("finished freeing p->vertex \n");
  }
  
  p->numVertex = numV;
  p->vertex = malloc(sizeof(Point)*numV); 
  
  for (i = 0; i < numV; i++) {
    p->vertex[i] = vlist[i];
  }
}

/* Allocates space and copies the vertex data from one polygon to the others.*/
void Polyline_copy(Polyline *to, Polyline *from) {
  int i, numV;
  Polyline_clear(to);
  numV = to->numVertex = from->numVertex;
  to->vertex = malloc(sizeof(Point)*numV);
  
  for (i = 0; i < numV; i++) {
    to->vertex[i] = from->vertex[i];
  }
}

/* frees the internal data for a polyline */
void Polyline_free(Polyline *p){
 free(p->vertex);
 p->vertex = NULL;
 free(p);
 p = NULL;
}

/* normalize each vertex by the homogeneous coordinate */
void Polyline_normalize( Polyline *p ) {
  int i, numV;
  numV = p->numVertex;
  
  for (i = 0; i < numV; i++) {
    p->vertex[i].val[0] = p->vertex[i].val[0]/p->vertex[i].val[3];
    p->vertex[i].val[1] = p->vertex[i].val[1]/p->vertex[i].val[3];
    p->vertex[i].val[3] = 1.0;
  }
}

/* prints polyline data to the stream designated by the FILE pointer. */
void Polyline_print(Polyline *p, FILE *fp) {
  int i;
  int numV = p->numVertex;
  for (i=0; i<numV; i++) {
    fprintf(fp,"Vertex #: %d x: %f y: %f z: %f h: %f \n",i,
    		p->vertex[i].val[0],p->vertex[i].val[1],p->vertex[i].val[2],p->vertex[i].val[3]);
  }
}

/*draw the outline of the polyline using color color. If the zBuffer flag is set, should take into
 * account the z-buffer values when drawing lines.*/
void Polyline_drawFrame(Polyline *p, Image *src, Color color) {
  int numV = p->numVertex;
  Point *vlist = p->vertex;
  Point point1, point2;
  Line line;
  int i;

  
  for (i = 0; i < numV - 1; i++) {
    printf("drawing at vertex: %d \n",i);
    // put start and end point data into the point pointers
    Point_set2D(&point1, vlist[i].val[0], vlist[i].val[1]);
    Point_set2D(&point2, vlist[i+1].val[0], vlist[i+1].val[1]);
    // put line data into the line pointer
    Line_set(&line, point1, point2);
    // draw the line
    Line_draw(&line, src, color);
  }
}


void Polyline_zBuffer(Polyline *p, int flag) {
  p->zBuffer = flag;
}
