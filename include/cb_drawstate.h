#ifndef CB_DRAWSTATE_H

#define CB_DRAWSTATE_H

typedef enum {
  ShadeFrame,
  ShadeConstant,
  ShadeFlat,
  ShadeGouraud,
  ShadePhong,
  ShadeDepth
} ShadeMethod;


typedef enum {
  TextureColor,
  TextureNormal,
  TextureHeight
} TextureType;


// Texture structure
typedef struct {
  Image *img;
  TextureType textureType;
} Texture;
  

// TextureCoord structure
typedef struct {
  float s;
  float t;
} TextureCoord;




// DrawState structure
typedef struct {
  Color color;
  Color flatColor;
  Color body;
  Color surface;
  float surfaceCoeff;
  ShadeMethod shade;
  int zBufferFlag;
  Point viewer;
  Texture *tex;
} DrawState;





/*******************
*     DrawState    *
********************/

DrawState *DrawState_create(void);
void DrawState_setColor(DrawState *s, Color c);
void DrawState_setBody(DrawState *s, Color c);
void DrawState_setSurface(DrawState *s, Color c);
void DrawState_setSurfaceCoeff(DrawState *s, float f);
void DrawState_copy(DrawState *to, DrawState *from);

/********************
*     Texture       *
*********************/
Texture *Texture_create(char *filename, TextureType textureType);
Color Texture_value(Texture *tex, Point corner[4]);


#endif
