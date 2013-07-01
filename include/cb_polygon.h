// Dan Nelson and Hieu Phan
// Project 4
// October 3, 2011
// cb_polygon.h

#ifndef CB_POLYGON_H

#define CB_POLYGON_H


/*****************************************
 *				STRUCTURES				 * 
 *****************************************/

// Polygon
typedef struct {
  int zBuffer; // whether to use the z-buffer; should default to true (1)
  int oneSided; // whether to consider the polygon one-sided (1) or two-sided (0) for shading
  int nVertex; // number of vertices
  Point *vertex; // vertex info
  Vector *normal; // surface normal info
  Color *color;
  Point *wVertex; //3D vertex info, used for Phong shading
  Vector *wNormal; //3D normal info, used for Phong shading
  TextureCoord *texCoord;
  Texture *tex;
} Polygon;


/*****************************************
 *			Polygon FUNCTIONS		     * 
 *****************************************/
Polygon *Polygon_create(void);
Polygon *Polygon_init(int numV, Point *vlist);
void Polygon_clear(Polygon *p);

void Polygon_copy(Polygon *to, Polygon *from);
void Polygon_free(Polygon *p);
void Polygon_print(Polygon *p, FILE *fp);

void Polygon_setNULL(Polygon *p);
void Polygon_set(Polygon *p, int numV, Point *vlist);
void Polygon_setColors(Polygon *p, int numV, Color *clist);
void Polygon_setNormals(Polygon *p, int numV, Vector *nlist);
void Polygon_calculateNormals(Polygon *p, int numV);
void Polygon_setAll(Polygon *p, int numV, Point *vlist, 
                    Color *clist, Vector *nlist, int zBuffer, int oneSided,
                    TextureCoord *texList);
void Polygon_setWorld(Polygon *p, int numV);

void Polygon_zBuffer(Polygon *p, int flag);
void Polygon_drawFrame(Polygon *p, Image *src, Color color);
void Polygon_drawFill( Polygon *p, Image *src, DrawState *ds, Lighting* light );
void Polygon_drawFillB(Polygon *p, Image *src, Color color);
void Polygon_normalize(Polygon *p);
void Polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds);
void Polygon_drawShade( Polygon *p, Image *src, DrawState* ds, Lighting *light);
void Polygon_setTexture(Polygon *p, int numV, TextureCoord *texList);


#endif