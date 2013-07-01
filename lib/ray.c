/* Dan Nelson
 * Graphics Package
 * ray.c
 * Implements a ray tracer
 */


#include "cb_graphics.h"


Lighting *global_light;
RayModule *global_rayModule;


// #####################
// ### Ray Functions ###
// #####################

/*
 * Assigns a starting point and vector for a ray
 * @r: the new ray
 * @p: the starting point of the ray
 * @v: the ray vector
 * @return: void
 */
void Ray_set(Ray *ray, Point p, Vector v) {
	Point_copy(&(ray->p), &p);
	Vector_normalize(&v);
	Vector_copy(&(ray->v), &v);
}


/* 
 * Prints out ray information
 * @r: a ray
 * @p: file to print to
 * @return: void
 */
void Ray_print(Ray *ray, FILE *fp) {
	fprintf(fp,"Ray center (%f, %f, %f),   "
				"Ray vector (%f, %f, %f)\n",
				ray->p.val[0], ray->p.val[1], ray->p.val[2],
				ray->v.v[0], ray->v.v[1], ray->v.v[2]);
}


/* 
 * Copies ray data from src to dest
 * @dest: a ray to copy to
 * @src: a ray to copy from
 * @return: void
 */
void Ray_copy(Ray *dest, Ray *src) {
	int i;

	// point copy
	for (i=0; i<4; i++) {
		dest->p.val[i] = src->p.val[i];
	}
	
	// vector copy
	for (i=0; i < 4; i++) {
		dest->v.v[i] = src->v.v[i];
	}
}


 /*
 * Calculates the refracted ray from a given ray and intersection.
 * The new ray is stored in ray2.
 * @ray1: an orignal ray
 * @intersection: the refraction intersections
 * @ray2: the refracted ray
 * @return: void
 */
void Ray_refract(Ray *ray1, Intersection *inter, Ray *ray2) {
	Point p, ray_p;
	Vector nor, v, refract;
	double n, a, b;
	
	p = inter->p;				// intersection point
	nor = inter->nor;			// intersection normal
	v = ray1->v;				// ray vector
	
	// index of refraction
	n = 1.0/RayElement_getIndexOfRefraction(inter->e);
	a = - (Vector_dot(&nor, &v));
	b = sqrt( 1 - n*n * (1 - a*a) );
 
	refract.v[0] = n * v.v[0] + (n * a - b) * nor.v[0];
	refract.v[1] = n * v.v[1] + (n * a - b) * nor.v[1];
	refract.v[2] = n * v.v[2] + (n * a - b) * nor.v[2];

	ray_p.val[0] = p.val[0] + nor.v[0] * 10e-6;
	ray_p.val[1] = p.val[1] + nor.v[1] * 10e-6;
	ray_p.val[2] = p.val[2] + nor.v[2] * 10e-6;

	Ray_set(ray2, ray_p, refract);
}


/*
 * Calculates the reflected ray from a given ray and intersection.
 * The new ray is stored in ray2.
 * @ray1: an orignal ray
 * @intersection: the reflection intersection
 * @ray2: the reflected ray
 * @return: void
 */
void Ray_reflect(Ray *ray1, Intersection *inter, Ray *ray2) {
	Point p, ray_p;
	Vector nor, v, temp;
	
	p = inter->p;
	nor = inter->nor;
	v = ray1->v;
    
	Vector_scale(&nor, &temp, 2.0* (-(Vector_dot(&nor, &v))));
	Vector_sum(&temp, &v, &temp);
  
	ray_p.val[0] = p.val[0] + nor.v[0] * 10e-6;
	ray_p.val[1] = p.val[1] + nor.v[1] * 10e-6;
	ray_p.val[2] = p.val[2] + nor.v[2] * 10e-6;

	Ray_set(ray2, ray_p, temp);
}


/*
 * Sends a shadow ray from the current point to a light source. Determines if there
 * is any object blocking the lights path. If no object is found it calculates the
 * diffuse color.
 * @inter: an intersection
 * @vrp: the view reference point
 * @return: the diffuse color
 */
Color Ray_send(Intersection *inter, Point vrp) {
	int i;
	Intersection *currIntersect = NULL;
	Vector ray_v;							// ray vector = light position - origin point
	Ray s_ray; 								// shadow ray
	Point shadow_p;							// shadow origin
	
	RayElement *e;
	Vector view;
	Point light_p;
	
	Color newColor = {{0.0,0.0,0.0}};
	Color diffuse = {{0.0,0.0,0.0}};
	
	// send shadow ray to each light
	for (i = 0; i< global_light->nLights; i++) {
		light_p = global_light->light[i].position;
		
		// calculate shadow ray direction
		ray_v.v[0] = light_p.val[0] - inter->p.val[0];
		ray_v.v[1] = light_p.val[1] - inter->p.val[1];
		ray_v.v[2] = light_p.val[2] - inter->p.val[2];
		shadow_p.val[0] = inter->p.val[0] + 10e-5 * inter->nor.v[0];
		shadow_p.val[1] = inter->p.val[1] + 10e-5 * inter->nor.v[1];
		shadow_p.val[2] = inter->p.val[2] + 10e-5 * inter->nor.v[2];
		
		// set the shadow ray
		Ray_set(&s_ray, shadow_p, ray_v);
		
		// grab the first element
		e = global_rayModule->head; 
		
		// intersect with all objects in the list
		while (e) {
			currIntersect = Ray_intersect(&s_ray,e);
			
			// once an object is blocking light, get out!
			if ((currIntersect != NULL)) {
				break;
			}
			// get the next element
			e = e->next;
		}  
	
		// if the light is not being blocked by an object
		if (currIntersect == NULL) {
			Color color;
		
			// calculate color at point
			view.v[0] = -inter->p.val[0] + vrp.val[0]; 
			view.v[1] = -inter->p.val[1] + vrp.val[1];
			view.v[2] = -inter->p.val[2] + vrp.val[2];
			Vector_normalize(&view);
			
			// diffuse color
			color = RayElement_getDiffuseColor(inter->e);
			
			// Calculate diffuse lighting here
			Light_diffuse(&(global_light->light[i]), &(inter->nor), &view, 
								&(inter->p), &color, 32.0, 1, &diffuse);
		}
		
		Color_sum(&newColor, &diffuse, &newColor);
	}
	return newColor;
}


/*
 * Intersects a ray with all objects in the scene and returns the
 * color at the given screen coordinates.
 * @ray: a ray
 * @depth: maximum depth
 * @eye: our point of view
 * @vrp: the view reference point
 * @return: color in screen coordinates
 */
Color Ray_trace(Ray *ray, int depth, Point eye, Point vrp) {
	Color pointColor = {{0.0, 0.0, 0.0}};
	Color reflectValue = {{0.0,0.0,0.0}};
	Color refractValue = {{0.0,0.0,0.0}};
	Color coeffReflect = {{0.0,0.0,0.0}}; 
	Color coeffRefract = {{0.0,0.0,0.0}};
	
	RayElement *e;
	Intersection *ret;
	Intersection *currIntersect;
	double minDist = 10e10;
	
	// return black if max depth is reached;
	if (depth == 0) {
		return pointColor; 
	}
	
	// allocate memory
	ret = malloc(sizeof(Intersection));
	
	// get the first element
	e = global_rayModule->head;
	
	// for each element in the module
	while (e) {
		currIntersect = Ray_intersect(ray,e);
		if (currIntersect != NULL) {
			// calculate the distance of the nearest intersection of R with the object
			double dist = Point_dist(&eye, &(currIntersect->p));
			
			if (dist < minDist) {
				// update dist to this distance
				minDist = dist;

				Intersection_copy(ret, currIntersect);
				ret->e = e;				
			}
		}
		// get the next element
		e = e->next;
	} 
	
	if (minDist < 10e10) {
		pointColor = Ray_send(ret, vrp);
		if (RayElement_isReflective(result->e) == 1){
			Ray reflectedRay;
			
			// calculate reflected ray and color
			Ray_reflect(ray, ret, &reflectedRay);
			reflectValue = Ray_trace(&reflectedRay, depth - 1, eye, vrp);
			
			// calculate reflection coefficient
			coeffReflect = RayElement_getSpecularColor(result->e);
		}
		// sum up the all the light at the point
		pointColor = addColors(pointColor, reflectValue, coeffReflect,
												refractValue, coeffRefract);
	}
	
	free(ret);
	return pointColor;
}


/* 
 * Sets the data of an intersection object
 * @i: a intersection object
 * @p: point of intersection
 * @nor: vector normal
 * @return: void
 */
void Intersection_set(Intersection *i, Point p, Vector nor) {
	Point_copy(&(i->p), &p);
	Vector_copy(&(i->nor), &nor);
}


/* 
 * Copies data from one intersection to another
 * @dest: an intersection object to copy to
 * @src: an intersection object to copy from
 * @return: void
 */
void Intersection_copy(Intersection *dest, Intersection *src) {
	Point_copy(&(dest->p), &(src->p));
	Vector_copy(&(dest->nor), &(src->nor));
}


/*
 * Intersects a ray with a ray object element and calls the 
 * appropriate function
 * @r: the ray
 * @e: an object element to intersect with
 * @return:
 */
Intersection* Ray_intersect(Ray *ray, RayElement *e) {
	Intersection *intersect;
	intersect = NULL;
	switch (e->type) {
		case RayObjPlane:
			intersect = Ray_planeIntersect(ray, &(e->obj.plane), 1);
			break;
		case RayObjSphere:
			intersect = Ray_sphereIntersect(ray, &(e->obj.sphere));
			break;
	}
	return intersect;
}


/*
 * Looks for possible intersections between a ray and a sphere
 * and returns the closet one. If there are no intersections
 * then NULL is returned.
 * @r: the ray
 * @sphere: the sphere the ray is trying to intersect
 * @return: the intersection if the ray hits the sphere
 */
Intersection* Ray_sphereIntersect(Ray *ray, Sphere *sphere) {
	// get values from ray
	Point ray_p = ray->p;						// ray oriting
	Vector ray_v = ray->v;						// ray vector
	
	// get values from sphere
	Point sphere_c = sphere->c;					// sphere center
	double sphere_r = sphere->r;				// sphere radius
	
	// new values
	Vector distRtoS;							// length from ray to sphere
	double distRtoS_2;							// ray to sphere squared
	double sphere_r_2 = sphere_r*sphere_r;		// sphere radius squared
	double ray_close, halfCord_2, inter_dist;
	
	// return variables
	Point inter;								// intersection point 
	Vector normal;								// normal at intersection point
	Intersection *ret;							// itnersection object to return
	
	// calculate distance between ray origin and sphere center
	Vector_set(&distRtoS, sphere_c.val[0] - ray_p.val[0],
								sphere_c.val[1] - ray_p.val[1],
								sphere_c.val[2] - ray_p.val[2]);
	// sqaure the distance
	distRtoS_2 = Vector_lenSquared(&distRtoS);
	
	// calculate ray distance which is closest to the center
	ray_close = Vector_dot(&distRtoS, &ray_v);
	
	// test if ray is outside and points away from sphere
	if ((ray_close < 0) && (distRtoS_2 >= sphere_r_2) ) {
		return NULL;
	}
	
	// find square of half chord intersection distance
	halfCord_2 = sphere_r_2 - distRtoS_2 + ray_close * ray_close;
	
	// just in case
	if (halfCord_2 < 0) {
		return NULL;
	}
	
	// calculate intersection distance
	if (distRtoS_2 >= sphere_r_2) {
		inter_dist = ray_close - sqrt(halfCord_2);
	}
	else {
		inter_dist = ray_close + sqrt(halfCord_2);
	}
	
	if (inter_dist == 0.0) {
		return NULL;
	}
	
	// calculate and set intersection point
	Point_set(&inter, ray_p.val[0] + ray_v.v[0] * inter_dist,
							ray_p.val[1] + ray_v.v[1] * inter_dist,
							ray_p.val[2] + ray_v.v[2] * inter_dist);
							
	// calculate and set normal at intersection point
	Vector_set(&normal, (inter.val[0] - sphere_c.val[0])/sphere_r,
						(inter.val[1] - sphere_c.val[1])/sphere_r,
						(inter.val[2] - sphere_c.val[2])/sphere_r);
	
	// make space for return value
	ret = malloc(sizeof(Intersection));
	
	// set the return value
	Intersection_set(ret, inter, normal);
	
	return ret;
}


/*
 * Looks for possible intersections between a ray and a plane
 * and returns the closet one. If there are no intersections
 * then NULL is returned.
 * @ray: the ray
 * @plane: the sphere the ray is trying to intersect
 * @return: the intersection if the ray hits the sphere
 */
Intersection* Ray_planeIntersect(Ray *ray, Plane *plane, int singleSide) {
	Point ray_p, inter;
	Vector normal, ray_v;
	Intersection *result;
	double A,B,C,D, v_out, v_0, inter_dist;
	
	A = plane->p[0];
	B = plane->p[1];
	C = plane->p[2];
	D = plane->p[3];
	
	ray_p = ray->p;
	ray_v = ray->v;
  
	// calculate the ray that comes out (planes index of refraction * ray vector)
	v_out = A * ray_v.v[0] + B * ray_v.v[1] + C * ray_v.v[2];
	
	// if the plane is parallel to the ray or if it has been culled
	if ( (v_out == 0) || ((v_out > 0) && (singleSide == 1))) {
		return NULL;
	}
	
	// calculate v_0 and t and compare t to zero
	v_0 = - (A * ray_p.val[0] + B * ray_p.val[1] + C * ray_p.val[2] + D);
	inter_dist = v_0/v_out;
	
	// if it intersects behind the origin
	if (inter_dist < 0) {
		return NULL;
	}
	
	// compute intersection point
	Point_set(&inter, ray_p.val[0] + ray_v.v[0] * inter_dist,
						ray_p.val[1] + ray_v.v[1] * inter_dist,
						ray_p.val[2] + ray_v.v[2] * inter_dist);
	
	// set the normal
	if (v_out < 0) {
		Vector_set(&normal, A, B, C);
	}
	else {
		Vector_set(&normal, -A, -B, -C);
	}
  
	// set the result
	result = malloc(sizeof(Intersection));
	Intersection_set(result, inter, normal);
	
	return result;
}


/*
 * Traces a ray until it finds an intersection
 * @ray: a ray
 * @depth: maximum depth
 * @eye: our point of view
 * @return: the intersection
 */
Intersection* trace(Ray *ray, int depth, Point eye) {
	double minDist = 10e10;
	Intersection *new;
	Intersection *currIntersect;
	RayElement *e;
	
	new = malloc(sizeof(Intersection));
	
	// get the first element
	e = global_rayModule->head;
	
	// for each element in the module
	while (e) {
	currIntersect = Ray_intersect(ray,e);
		if (currIntersect != NULL) {
			// calculate the distance of the nearest intersection of with the object
			double dist = Point_dist(&eye, &(currIntersect->p));
			
			if (dist < minDist) {
				// update the distance
				minDist = dist;
				Intersection_copy(new, currIntersect);
				new->e = e;
			}
		}
		// get the next element
		e = e->next;
	}  
	
	// if the distance is reasonable return it
	if (minDist < 10e10) {
		return new;
	}
	else {
		return NULL;
	}
}


/*
 * Calculates the lighting color by combining the point color with refraction and reflection
 * colors
 * @pointC: color at the point
 * @reflectV: reflection value
 * @coeffReflect: reflection coeffecient
 * @refractV: refraction value
 * @coeffRefract: refraction coefficient
 * @return: the combined color
 */
Color addColors(Color pointC, Color reflectV, Color coeffReflect,
									Color refractV, Color coeffRefract) {
	Color new;
	int i;
	
	for (i=0; i<3; i++) {
		new.c[i] = pointC.c[i] +
					reflectV.c[i] * coeffReflect.c[i] +
					refractV.c[i] * coeffRefract.c[0];
	}
	return new;
}

