/* Dan Nelson
 * CS451 Project 4: Ray Tracing
 * May 10, 2012
 * plane.h
 * Plane prototypes
 */
 
 
#ifndef CB_PLANE_H
#define CB_PLANE_H

 
// Plane structure
typedef struct {
	double p[4]; 		// plane = A*x + B*y + C*z + D = 0 where A^2+B^2+C^2 = 1
	Color diffuse;
	Color specular;
	Color refraction;
	int isReflective; 	// 1 = true, 0 = false
	int isRefractive;	// 1 = true, 0 = false
	double rIndex; 			// index of refraction
} Plane;


// #######################
// ### Plane Functions ###
// #######################

void Plane_set(Plane *pl, double A, double B, double C, double D);
void Plane_setColor(Plane *p, Color diffuse, Color specular, int isReflective);

#endif



