/* Dan Nelson
 * Graphics Package
 * lighting.c
 */


#include "cb_graphics.h"


// initialize the light to default values
void Light_init( Light *light ) {
  light->type = LightNone;
  
  Color_set(&(light->color), 1.0, 1.0, 1.0);
  
  light->direction.v[0] = 1.0;
  light->direction.v[1] = 0.0;
  light->direction.v[2] = 0.0;
  
  light->position.val[0] = 0.0;
  light->position.val[1] = 0.0;
  light->position.val[2] = 0.0;
  
  light->cutoff = 0.0;
  light->sharpness = 32.0;
}


// copy the light information
void Light_copy( Light *to, Light *from ) {
  // memcpy(to,from, sizeof(Light))
  
  to->type = from->type;
    
  to->color.c[0] = from->color.c[0];
  to->color.c[1] = from->color.c[1];
  to->color.c[2] = from->color.c[2];
  
  to->direction.v[0] = from->direction.v[0];
  to->direction.v[1] = from->direction.v[1];
  to->direction.v[2] = from->direction.v[2];
  
  to->position.val[0] = from->position.val[0];
  to->position.val[1] = from->position.val[1];
  to->position.val[2] = from->position.val[2];
  
  to->cutoff = from->cutoff;
  to->sharpness = from->sharpness;
}


// allocate and return a new lighting structure set to default values
Lighting *Lighting_create(void) {
  int i;
  Lighting *lighting = malloc(sizeof(Lighting));
  lighting->nLights = 0;
  
  for (i=0;i < MAX_LIGHTS; i++) {
    Light_init(&(lighting->light[i]));
  }
  return lighting;
}


// initialize the lighting structure to default values
void Lighting_init( Lighting *l ) {
  l->nLights = 0;
}


/* add a new light to the Lighting structure given the parameters, some 
 * of which may be NULL, depending upon the type. Make sure you don’t
 * add more lights than MAX LIGHTS
 */
void Lighting_add( Lighting *l, LightType type, Color *c, Vector *dir,
					Point *pos, float cutoff, float sharpness ) {
  
  //Color_set(&l->light[nLights]->color, c->c[0], c->c[1], c->c[2]);

  if (l->nLights < MAX_LIGHTS - 1) {
	Light_init(&l->light[l->nLights]);
	
	l->light[l->nLights].type = type;
	
	if (c != NULL) {
		l->light[l->nLights].color.c[0] = c->c[0];
		l->light[l->nLights].color.c[1] = c->c[1];
		l->light[l->nLights].color.c[2] = c->c[2];
	}
	
	if (dir != NULL) {
		l->light[l->nLights].direction.v[0] = dir->v[0];
		l->light[l->nLights].direction.v[1] = dir->v[1];
		l->light[l->nLights].direction.v[2] = dir->v[2];
	}
	
	if (pos != NULL) {
		l->light[l->nLights].position.val[0] = pos->val[0];
		l->light[l->nLights].position.val[1] = pos->val[1];
		l->light[l->nLights].position.val[2] = pos->val[2];
	}
	
	l->light[l->nLights].cutoff = cutoff;
	l->light[l->nLights].sharpness = sharpness;
 
	l->nLights++;
  }
  else {
    printf("MAX_LIGHTS reached. Can't add more lights");
  }
}


/* calculate the proper color given the normal N, view vector V,
 * 3D point P, body color Cb, surface color Cs, sharpness value s,
 * the lighting, and whether the polygon is one-sided or two-sided.
 * Put the result in the Color c.
 */
void Lighting_shading( Lighting *l, Vector *N, Vector *V, 
                       Point *p, Color *Cb, Color *Cs, float s,
                       int oneSided, Color *c ) {
  double r,g,b;
  double V1, V2;
  Vector lVector, H;
  
  r = g = b = 0.0;
  Vector_normalize(N);
  Vector_normalize(V);
  Point_normalize(p);
  
  int i;
  
  for(i=0;i<l->nLights;i++) {
    Light currentL = l->light[i];
    switch(currentL.type)
    {
      case LightNone:
        break;
        
      case LightAmbient:
        /*printf("in light ambient \n");
        printf("\n");
        printf("current color: (%f, %f, %f)\n",
        						currentL.color.c[0],
        						currentL.color.c[1],
        						currentL.color.c[2]);
        printf("CB color: (%f, %f, %f)\n",
        						Cb->c[0],
        						Cb->c[1],
        						Cb->c[2]);
    	printf("\n"); */
        
        r += currentL.color.c[0]*Cb->c[0];
        g += currentL.color.c[1]*Cb->c[1];
        b += currentL.color.c[2]*Cb->c[2];
        break;
        
      case LightDirect:
        break;
        
      case LightPoint:   

        //lVector = currentL.position minus p
        lVector.v[0] = currentL.position.val[0] - p->val[0];
        lVector.v[1] = currentL.position.val[1] - p->val[1];
        lVector.v[2] = currentL.position.val[2] - p->val[2];
        Vector_normalize(&lVector);

        //H = lVector + V divided by 2
        H.v[0] = (double)(lVector.v[0] + V->v[0]) / 2.0;
        H.v[1] = (double)(lVector.v[1] + V->v[1]) / 2.0;
        H.v[2] = (double)(lVector.v[2] + V->v[2]) / 2.0;
       
        
        V1 = Vector_dot(&lVector, N);
        V2 = Vector_dot(&H, N);

        //if onesided and V1<0 then dont add in
        if ((oneSided == 1) && (V1 < 0)) {
          break;
        }
        
        //if not onesided and V1 <0 then negate V1 and calculate V2               
        if ((oneSided == 0) && (V1 < 0)) {
          V1 = -V1;
          V2 = -V2;
        }
        V2 = pow(V2,s);
        r += Cb->c[0]*currentL.color.c[0]*V1 + currentL.color.c[0]*Cs->c[0]*V2;
        g += Cb->c[1]*currentL.color.c[1]*V1 + currentL.color.c[1]*Cs->c[1]*V2;
        b += Cb->c[2]*currentL.color.c[2]*V1 + currentL.color.c[2]*Cs->c[2]*V2;
        break;
        
      case LightSpot:
        break;
    }
  }

  c->c[0] = r;
  c->c[1] = g;
  c->c[2] = b;
  
}


/* calculate the diffuse lighting at a point */
void Light_diffuse( Light *l, Vector *N, Vector *V, 
                       Point *p, Color *Cb, float s,
                       int oneSided, Color *c ) {
  double V1;//, V2;
  Vector lVector;//, H;
  
  Vector_normalize(N);
  Vector_normalize(V);
  Point_normalize(p);
    
  //lVector = currentL.position minus p
  lVector.v[0] = l->position.val[0] - p->val[0];
  lVector.v[1] = l->position.val[1] - p->val[1];
  lVector.v[2] = l->position.val[2] - p->val[2];
  Vector_normalize(&lVector);
		
  V1 = Vector_dot(&lVector, N);

  //if onesided and V1<0 then dont add in
  if ((oneSided == 1) && (V1 < 0)) {
    c->c[0] = 0.0;
    c->c[1] = 0.0;
    c->c[2] = 0.0;
    return;
  }
	
  //if not onesided and V1 <0 then negate V1 and calculate V2               
  if ((oneSided == 0) && (V1 < 0)) {
	V1 = -V1;
  }

  c->c[0] = Cb->c[0] * l->color.c[0]*V1;
  c->c[1] = Cb->c[1] * l->color.c[1]*V1;
  c->c[2] = Cb->c[2] * l->color.c[2]*V1;
  
}










