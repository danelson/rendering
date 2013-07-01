// Dan Nelson and Hieu Phan
// Project 8
// November 15, 2011
// cb_lighting.h

#ifndef CB_LIGHTING_H

#define CB_LIGHTING_H


#define MAX_LIGHTS 64

typedef enum {
  LightNone,
  LightAmbient,
  LightDirect,
  LightPoint,
  LightSpot,
} LightType;


typedef struct {
  LightType type;
  Color color;
  Vector direction;
  Point position;
  float cutoff;  // stores the cosine of the cutoff angle of a spotlight
  float sharpness;  // coefficient of the falloff function (power for cosine)
} Light;

// Lighting struct
typedef struct {
  int nLights;
  Light light[MAX_LIGHTS];
} Lighting;

void Light_init( Light *light);
void Light_copy( Light *to, Light *from);
Lighting *Lighting_create( void);
void Lighting_init( Lighting *l);
void Lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness);
void Lighting_shading( Lighting *l, Vector *N, Vector *V, 
                       Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c);
void Light_diffuse( Light *l, Vector *N, Vector *V, 
                       Point *p, Color *Cb, float s,
                       int oneSided, Color *c );                     
                       
#endif
