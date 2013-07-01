/* Dan Nelson
 * Graphics Package
 * polygon.c
 */


#include <string.h>
#include "cb_graphics.h"


/******************
POLYGON FUNCTIONS
******************/

/* returns an allocated Polygon pointer 
 * initialized so that nVertex is 0 and vertex is NULL.*/
Polygon *Polygon_create(void) {
  Polygon *p = malloc(sizeof(Polygon));
  p->nVertex = 0;
  p->zBuffer = 1;
  p->oneSided = 0;
  p->vertex = NULL;
  p->normal = NULL;
  p->color = NULL;
  p->texCoord = NULL;
  return p;
}


/* returns an allocated Polygon pointer with the vertex list 
 * initialized to a copy of the points in vlist. */
Polygon *Polygon_init(int numV, Point *vlist) {
  int i;
  
  Polygon *p = (Polygon *)malloc(sizeof(Polygon));
  // if sucessfully allocated polygon pointer
  if (p != NULL) {
    p->nVertex = numV;
    p->vertex = (Point *)malloc(sizeof(Point)*numV); 
    //p->color = (Color *)malloc(sizeof(Color)*numV);
    // if sucessfully allocated vlist pointer
    if (p->vertex != NULL) {
      for (i = 0; i < numV; i++) {
        p->vertex[i] = vlist[i];
      }
      return p;
    }    
    // failed to init vlist pointer
    else {
      return NULL;
    }
  }
  // failed to init polygon pointer
  else {
    return NULL;
  }
}


/* frees the internal data and resets the fields. */
void Polygon_clear(Polygon *p) {
  p->nVertex = 0;
  if (p->vertex != NULL) {
    free(p->vertex);
  }
  if (p->normal != NULL) {
    free(p->normal);
  }
  if (p->color != NULL) {
    free(p->color);
  }
  if (p->texCoord != NULL) {
    free(p->texCoord);
  }
  p->vertex = NULL;
  p->normal = NULL;
  p->color = NULL;
  p->oneSided = 0;
  p->zBuffer = 1;
}


/* frees the internal data for a Polygon and the Polygon pointer. */
void Polygon_free(Polygon *p) {
  if (p->vertex != NULL)
    free(p->vertex);
  if (p->normal != NULL)
    free(p->normal);
  if (p->color != NULL)
    free(p->color);
  if (p->texCoord != NULL)
    free(p->texCoord);
  if (p != NULL)
    free(p);
}


/* initializes the existing Polygon to an empty Polygon. */
void Polygon_setNULL(Polygon *p) {
  p->nVertex = 0;
  p->vertex = NULL;
  p->normal = NULL;
  p->wVertex = NULL;
  p->wNormal = NULL;
  p->texCoord = NULL;
  p->color = NULL;
  p->oneSided = 0;
  p->zBuffer = 1;
}


/* initializes the vertex list to the points in vlist. */
void Polygon_set(Polygon *p, int numV, Point *vlist) {
  int i;
  
  if (p->vertex != NULL) {
    free(p->vertex);
  }
  
  p->nVertex = numV;
  p->vertex = malloc(sizeof(Point)*numV); 
  
  for (i = 0; i < numV; i++) {
    p->vertex[i] = vlist[i];
  }
  
}


/* sets the z-buffer flag to the given value. */
void Polygon_zBuffer(Polygon *p, int flag) {
  p->zBuffer = flag;
}


/* Allocates space and copies the vertex data from one polygon to the others.*/
void Polygon_copy(Polygon *to, Polygon *from) {
  int i;
  
  if (to->nVertex != 0) {
    Polygon_clear(to);
  }
  
  to->nVertex = from->nVertex;
  to->vertex = malloc(sizeof(Point)*to->nVertex);
  
  for (i=0; i < to->nVertex; i++) {
    Point_copy(&(to->vertex[i]), &(from->vertex[i]));
  }
  
  if (from->normal != NULL) {
    to->normal = malloc(sizeof(Vector)*to->nVertex);
    for (i=0; i < to->nVertex; i++) {
      Vector_copy(&(to->normal[i]), &(from->normal[i]));
    }
  }
  
  if (from->color != NULL) {
    to->color = malloc(sizeof(Color)*to->nVertex);
    for (i=0; i < to->nVertex; i++) {
      Color_copy(&(to->color[i]),&(from->color[i]));
    }
  }
  
  if (from->texCoord != NULL) {
    to->texCoord = malloc(sizeof(TextureCoord)*to->nVertex);
    for (i=0; i < to->nVertex; i++) {
      to->texCoord[i].s = from->texCoord[i].s;
      to->texCoord[i].t = from->texCoord[i].t;
    }
  }
  
  to->zBuffer = from->zBuffer;
  to->oneSided = from->oneSided;  
}


/* prints polyline data to the stream designated by the FILE pointer. */
void Polygon_print(Polygon *p, FILE *fp) {
  int i;
  int numV = p->nVertex;
  for (i=0; i<numV; i++) {
    fprintf(fp,"Vertex #: %d x: %f y: %f z: %f h: %f \n",i,
    		p->vertex[i].val[0],p->vertex[i].val[1],p->vertex[i].val[2],p->vertex[i].val[3]);
  }
}


/*draw the outline of the polyline using color color. If the zBuffer flag is set, should take into
 * account the z-buffer values when drawing lines.*/
void Polygon_drawFrame(Polygon *p, Image *src, Color color) {
  int numV = p->nVertex;
  Point *vlist = p->vertex;
  Point point1, point2;
  Line line;
  int i;
  
  printf("in draw frame \n");
  for (i = 0; i < numV - 1; i++) {
    //printf("drawing at vertex: %d \n",i);
    // put start and end point data into the point pointers
    Point_set2D(&point1, vlist[i].val[0], vlist[i].val[1]);
    Point_set2D(&point2, vlist[i+1].val[0], vlist[i+1].val[1]);
    // put line data into the line pointer
    Line_set(&line, point1, point2);
    Line_zBuffer(&line,p->zBuffer);
    // draw the line
    Line_draw(&line, src, color);
  }

  Point_set2D(&point1, vlist[numV-1].val[0], vlist[numV-1].val[1]);
  Point_set2D(&point2, vlist[0].val[0], vlist[0].val[1]);
  Line_set(&line, point1, point2);
  Line_zBuffer(&line,p->zBuffer);
  Line_draw(&line, src, color);
  printf("finish draw frame \n");
}


#define MAX_(x, y) (((x) > (y)) ? (x) : (y))
#define MIN_(x, y) (((x) < (y)) ? (x) : (y))


/* draw the filled polygon using color c with the
 * Barycentric coordinates algorithm. */
void Polygon_drawFillB(Polygon *p, Image *src, Color color) {
  int min_x,max_x,min_y,max_y;
  float xa,xb,xc;
  float ya,yb,yc;
  int i,j;
  float alpha,beta,gamma;
  
  xa = p->vertex[0].val[0];
  xb = p->vertex[1].val[0];
  xc = p->vertex[2].val[0];
  ya = p->vertex[0].val[1];
  yb = p->vertex[1].val[1];
  yc = p->vertex[2].val[1];
  
  min_x = MIN_(xa,xb);
  min_x = MIN_(min_x,xc);
  max_x = MAX_(xa,xb);
  max_x = MAX_(max_x,xc);
  
  min_y = MIN_(ya,yb);
  min_y = MIN_(min_y,yc);
  max_y = MAX_(ya,yb);
  max_y = MAX_(max_y,yc);
    
  for (i=min_x; i<max_x; i++) {
    for (j=min_y; j<max_y; j++) {
      beta = ( (ya-yc)*(i+0.5) + (xc-xa)*(j+0.5) + xa*yc - xc*ya ) /
             ( (ya-yc)*xb + (xc-xa)*yb + xa*yc - xc*ya );
      gamma = ( (ya-yb)*(i+0.5) + (xb-xa)*(j+0.5) + xa*yb - xb*ya ) /
              ( (ya-yb)*xc + (xb - xa)*yc + xa*yb - xb*ya );
      alpha = 1 - beta - gamma;
      
      if ( (alpha >= 0 && alpha <= 1) &&
           (beta >= 0 && beta <= 1) &&
           (gamma >= 0 && gamma <= 1) ) {
       
         Image_setColor(src,j,i,color);
      }
    }
  }
}


// normalize each vertex by the homogeneous coord
void Polygon_normalize(Polygon *p) {
  int i, numV;
  numV = p->nVertex;
  
  for (i = 0; i < numV; i++) {
    p->vertex[i].val[0] = p->vertex[i].val[0]/p->vertex[i].val[3];
    p->vertex[i].val[1] = p->vertex[i].val[1]/p->vertex[i].val[3];
    p->vertex[i].val[3] = 1.0;
  }
}


/* For the ShadeFlat and ShadeGouraud cases of the shade field 
 * of DrawState, calculate colors at each vertex and create and 
 * fill out the color array of the Polygon data structure. 
 */
void Polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds) {
  int i;
  float s = 32.0; //sharpness
  Vector tempVnormal = {{0.0,0.0,0.0,0.0}};
  Point tempVertex = {{0.0,0.0,0.0,0.0}};
  Vector view;
  
  if (p->color != NULL) {
    free(p->color);
  }
  // allocate the color array
  p->color = malloc(p->nVertex*sizeof(Color));
  
  switch(ds->shade)
  {   
    case ShadeFlat:
      printf("Polygon_shade: ShadeFlat \n");
      // calculate the average normal
      for (i = 0; i < p->nVertex; i++) {
        Vector_normalize(&(p->normal[i]));
        tempVnormal.v[0]+= p->normal[i].v[0];
        tempVnormal.v[1]+= p->normal[i].v[1];
        tempVnormal.v[2]+= p->normal[i].v[2];
        
        tempVertex.val[0]+= p->vertex[i].val[0];
        tempVertex.val[1]+= p->vertex[i].val[1];
        tempVertex.val[2]+= p->vertex[i].val[2];
        tempVertex.val[3]+= p->vertex[i].val[3];
      }
      tempVnormal.v[0] = tempVnormal.v[0]/p->nVertex;
      tempVnormal.v[1] = tempVnormal.v[1]/p->nVertex;
      tempVnormal.v[2] = tempVnormal.v[2]/p->nVertex;
      Vector_normalize(&tempVnormal);
      
      tempVertex.val[0] = tempVertex.val[0]/p->nVertex;
      tempVertex.val[1] = tempVertex.val[1]/p->nVertex;
      tempVertex.val[2] = tempVertex.val[2]/p->nVertex;
      tempVertex.val[3] = tempVertex.val[3]/p->nVertex;
      Point_normalize(&tempVertex);
      
      view.v[0] = -p->vertex[i].val[0] + ds->viewer.val[0]; 
      view.v[1] = -p->vertex[i].val[1] + ds->viewer.val[1];
      view.v[2] = -p->vertex[i].val[2] + ds->viewer.val[2];
      Vector_normalize(&view);

      Lighting_shading(lighting, &tempVnormal, &view, 
                       &(p->vertex[i]), &(ds->body), &(ds->surface), s,
                       p->oneSided, &(ds->flatColor));
      printf("flat color \n");
      Color_print(&(ds->flatColor),stdout);
      // calculate the color values
      for (i = 0; i < p->nVertex; i++) {
        p->color[i] = ds->flatColor;
      }
      break;
      
    case ShadeGouraud:
      //printf("Polygon_shade: ShadeGouraud \n");
      // calculate the color values
      for (i = 0; i < p->nVertex; i++) {  
        view.v[0] = -p->vertex[i].val[0] + ds->viewer.val[0]; 
        view.v[1] = -p->vertex[i].val[1] + ds->viewer.val[1];
        view.v[2] = -p->vertex[i].val[2] + ds->viewer.val[2];
        Vector_normalize(&view);     
        Vector_normalize(&(p->normal[i]));
        Lighting_shading(lighting, &(p->normal[i]), &view, 
                       &(p->vertex[i]), &(ds->body), &(ds->surface), s,
                       p->oneSided, &(p->color[i]));
        //Color_print(&(p->color[i]),stdout);
      }
      break;
      
    case ShadePhong:
      break;
    case ShadeFrame:
      break;
    case ShadeConstant:
      break;
    case ShadeDepth:
      break;
  }
}


// initializes the color array to the colors in clist.
void Polygon_setColors(Polygon *p, int numV, Color *clist) {
  int i;   
  
  if (p->color != NULL) {
    free(p->color);
  }
  
  p->color = malloc(sizeof(Color)*numV); 
  for (i=0; i<numV; i++) {
     p->color[i]= clist[i];
  }
  
}


// initializes the normal array to the vectors in nlist.
void Polygon_setNormals(Polygon *p, int numV, Vector *nlist) {
  int i;
  
  if (p->normal != NULL) {
    free(p->normal);
  }
  
  p->normal = malloc(sizeof(Vector)*numV);   
  for (i=0; i<numV; i++) {
     p->normal[i]= nlist[i];
  }
}


// initializes the texture coords array to the 
void Polygon_setTexture(Polygon *p, int numV, TextureCoord *texList) {
  int i;
  
  if (p->texCoord != NULL) {
    free(p->texCoord);
  }
  
  p->texCoord = malloc(sizeof(Vector)*numV);   
  for (i=0; i<numV; i++) {
     p->texCoord[i]= texList[i];
  }
}

// initializes the texture coords array to the 
/*void Polygon_setTexturePointer(Polygon *p, *tex) {
  int i;
  
  if (p->tex != NULL) {
    free(p->texCoord);
  }
  
  p->tex = tex;
} */

// calculate the normal vector and set the normal vectors of the polygon
// to the calculated normal
void Polygon_calculateNormals(Polygon *p, int numV) {
  int i;
  Vector v1, v2, normal;
  
  
  // calculate the normal by calculating cross product of two sides of the polygon
  v1.v[0] = p->vertex[0].val[0] - p->vertex[1].val[0];
  v1.v[1] = p->vertex[0].val[1] - p->vertex[1].val[1];
  v1.v[2] = p->vertex[0].val[2] - p->vertex[1].val[2];
  v1.v[3] = 0;
  
  v2.v[0] = p->vertex[1].val[0] - p->vertex[2].val[0];
  v2.v[1] = p->vertex[1].val[1] - p->vertex[2].val[1];
  v2.v[2] = p->vertex[1].val[2] - p->vertex[2].val[2];
  v2.v[3] = 0;

  Vector_cross(&v1,&v2,&normal);
  normal.v[3] = 0;
  
  // set the normal vectors to the calculated normal
  if (p->normal != NULL) {
    free(p->normal);
  }  
  p->normal = malloc(sizeof(Vector)*numV);
  
  for (i=0; i<numV; i++) {
     Vector_copy(&(p->normal[i]),&normal);
  }  
  
}


// initializes the vertex list to the points in vlist, the colors to the colors 
// in clist, and the normals to the vectors in nlist.
void Polygon_setAll(Polygon *p, int numV, Point *vlist, 
  Color *clist, Vector *nlist, int zBuffer, int oneSided,
  TextureCoord *texList) {
  
  Polygon_set(p,numV,vlist);
  Polygon_setColors(p,numV,clist);
  Polygon_setNormals(p,numV,nlist);
  Polygon_setTexture(p,numV,texList);
  p->zBuffer = zBuffer;
  p->oneSided = oneSided;
}


//Store vertex and normal info into p->wVertex and p->wNormal
//Used for Phong shading
void Polygon_setWorld(Polygon *p, int numV) {
  int i;
  if (p->wVertex != NULL)
    free(p->wVertex);
  if (p->wNormal != NULL)
    free(p->wNormal);

  p->wNormal = malloc(sizeof(Vector)*numV);
  for (i=0;i<numV;i++) {
    Vector_copy(&(p->wNormal[i]),&(p->normal[i]));
  }
  
  p->wVertex = malloc(sizeof(Point)*numV);
  for (i=0;i<numV;i++) {
    Point_copy(&(p->wVertex[i]),&(p->vertex[i]));
  }
}

