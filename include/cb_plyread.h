#ifndef CB_PLYREAD_H

#define CB_PLYREAD_H

typedef enum {
	type_float32,
	type_uint8,
	type_int32,
	type_list,
	type_none
} ply_type;

typedef struct {
	ply_type type;
	ply_type listCardType;
	ply_type listDataType;
	char name[32];
	void *next;
} ply_property;

int readPLY(char filename[], int *nPolygons, Polygon **plist, Color **clist, int estNormals);


#endif
