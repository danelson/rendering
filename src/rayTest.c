/* Dan Nelson
 * Graphics Package
 * rayTest.c
 * Main function for ray tracer
 */


#include "cb_graphics.h"


void raw_input(char *prompt, char *buffer, size_t length);
void raw_input(char *prompt, char *buffer, size_t length) {
    printf("%s", prompt);
    fflush(stdout);
    fgets(buffer, length, stdin);
}


int main(int argc, char *argv[]) {  
	Image *src;
	int rows = 700;
	int cols = 1200;
	int x,y;
	
	// Variables
	Point lightPos;
	Point sphere_c;
	Plane plane;
	Sphere sphere;
	Matrix VTM;
	Matrix GTM;
	View3D view;
	Vector view_u;
	Vector view_v;
	Ray currRay;
	Vector currV;
	Point eye;
	
	// Lots of colors
	Color pointC;
	Color black = {{0.0, 0.0, 0.0}};
	Color white = {{1.0, 1.0, 1.0}};
	Color red = {{0.7, 0.13, 0.13}};
	Color green = {{0.0, 0.4, 0.0}};
	Color blue = {{0.0, 0.0, 0.8}};
	Color skyBlue = {{0.53, 0.81, 0.98}};
	Color slateBlue = {{106.0/255.0, 90.0/255.0, 205.0/255.0}};
	Color darkTurquoise = {{0.0, 206.0/255.0, 209.0/255.0}};
	Color yellow = {{1.0, 1.0, 0.0}};
	Color khaki = {{240.0/255.0, 230.0/255.0, 140.0/255.0}};
	Color orange = {{1.0, 165.0/255.0, 0.0}};
	
	Color coeffReflect = {{0.8, 0.8, 0.8}};
	Color coeffRefract = {{0.5, 0.5, 0.5}};
	
	// initialize matrices
	Matrix_identity(&GTM);
	Matrix_identity(&VTM);
		
	// set the View parameters  
	Point_set(&(view.vrp), 10, 6, 10);
	Vector_set(&(view.vpn), -10, -6, -10);
	Vector_set(&(view.vup), 0.0, 1.0, 0.0);
	Vector_cross(&(view.vup), &(view.vpn), &(view_u));
	Vector_cross(&(view.vpn), &(view_u), &(view_v));
	Vector_normalize(&(view_u));
	Vector_normalize(&(view_v));

	view.d = 1.0;
	view.du = 6.0;
	view.dv = 3.5;
	view.f = 3.0;
	view.b = 15.0;
	view.screenx = cols;
	view.screeny = rows;
	
	Matrix_setView3D(&VTM, &view);
	
	printf("VTM: \n");
	Matrix_print(&VTM, stdout);
	
	// initialize the image
	src = Image_create(rows, cols);
	
	// Add a single white light on the left pointing right
	global_light = Lighting_create();
	Point_set(&(lightPos), 8, 8, -1);
	Lighting_add( global_light, LightPoint, &white, NULL, &(lightPos), 0, 0 );
	
	// Create the module
	global_rayModule = RayModule_create();
	
	// Float ball 1
	Point_set(&sphere_c, 3.0, 1.5, -3.5);
	Sphere_set(&(sphere), sphere_c, 0.75);
	Sphere_setColor(&(sphere), orange, coeffRefract, coeffReflect, 1, 1);
	Sphere_setRIndex(&(sphere), 1.5);
	RayModule_sphere(global_rayModule, &(sphere));
	
	// Ball 4
	Point_set(&sphere_c, -6, 0.75, 0);
	Sphere_set(&(sphere), sphere_c, 1.6);
	Sphere_setColor(&(sphere), darkTurquoise, coeffRefract, coeffReflect, 1, 1);
	Sphere_setRIndex(&(sphere), 1.5);
	RayModule_sphere(global_rayModule, &(sphere));
	
	// Ball 3
	Point_set(&sphere_c, -1, 0, 0);
	Sphere_set(&(sphere), sphere_c, 1.4);
	Sphere_setColor(&(sphere), green, coeffRefract, coeffReflect, 1, 1);
	Sphere_setRIndex(&(sphere), 1.5);
	RayModule_sphere(global_rayModule, &(sphere));
	
	// Ball 2
	Point_set(&sphere_c, 3, -0.75, 0);
	Sphere_set(&(sphere), sphere_c, 1.2);
	Sphere_setColor(&(sphere), red, coeffRefract, coeffReflect, 1, 1);
	Sphere_setRIndex(&(sphere), 1.5);
	RayModule_sphere(global_rayModule, &(sphere));
	
	// Ball 1
	Point_set(&sphere_c, 6, -1.5, 0);
	Sphere_set(&(sphere), sphere_c, 1.0);
	Sphere_setColor(&(sphere), slateBlue, coeffRefract, coeffReflect, 1, 1);
	Sphere_setRIndex(&(sphere), 1.5);
	RayModule_sphere(global_rayModule, &(sphere));
	
	// The floor
	Plane_set(&plane, 0, 1, 0, 1);
	Plane_setColor(&(plane), khaki, coeffReflect, 1);
	RayModule_plane(global_rayModule, &(plane));
	
	// Trace the rays	
	for (y=0; y<rows; y++) {
		for (x=0; x<cols; x++) {
			// set the current vector
			currV.v[0] = view.d * view.vpn.v[0] 
					  + view.du * ((2*(double)x+1)/(2*(double)cols) - 0.5) * view_u.v[0] 
					  + view.dv * ((2*(double)y+1)/(2*(double)rows) - 0.5) * view_v.v[0];
			currV.v[1] = view.d * view.vpn.v[1] 
					  + view.du * ((2*(double)x+1)/(2*(double)cols) - 0.5) * view_u.v[1] 
					  + view.dv * ((2*(double)y+1)/(2*(double)rows) - 0.5) * view_v.v[1];
			currV.v[2] = view.d * view.vpn.v[2] 
					  + view.du * ((2*(double)x+1)/(2*(double)cols) - 0.5) * view_u.v[2] 
					  + view.dv * ((2*(double)y+1)/(2*(double)rows) - 0.5) * view_v.v[2];
					
			// set our view
			eye.val[0] = view.vrp.val[0] - view.d * view.vpn.v[0];
			eye.val[1] = view.vrp.val[1] - view.d * view.vpn.v[1];
			eye.val[2] = view.vrp.val[2] - view.d * view.vpn.v[2];
			
			// set the current ray
			Ray_set(&currRay, eye, currV);
			
			// trace the ray
			pointC = Ray_trace(&currRay, 10, eye, view.vrp);
			
			// Set the color
			Image_setColor(src, rows-y, x, pointC);
		}
	}
	
	// Write the image
	Image_writePPM( src, "raytest.ppm" );
	
	// Free the image
	Image_free( src );
	
	return(0);
}


