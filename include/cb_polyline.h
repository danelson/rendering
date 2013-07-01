// Dan Nelson and Hieu Phan
// Project 3
// September 29, 2011
// cb_polyline.h

#ifndef CB_POLYLINE_H

#define CB_POLYLINE_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// Polyline
typedef struct {
  int zBuffer;
  int numVertex; // number of vertices
  Point *vertex; // vertex info
} Polyline;


/*****************************************
 *			POLYLINE FUNCTIONS		     * 
 *****************************************/
Polyline *Polyline_create(void);
Polyline *Polyline_init(int numV, Point *vlist);
void Polyline_clear(Polyline *p);
void Polyline_setNULL(Polyline *p);
void Polyline_set(Polyline *p, int numV, Point *vlist);
void Polyline_zBuffer(Polyline *p, int flag);
void Polyline_copy(Polyline *to, Polyline *from);
void Polyline_free(Polyline *p);
void Polyline_print(Polyline *p, FILE *fp);
void Polyline_drawFrame(Polyline *p, Image *src, Color color);
void Polyline_normalize( Polyline *p );



#endif