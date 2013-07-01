/* Dan Nelson
 * Graphics Package
 * ray_module.c
 * Element and module functions for ray tracing
 */


#include "cb_graphics.h"


// ###################
// ### RAY ELEMENT ###
// ###################

/*
 * Allocates an empty RayElement
 * @return: a new ray element
 */
RayElement *RayElement_create() {
	RayElement *e = malloc(sizeof(RayElement));
	e->type = ObjNone;
	e->next = NULL;
	return e;
}


/*
 * Allocate an RayElement and store a duplicate of the data pointed
 * to by obj in the RayElement. RayModules do not get duplicated. The
 * function needs to handle each type of object separately in a
 * case statement.
 * @type: a type of ray object
 * @obj: a ray object
 * @return: the ray element
 *
 * This function is modified from Element_init in module.c
 */
RayElement *RayElement_init(RayObjType type, void *obj) {
	RayElement *e = RayElement_create();
	e->type = type;
	switch (type) {
		case RayObjPlane:
			memcpy(&(e->obj.plane), obj, sizeof(Plane));
			break;
		case RayObjSphere:
			memcpy(&(e->obj.sphere), obj, sizeof(Sphere));
			break;
		default:
			break;
  }
  return e;
}


/*
 * Frees the ray element
 * @e: a ray element to free
 * @return: void
 */
void RayElement_delete(RayElement *e) {
	free(e);
}


/*
 * Returns the diffuse color a ray object
 * @e: a ray element
 * @return: the diffuse color
 */
Color RayElement_getDiffuseColor(RayElement *e) {
	Color c = {{0.0,0.0,0.0}};
	switch (e->type) {
		case RayObjPlane:
			return e->obj.plane.diffuse;
		case RayObjSphere:
			return e->obj.sphere.diffuse;
		default:
			return c;
	}
}


/*
 * Returns the specular color a ray object
 * @e: a ray element
 * @return: the specular color
 */
Color RayElement_getSpecularColor(RayElement *e) {
	Color c = {{0.0,0.0,0.0}};
	switch (e->type) {
		case RayObjPlane:
			return e->obj.plane.specular;
		case RayObjSphere:
			return e->obj.sphere.specular;
		default:
			return c;
	}
}


/*
 * Returns the refraction color a ray object
 * @e: a ray element
 * @return: the refraction color
 */
Color RayElement_getRefractionColor(RayElement *e) {
	Color c = {{0.0,0.0,0.0}};
	switch (e->type) {
		case RayObjPlane:
			return e->obj.plane.refraction;
		case RayObjSphere:
			return e->obj.sphere.refraction;
		default:
			return c;
	}
}


/*
 * Returns the index of refraction of a ray object
 * @e: a ray element
 * @return: the index of refraction
 */
double RayElement_getIndexOfRefraction(RayElement *e) {
	double rIndex = 0.0;
	switch (e->type) {
		case RayObjPlane:
			return e->obj.plane.rIndex;
			break;
		case RayObjSphere:
			return e->obj.sphere.rIndex;
		default:
			return rIndex;
	}
}


/*
 * Returns whether or not a ray object is refractive
 * @e: a ray element
 * @return: 1 if refractive, 0 if not
 */
int RayElement_isRefractive(RayElement *e) {
	int bool = 0;
	switch (e->type) {
		case RayObjPlane:
			return e->obj.plane.isRefractive;
			break;
		case RayObjSphere:
			return e->obj.sphere.isRefractive;
		default:
			return bool;
	}
}


/*
 * Returns whether or not a ray object is reflective
 * @e: a ray element
 * @return: 1 if reflective, 0 if not
 */
int RayElement_isReflective(RayElement *e) {
	int bool = 0;
	switch (e->type) {
		case RayObjPlane:
			return e->obj.plane.isReflective;
			break;
		case RayObjSphere:
			return e->obj.sphere.isReflective;
			break;
		default:
			return bool;
	}
}



// ##################
// ### RAY MODULE ###
// ##################

/*
 * Allocate an empty ray module.
 * @return: a new ray module
 *
 * This function is modified from Module_create in module.c
 */
RayModule *RayModule_create() {
	RayModule *rmd = malloc(sizeof(RayModule));
	rmd->head = NULL;
	rmd->tail = NULL;
	return rmd;
}


/*
 * Clear the ray module’s list of ray elements, freeing memory as
 * appropriate
 * @rmd: a ray module to free
 * @return: void
 *
 * This function is modified from Module_clear in module.c
 */
void RayModule_clear(RayModule *rmd) {
	RayElement *p,*q;
	p = rmd->head;
	while (p) {
		q = p;
		p = p->next;
		RayElement_delete(q);
	}
}


/*
 * Free all of the memory associated with a ray module, including
 * the memory pointed to by rmd.
 * @rmd: a ray module
 * @return: void
 *
 * This function is modified from Module_delete in module.c
 */
void RayModule_delete(RayModule *rmd) {
	RayModule_clear(rmd);
	free(rmd);
}


/*
 * Generic insert of an ray object element into the ray module at
 * the tail of the list.
 * @rmd: a ray module
 * @e: a ray element
 * @return: void
 *
 * This function is modified from Module_insert in module.c
 */
void RayModule_insert(RayModule *rmd, RayElement *e) {
	if (rmd->head == NULL) {
		rmd->head = e;
		rmd->tail = e;
	}
	else {
		rmd->tail->next = e;
		rmd->tail = e;
	}
}


/*
 * Adds p to the tail of the ray module's list
 * @rmd: a ray module
 * @s: a ray object plane
 * @return: void
 */
void RayModule_plane(RayModule *rmd, Plane *p) {
	RayElement *e = RayElement_init(RayObjPlane, p);
	RayModule_insert(rmd, e);
}


/*
 * Adds p to the tail of the ray module's list
 * @rmd: a ray module
 * @s: a ray object sphere
 * @return: void
 */
void RayModule_sphere(RayModule *rmd, Sphere *s) {
	RayElement *e = RayElement_init(RayObjSphere, s);
	RayModule_insert(rmd, e);
}





