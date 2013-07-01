// Dan Nelson and Hieu Phan
// Project 6
// October 24, 2011
// cb_module.h

#ifndef CB_MODULE_H

#define CB_MODULE_H


typedef enum {
  ObjNone,
  ObjLine,
  ObjPoint,
  ObjPolyline,
  ObjPolygon,
  ObjCircle,
  ObjSphere,
  ObjPlane,
  ObjIdentity,
  ObjMatrix,
  ObjColor,
  ObjBodyColor,
  ObjSurfaceColor,
  ObjSurfaceCoeff,
  ObjLight,
  ObjTexture,
  ObjModule
} ObjectType;

// Element union
typedef union {
  Point point;
  Line line;
  Polyline polyline;
  Polygon polygon;
  Circle circle;
  Sphere sphere;
  Plane plane;
  Matrix matrix;
  Color color;
  float coeff;
  Texture *tex;
  void *module;
} Object;

// Element structure
typedef struct {
  ObjectType type;
  Object obj;
  void *next;
} Element;

// Module structure
typedef struct {
  Element *head;
  Element *tail;
} Module;



/*******************
*      Element     *
********************/

Element *Element_create(void);
Element *Element_init(ObjectType type, void *obj);
void Element_delete(Element *e);


/*******************
*      Module      *
********************/

Module *Module_create(void);
void Module_clear(Module *md);
void Module_delete(Module *md);
void Module_insert(Module *md, Element *e);
void Module_module(Module *md, Module *sub);
void Module_point(Module *md, Point *p);
void Module_line(Module *md, Line *l);
void Module_polyline(Module *md, Polyline *p);
void Module_polygon(Module *md, Polygon *p);
void Module_circle(Module *md, Circle *c);
void Module_sphere(Module *md, Sphere *s);
void Module_plane(Module *md, Plane *p);
void Module_identity(Module *md);
void Module_translate2D(Module *md, double tx, double ty);
void Module_scale2D(Module *md, double sx, double sy);
void Module_rotateZ(Module *md, double cth, double sth);
void Module_shear2D(Module *md, double shx, double shy);
void Module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);
void Module_translate(Module *md, double tx, double ty, double tz);
void Module_scale(Module *md, double sx, double sy, double sz);
void Module_rotateX(Module *md, double cth, double sth);
void Module_rotateY(Module *md, double cth, double sth);
void Module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);
void Module_cube(Module *md, int solid, Texture *tex[6]);
void Module_color(Module *md, Color *c);
void Module_bodyColor(Module *md, Color *c);
void Module_surfaceColor(Module *md, Color *c);
void Module_surfaceCoeff(Module *md, float coeff);
void Module_texture(Module *md, Texture *tex);





#endif


