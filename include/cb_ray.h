/* Dan Nelson
 * CS451 Project 4: Ray Tracing
 * May 10, 2012
 * cb_ray.h
 * Prototypes for ray.c
 */


#ifndef CB_RAY_H
#define CB_RAY_H


extern Lighting *global_light;
extern RayModule *global_rayModule;


// Ray structure
typedef struct {
  Point p; 			// starting point of ray
  Vector v; 		// ray vector
} Ray;

typedef struct {
  Point p; 			// intersection point
  Vector nor; 		// normal vector at intersect point
  RayElement *e;
} Intersection;


// #####################
// ### Ray functions ###
// #####################

void Ray_set(Ray *r, Point p, Vector v);
void Ray_print(Ray *r, FILE *fp);
void Ray_copy(Ray *dest, Ray *src);
void Ray_reflect(Ray *ray1, Intersection *inter, Ray *ray2);
void Ray_refract(Ray *ray1, Intersection *inter, Ray *ray2);

Intersection* Ray_sphereIntersect(Ray *ray, Sphere *sphere);
Intersection* Ray_planeIntersect(Ray *ray, Plane *plane, int singleSide);
Intersection* Ray_intersect(Ray *ray, RayElement *e);
void Intersection_set(Intersection *inter, Point p, Vector v);
void Intersection_copy(Intersection *to, Intersection *from);
Intersection* trace(Ray *ray, int depth, Point eye);

Color addColors(Color pointC, Color reflectV, Color coeffReflect,
									Color refractV,	Color coeffRefract);
Color Ray_send(Intersection *inter, Point vrp);
Color Ray_trace(Ray *ray, int depth, Point eye, Point vrp);


#endif