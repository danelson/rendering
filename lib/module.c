/* Dan Nelson
 * Graphics Package
 * module.c
 */


#include <string.h>
#include "cb_graphics.h"


/*******************
*      Element     *
********************/

// Allocate an empty Element.
Element *Element_create() {
  Element *e = malloc(sizeof(Element));
  e->type = ObjNone;
  e->next = NULL;
  return e;
}

/*
 * Allocate an Element and store a duplicate of the data pointed
 * to by obj in the Element. Modules do not get duplicated. The
 * function needs to handle each type of object separately in a
 * case statement.
 */
Element *Element_init(ObjectType type, void *obj) {
  Element *e = Element_create();
  e->type = type;
  switch (type)
  {
    case ObjNone:
      break;
    case ObjLine:
      memcpy(&(e->obj.line), obj, sizeof(Line));
      break;
    case ObjPoint:
      memcpy(&(e->obj.point), obj, sizeof(Point));
      break;
    case ObjPolyline:
      Polyline_setNULL(&(e->obj.polyline));
      Polyline_copy(&(e->obj.polyline),(Polyline*)obj);
      break;
    case ObjPolygon:
      Polygon_setNULL(&(e->obj.polygon));
      Polygon_copy(&(e->obj.polygon),(Polygon*)obj);
      break;
    case ObjCircle:
      memcpy(&(e->obj.circle), obj, sizeof(Circle));
      break;
    case ObjSphere:
      memcpy(&(e->obj.sphere), obj, sizeof(Sphere));
    case ObjPlane:
      memcpy(&(e->obj.plane), obj, sizeof(Plane));
    case ObjIdentity:
      memcpy(&(e->obj.matrix), obj, sizeof(Matrix));
      break;
    case ObjMatrix:
      memcpy(&(e->obj.matrix), obj, sizeof(Matrix));
      break;
    case ObjColor:
      Color_copy(&(e->obj.color), obj);
      break;
    case ObjBodyColor:
      Color_copy(&(e->obj.color), obj);
      break;
    case ObjSurfaceColor:
      Color_copy(&(e->obj.color), obj);
      break;
    case ObjSurfaceCoeff:
      memcpy(&(e->obj.coeff), obj, sizeof(float));
      break;
    case ObjLight:
      //memcpy(&(e->obj.????), obj, sizeof(????));
      break;
    case ObjTexture:
      e->obj.tex = obj;
    case ObjModule:
      e->obj.module = obj;
      break;
    default:
      break;
  }
  return e;
}

// free the element and the object it contains
void Element_delete(Element *e) {
  switch (e->type)
  {
    case ObjPolyline:
      Polyline_clear(&(e->obj.polyline));
      break;
    case ObjPolygon:
      Polygon_clear(&(e->obj.polygon));
      break;
  	default:
  	  break;
  }
  free(e);
}


/*******************
*       Module     *
********************/


// Allocate an empty module.
Module *Module_create() {
  Module *md = malloc(sizeof(Module));
  md->head = NULL;
  md->tail = NULL;
  return md;
}

// Clear the module’s list of Elements, freeing memory as appropriate.
void Module_clear(Module *md) {
  Element *p,*q;
  p = md->head;
  while (p) {
    q = p;
    p = p->next;
    Element_delete(q);
  }
}

// Free all of the memory associated with a module, including
// the memory pointed to by md.
void Module_delete(Module *md) {
  Module_clear(md);
  free(md);
}

// Generic insert of an element into the module at the tail of the list.
void Module_insert(Module *md, Element *e) {
  if (md->head == NULL) {
    md->head = e;
    md->tail = e;
  }
  else {
    md->tail->next = e;
    md->tail = e;
  }
}


// Adds a pointer to the Module sub to the tail of the module’s list.
void Module_module(Module *md, Module *sub) {
  Element *e = Element_init(ObjModule, sub);
  Module_insert(md, e);
}


// Adds p to the tail of the module’s list.
void Module_point(Module *md, Point *p) {
  Element *e = Element_init(ObjPoint, p);
  Module_insert(md, e);
}


// Adds p to the tail of the module’s list.
void Module_line(Module *md, Line *l) {
  Element *e = Element_init(ObjLine, l);
  Module_insert(md, e);
}


// Adds p to the tail of the module’s list.
void Module_polyline(Module *md, Polyline *p) {
  Element *e = Element_init(ObjPolyline, p);
  Module_insert(md, e);
}


// Adds p to the tail of the module’s list.
void Module_polygon(Module *md, Polygon *p) {
  Element *e = Element_init(ObjPolygon, p);
  Module_insert(md, e);
}


// Adds p to the tail of the module’s list.
void Module_circle(Module *md, Circle *c) {
  Element *e = Element_init(ObjCircle, c);
  Module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void Module_sphere(Module *md, Sphere *s) {
  Element *e = Element_init(ObjSphere, s);
  Module_insert(md, e);
}


// Adds p to the tail of the module’s list.
void Module_plane(Module *md, Plane *p) {
  Element *e = Element_init(ObjPlane, p);
  Module_insert(md, e);
}


//
void Module_texture(Module *md, Texture *tex) {
  Element *e = Element_init(ObjTexture, tex);
  Module_insert(md, e);
}


/*
 * Object that sets the current transform to the identity,
 * placed at the tail of the module’s list.
 */
void Module_identity(Module *md) {
  Matrix m;
  Matrix_identity(&m);
  Element *e = Element_init(ObjIdentity, &m);
  Module_insert(md, e);
}

/*
 * Matrix operand to add a translation matrix to the tail
 * of the module’s list.
 */
void Module_translate2D(Module *md, double tx, double ty) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_translate2D(&m, tx, ty);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

/*
 * Matrix operand to add a scale matrix to the tail of the
 * module’s list.
 */
void Module_scale2D(Module *md, double sx, double sy) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_scale2D(&m, sx, sy);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

/*
 * Matrix operand to add a rotation about the Z axis to the
 * tail of the module’s list.
 */
void Module_rotateZ(Module *md, double cth, double sth) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_rotateZ(&m, cth, sth);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

/*
 * Matrix operand to add a 2D shear matrix to the tail of
 * the module’s list.
 */
void Module_shear2D(Module *md, double shx, double shy) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_translate2D(&m, shx, shy);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

/*
 * Draw the module into the image using the given view transformation
 * matrix [VTM], Lighting and DrawState by traversing the list of
 * Elements. (For now, Lighting can be an empty structure.)
 */
void Module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds,
				Lighting *lighting, Image *src) {

  printf("module draw\n");

  // set the matrix LTM to identity
  Matrix LTM;
  Matrix_identity(&LTM);
  Line l;
  Point x;
  Polyline pl;
  Polygon pg;
  Circle circle;
  Matrix TM;  
  DrawState *tempDS = DrawState_create();

  Polygon_setNULL(&pg);
  Polyline_setNULL(&pl);
  
  // for each element E in the module md
  Element *e;
  e = md->head;
  
  while (e) {

    //if (e->type >= 13)
    //printf("e->type is NULL X_X\n");
    switch (e->type)
    {
      case ObjNone:
        printf("objNone\n");
        break;
        
      case ObjLine:
        printf("objline\n");
        // copy the line data in E to L
        memcpy(&l, &(e->obj.line), sizeof(Line));
        
        // transform L by the LTM, GTM, VTM
        Matrix_xformLine(&LTM, &l);
        Matrix_xformLine(GTM, &l);
        Matrix_xformLine(VTM, &l);
        
        // normalize L by the homogeneous coord
        Line_normalize(&l);
        
        // draw L using DS->color
        Line_draw(&l, src, ds->color);
        break;
        
      case ObjPoint:
        printf("objpoint\n");
        // copy the line data in E to X
        memcpy(&x, &(e->obj.point), sizeof(Point));
        
        //transform X by the LTM, GTM, VTM        
        Matrix_xformPoint(&LTM, &x, &x);
        Matrix_xformPoint(GTM, &x, &x);
        Matrix_xformPoint(VTM, &x, &x);
        
        // normalize X by the homogeneous coord
        Point_normalize(&x);
        
        // draw X using DS->color (if X is in the image)
        Point_draw(&x, src, ds->color);
        break;
        
      case ObjPolyline:
        printf("objpolyline\n");
        // copy the polyline data in E to P
        Polyline_copy(&pl, &(e->obj.polyline));
        
        //transform P by the LTM, GTM, VTM
        Matrix_xformPolyline(&LTM, &pl);
        Matrix_xformPolyline(GTM, &pl);
        Matrix_xformPolyline(VTM, &pl);
        
        //normalize P by the homogeneous coord
        Polyline_normalize(&pl);
        
        // draw P using DS->color
        Polyline_drawFrame(&pl, src, ds->color);
        break;
        
      case ObjPolygon:
        printf("objpolygon\n");
        // copy the polygon data in E to P
        Polygon_copy(&pg, &(e->obj.polygon));
        //transform P by the LTM, GTM,
        Matrix_xformPolygon(&LTM, &pg);
        Matrix_xformPolygon(GTM, &pg);
        
        // if shadePhong, store world coordinate into appropriate fields     
        if (ds->shade == ShadePhong) {
          printf("before setworld \n");
          Polygon_setWorld(&pg,pg.nVertex);
          printf("after setworld \n");
        }
	    //printf("l->nLights %d \n", lighting->nLights);
        if ((ds->shade == ShadeGouraud) || (ds->shade == ShadeFlat)) {
          // call Polygon_shade to calculate color at each vertex using p
          printf("before polygon shade \n");
          Polygon_shade(&pg, lighting, ds);
          printf("after polygon shade \n");
        }
        
        // transform by VTM
        Matrix_xformPolygon(VTM, &pg);        
        
        //Homogenize the X and Y coordinates
        Polygon_normalize(&pg);
		
		//Polygon_drawFrame(&pg,src,ds->color);
        printf("before drawshade \n");
		Polygon_drawShade(&pg, src, ds, lighting);
		printf("after drawshade \n");
		
        break;
        
        
    case ObjCircle:
        printf("objcircle\n");
        // copy the polyline data in E to P
        memcpy(&circle, &(e->obj.circle), sizeof(Circle));        
        
        //Matrix temp;
        //Matrix_identity(&temp);
        //Matrix_multiply(GTM,&LTM,&temp);
        //Matrix_multiply(&temp,VTM,&temp);
        
        Circle_drawXForm(&circle, src, ds->color, VTM);
        //Circle_draw(&circle,src,ds->color);
        break;
        
      case ObjIdentity:
        printf("identity\n");
        // LTM = I
        Matrix_identity(&LTM);
        break;
        
      case ObjMatrix:
      	printf("objmatrix\n");
        //LTM = (Matrix field of E) * LTM
        Matrix_multiply(&(e->obj.matrix), &LTM, &LTM);
        break;
        
      case ObjColor:
        printf("objcolor\n");
        Color_copy(&(ds->color),&(e->obj.color));
        break;
        
      case ObjBodyColor:
        printf("objbodycolor\n");
        Color_copy(&(ds->body),&(e->obj.color));
        //ds->body = e->obj.color;
        break;
        
      case ObjSurfaceColor:
        printf("objsurfacecolor\n");
        Color_copy(&(ds->surface),&(e->obj.color));
        //ds->surface = e->obj.color;
        break;
        
      case ObjSurfaceCoeff:
        printf("objsurfacecoeff\n");
        ds->surfaceCoeff = e->obj.coeff;
        break;
        
      case ObjLight:
      	printf("objlight\n");
        //memcpy(&(e->obj.????), (char*)obj, sizeof(????));
        break;
        
      case ObjTexture:
        printf("objtexture\n");
        ds->tex = e->obj.tex;
        break;
        
      case ObjModule:
        printf("objmodule\n");
        //TM = GTM * LTM
        Matrix_multiply(GTM, &LTM, &TM);
        
        //tempDS = DS
        DrawState_copy(tempDS, ds);
        //Module_draw( (Module field of E), VTM, TM, tempDS, Light, src );
        
        Module_draw(e->obj.module, VTM, &TM, tempDS, lighting, src);
        break;
        
      default:
        printf("default\n");
        break;
    }
    
    e = e->next;
  }
}

// Matrix operand to add a 3D translation to the Module.
void Module_translate(Module *md, double tx, double ty, double tz) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_translate(&m, tx, ty, tz);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

// Matrix operand to add a 3D scale to the Module.
void Module_scale(Module *md, double sx, double sy, double sz) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_scale(&m, sx, sy, sz);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

// Matrix operand to add a rotation about the X-axis to the Module.
void Module_rotateX(Module *md, double cth, double sth) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_rotateX(&m, cth, sth);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

// Matrix operand to add a rotation about the Y-axis to the Module.
void Module_rotateY(Module *md, double cth, double sth) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_rotateY(&m, cth, sth);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

// Matrix operand to add a rotation that orients to the orthonormal axes u,v,w
void Module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w) {
  Matrix m;
  Element *e;
  Matrix_identity(&m);
  Matrix_rotateXYZ(&m, u, v, w);
  e = Element_init(ObjMatrix, &m);
  Module_insert(md, e);
}

/* Adds a unit cube, axis-aligned and centered on zero to the Module.
 * If solid is zero, add only lines. If solid is non-zero, use
 * polygons. Only lines need to be supported now.
 */
void Module_cube(Module *md, int solid, Texture *tex[6]) {
  Point v[8];
  Point  tv[4];
  Vector normal[4];
  TextureCoord st[4];
  printf("adding cube...\n");
  // corners of a cube, centered at (0, 0, 0)
  Point_set( &v[0], -0.5, -0.5, -0.5 );
  Point_set( &v[1],  0.5, -0.5, -0.5 );
  Point_set( &v[2],  0.5,  0.5, -0.5 );
  Point_set( &v[3], -0.5,  0.5, -0.5 );
  Point_set( &v[4], -0.5, -0.5,  0.5 );
  Point_set( &v[5],  0.5, -0.5,  0.5 );
  Point_set( &v[6],  0.5,  0.5,  0.5 );
  Point_set( &v[7], -0.5,  0.5,  0.5 );
  // add only line
  if (solid == 0) {
    Line temp;
    
    Line_set(&temp,v[0],v[1]);
    Module_line(md,&temp);
    Line_set(&temp,v[0],v[3]);
    Module_line(md,&temp);
    Line_set(&temp,v[0],v[4]);
    Module_line(md,&temp);
    
    Line_set(&temp,v[1],v[2]);
    Module_line(md,&temp);
    Line_set(&temp,v[1],v[5]);
    Module_line(md,&temp);

    Line_set(&temp,v[2],v[3]);
    Module_line(md,&temp);
    Line_set(&temp,v[2],v[6]);
    Module_line(md,&temp);
    
    Line_set(&temp,v[3],v[7]);
    Module_line(md,&temp);
    
    Line_set(&temp,v[4],v[5]);
    Module_line(md,&temp);
    Line_set(&temp,v[4],v[7]);
    Module_line(md,&temp);
    
    Line_set(&temp,v[5],v[6]);
    Module_line(md,&temp);
    
    Line_set(&temp,v[6],v[7]);
    Module_line(md,&temp);
  }
  // use polygons
  else {
    Polygon temp;
    Polygon_setNULL(&temp);
    
    // THIS IS HARD-CODED !!!!!
    if (tex != NULL) {
      st[0].s = 372; st[0].t = 0;
      st[1].s = 0; st[1].t = 0;
      st[2].s = 0; st[2].t = 372;
      st[3].s = 372; st[3].t = 372;
    }
    // front side
    Polygon_set( &temp, 4, &(v[0]) );
    Vector front = {{0.0,0.0,-1.0}};
    normal[0] = normal[1] = normal[2] 
      = normal[3] = front;
    Polygon_setNormals( &temp, 4, normal);    
    if (tex != NULL) 
      Module_texture(md,tex[0]);
      Polygon_setTexture(&temp, 4, st);

    Module_polygon(md,&temp);

    // back side
    Polygon_set( &temp, 4, &(v[4]) );
    Vector back = {{0.0,0.0,1.0}};
    normal[0] = normal[1] = normal[2] 
      = normal[3] = back;
    Polygon_setNormals( &temp, 4, normal);
    if (tex != NULL) 
      Module_texture(md,tex[1]);  
      Polygon_setTexture(&temp, 4, st);
    Module_polygon(md,&temp);
    
    // top side
    Point_copy( &tv[0], &v[2] );
    Point_copy( &tv[1], &v[3] );
    Point_copy( &tv[2], &v[7] );
    Point_copy( &tv[3], &v[6] );

    Polygon_set( &temp, 4, tv );
    Vector top = {{0.0,1.0,0.0}};
    normal[0] = normal[1] = normal[2] 
      = normal[3] = top;
    Polygon_setNormals( &temp, 4, normal);
    if (tex != NULL) 
      Module_texture(md,tex[2]);  
      Polygon_setTexture(&temp, 4, st);
    Module_polygon(md,&temp);

    // bottom side
    Point_copy( &tv[0], &v[0] );
    Point_copy( &tv[1], &v[1] );
    Point_copy( &tv[2], &v[5] );
    Point_copy( &tv[3], &v[4] );

    Polygon_set( &temp, 4, tv );
    Vector bottom = {{0.0,-1.0,0.0}};
    normal[0] = normal[1] = normal[2] 
      = normal[3] = bottom;
    Polygon_setNormals( &temp, 4, normal);    
    if (tex != NULL) 
      Module_texture(md,tex[3]);  
      Polygon_setTexture(&temp, 4, st);

    Module_polygon(md,&temp);

    // left side
    Point_copy( &tv[0], &v[0] );
    Point_copy( &tv[1], &v[3] );
    Point_copy( &tv[2], &v[7] );
    Point_copy( &tv[3], &v[4] );

    Polygon_set( &temp, 4, tv );
    Vector left = {{-1.0,0.0,0.0}};
    normal[0] = normal[1] = normal[2] 
      = normal[3] = left;
    Polygon_setNormals( &temp, 4, normal);       
    if (tex != NULL) 
      Module_texture(md,tex[4]);  
      Polygon_setTexture(&temp, 4, st);
    Module_polygon(md,&temp);

    // right side
    Point_copy( &tv[0], &v[1] );
    Point_copy( &tv[1], &v[2] );
    Point_copy( &tv[2], &v[6] );
    Point_copy( &tv[3], &v[5] );

    Polygon_set( &temp, 4, tv );
    Vector right = {{1.0,0.0,0.0}};
    normal[0] = normal[1] = normal[2] 
      = normal[3] = right;
    Polygon_setNormals( &temp, 4, normal);  
    if (tex != NULL) 
      Module_texture(md,tex[5]);  
      Polygon_setTexture(&temp, 4, st);

    Module_polygon(md,&temp);
  }
  return;
}

// Adds the foreground color value to the tail of the module’s list.
void Module_color(Module *md, Color *c) {
  Element *e = Element_init(ObjColor, c);
  Module_insert(md, e);
}

// Adds the body color value to the tail of the module’s list.
void Module_bodyColor(Module *md, Color *c) {
  Element *e = Element_init(ObjBodyColor, c);
  Module_insert(md, e);
}

// Adds the surface color value to the tail of the module’s list.
void Module_surfaceColor(Module *md, Color *c) {
  Element *e = Element_init(ObjSurfaceColor, c);
  Module_insert(md, e);
}

// Adds the specular coefficient to the tail of the module’s list.
void Module_surfaceCoeff(Module *md, float coeff) {
  Element *e = Element_init(ObjSurfaceCoeff, &coeff);
  Module_insert(md, e);
}

