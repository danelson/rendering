/* Dan Nelson
 * CS451 Project 4: Ray Tracing
 * May 10, 2012
 * cb_ray_object.h
 * Prototypes for ray objects
 */


#ifndef CB_RAY_SPHERE_H
#define CB_RAY_SPHERE_H


// Plane structure
typedef struct {
	double p[4];
	Color diffuse;
	Color specular;
	Color refraction;
	int isReflective; 		// 1 = true, 0 = false
	int isRefractive; 		// 1 = true, 0 = false
	double rIndex; 			// index of refraction
} Plane;

// Sphere structure
typedef struct {
	Point c; 				// center
	double r; 				// radius
	Color diffuse;
	Color specular;
	Color refraction;
	int isReflective; 		// 1 = true, 0 = false
	int isRefractive; 		// 1 = true, 0 = false
	double rIndex; 			// index of refraction
} Sphere;


// #############
// ### Plane ###
// #############

void Plane_set(Plane *pl, double A, double B, double C, double D);
void Plane_setColor(Plane *p, Color diffuse, Color specular, int isReflective);


// ##############
// ### Sphere ###
// ##############

void Sphere_set(Sphere *s, Point c, float r);
void Sphere_setColor(Sphere *s, Color diffuse, Color specular,
								Color refraction, int isRefractive,
													int isReflective);
void Sphere_setRIndex(Sphere *s, float rIndex);

#endif






