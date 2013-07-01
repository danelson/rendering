/* Dan Nelson
 * Graphics Package
 * ray_object.c
 * Plane and sphere functions for ray tracing
 */


#include "cb_graphics.h"


// #######################
// ### Plane Functions ###
// #######################

/*
 * Sets a plane of the form Ax + By + Cz = D
 * @return: void
 */
void Plane_set(Plane *pl, double A, double B, double C, double D) {
	// we also normalize plane parameters here
	double factor = sqrt(A*A + B*B + C*C);
	
	pl->p[0] = A/factor;
	pl->p[1] = B/factor;
	pl->p[2] = C/factor;
	pl->p[3] = D/factor;
}


/*
 * Sets the color of a plane
 * @p: a plane
 * @diffuse: the diffuse color
 * @specular: the specular color
 * @refraction: the refraction color
 * @isRefractive: boolean value
 * @isReflective: boolean value
 * @return: void
 */
void Plane_setColor(Plane *p, Color diffuse, Color specular, int isReflective) {
	Color_copy(&(p->diffuse), &diffuse);
	Color_copy(&(p->specular), &specular);
	p->isReflective = isReflective;
}


// ########################
// ### Sphere functions ###
// ########################

/*
 * Sets the center and radius of a sphere
 * @c: center
 * @r: radius
 * @return: void
 */
void Sphere_set(Sphere *s, Point c, float r) {
	s->c = c;
	s->r = r;
}


/*
 * Sets the color of a sphere
 * @s: a sphere
 * @diffuse: the diffuse color
 * @specular: the specular color
 * @refraction: the refraction color
 * @isRefractive: boolean value
 * @isReflective: boolean value
 * @return: void
 */
void Sphere_setColor(Sphere *s, Color diffuse, Color specular, Color refraction,
											int isRefractive, int isReflective) {
	Color_copy(&(s->diffuse), &diffuse);
	Color_copy(&(s->specular), &specular);
	Color_copy(&(s->refraction), &refraction);
	
	s->isReflective = isReflective;
	s->isRefractive = isRefractive;
}


/*
 * Sets the center and radius of a sphere
 * @s: a sphere
 * @rIndex: the index of refraction
 * @return: void
 */
void Sphere_setRIndex(Sphere *s, float rIndex) {
	s->rIndex = rIndex;
}




