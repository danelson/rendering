/* Dan Nelson
 * CS451 Project 4: Ray Tracing
 * May 10, 2012
 * cb_ray_module.h
 * Ray element information
 */


#ifndef CB_RAY_ELEMENT_H
#define CB_RAY_ELEMENT_H


typedef enum {
  RayObjSphere,
  RayObjPlane,
} RayObjType;

// Ray element union
typedef union {
  Sphere sphere;
  Plane plane;
} RayObject;

// Ray element structure
typedef struct {
  RayObjType type;
  RayObject obj;
  void *next;
} RayElement;

// Ray module structure
typedef struct {
  RayElement *head;
  RayElement *tail;
} RayModule;


// ###############
// ### Element ###
// ###############

RayElement *RayElement_create(void);
RayElement *RayElement_init(RayObjType type, void *obj);
void RayElement_delete(RayElement *e);
Color RayElement_getDiffuseColor(RayElement *e);
Color RayElement_getSpecularColor(RayElement *e);
Color RayElement_getRefractionColor(RayElement *e);
double RayElement_getIndexOfRefraction(RayElement *e);
int RayElement_isRefractive(RayElement *e);
int RayElement_isReflective(RayElement *e);


// ##############
// ### Module ###
// ##############

RayModule *RayModule_create(void);
void RayModule_clear(RayModule *rmd);
void RayModule_delete(RayModule *rmd);
void RayModule_insert(RayModule *rmd, RayElement *e);
void RayModule_plane(RayModule *rmd, Plane *p);
void RayModule_sphere(RayModule *rmd, Sphere *s);



#endif


