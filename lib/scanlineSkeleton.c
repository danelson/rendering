/*
  Bruce A. Maxwell
  Fall 2011

  polygon skeleton

*/


#include "cb_graphics.h"


typedef struct tEdge {
  int yUpper;
  Color cIntersect, dcPerScan;
  Point pIntersect, dpPerScan;
  float xIntersect, dxPerScan, zIntersect, dzPerScan
    , sIntersect, dsPerScan, tIntersect, dtPerScan;
  Vector nIntersect, dnPerScan;
  struct tEdge *next;
  TextureCoord texCoord[4];
  Texture tex;
} Edge;


/********************
Scanline Fill Algorithm
********************/

// sorted linked list insert
void insertEdge(Edge *list, Edge *edge);
void insertEdge(Edge *list, Edge *edge) {
  Edge *p, *q = list;

  p = q->next;
  while(p != NULL) {
    if(edge->xIntersect < p->xIntersect)
      p = NULL;
    else {
      q = p;
      p = p->next;
    }
  }
  edge->next = q ->next;
  q->next = edge;
}


// lower is the point with the smaller y value
// upper is the point with the greater y value
// draw from lower to upper
void makeEdgeRec(Point lower, Point upper, Color c1, Color c2, Vector n1, Vector n2, 
  Point p1, Point p2, float t1, float t2, float s1, float s2, Edge *edge, Edge *edges[], Image *src);
void makeEdgeRec(Point lower, Point upper, Color c1, Color c2, Vector n1, Vector n2, 
  Point p1, Point p2, float t1, float t2, float s1, float s2, Edge *edge, Edge *edges[], Image *src)
{
  int startRow = (int)(lower.val[1] + 0.5 ); // round the incoming point values
  int endRow = (int)(upper.val[1] + 0.5 );   // round

  float x1,y1,x2,y2,z1,z2,dscan;
  float d;
  
  x1 = lower.val[0];
  y1 = lower.val[1];
  z1 = lower.val[2];
  x2 = upper.val[0];
  y2 = upper.val[1];
  z2 = upper.val[2];

  
  if ( (y1 < 0) || (y2 > src->rows - 1) ) { 
    free(edge);
    return;
  }
  
  dscan = y2-y1;
  edge->dxPerScan = (x2-x1)/dscan;
  edge->dzPerScan = (1.0/z2-1.0/z1)/dscan;


    // calculate the initial cIntersect as the initial color value 
    // divided by the initial z value. In your fillScan function
    // you will need to multiply c/z by z to get the true color value.
    if (z1 != 0) {
      edge->cIntersect.c[0] = c1.c[0]/z1;
      edge->cIntersect.c[1] = c1.c[1]/z1;
      edge->cIntersect.c[2] = c1.c[2]/z1;
      
      edge->nIntersect.v[0] = n1.v[0]/z1;
      edge->nIntersect.v[1] = n1.v[1]/z1;
      edge->nIntersect.v[2] = n1.v[2]/z1;
      
      edge->pIntersect.val[0] = p1.val[0]/z1;
      edge->pIntersect.val[1] = p1.val[1]/z1;
      edge->pIntersect.val[2] = p1.val[2]/z1;
      edge->pIntersect.val[3] = p1.val[3]/z1;
      
      edge->sIntersect = s1/z1;
      edge->tIntersect = t1/z1;
    }
    else {
      edge->cIntersect.c[0] = c1.c[0];
      edge->cIntersect.c[1] = c1.c[1];
      edge->cIntersect.c[2] = c1.c[2];
          
      edge->nIntersect.v[0] = n1.v[0];
      edge->nIntersect.v[1] = n1.v[1];
      edge->nIntersect.v[2] = n1.v[2];
                
      edge->pIntersect.val[0] = p1.val[0];
      edge->pIntersect.val[1] = p1.val[1];
      edge->pIntersect.val[2] = p1.val[2];
      edge->pIntersect.val[3] = p1.val[3];
    }

    
    if (z1 != 0 && z2 != 0) {
      // To account for perspective projection, calculate dcPerScan 
      // using the color value divided by the depth value
      edge->dcPerScan.c[0] = ( c2.c[0]/z2 - c1.c[0]/z1 ) / dscan; 
      edge->dcPerScan.c[1] = ( c2.c[1]/z2 - c1.c[1]/z1 ) / dscan;
      edge->dcPerScan.c[2] = ( c2.c[2]/z2 - c1.c[2]/z1 ) / dscan;
      
      edge->dnPerScan.v[0] = ( n2.v[0]/z2 - n1.v[0]/z1 ) / dscan; 
      edge->dnPerScan.v[1] = ( n2.v[1]/z2 - n1.v[1]/z1 ) / dscan;
      edge->dnPerScan.v[2] = ( n2.v[2]/z2 - n1.v[2]/z1 ) / dscan;
      
      edge->dpPerScan.val[0] = ( p2.val[0]/z2 - p1.val[0]/z1 ) / dscan; 
      edge->dpPerScan.val[1] = ( p2.val[1]/z2 - p1.val[1]/z1 ) / dscan;
      edge->dpPerScan.val[2] = ( p2.val[2]/z2 - p1.val[2]/z1 ) / dscan;
      edge->dpPerScan.val[3] = ( p2.val[3]/z2 - p1.val[3]/z1 ) / dscan;
      
      edge->dsPerScan = (s2/z2 - s1/z1)/dscan;
      edge->dtPerScan = (t2/z2 - t1/z1)/dscan;
    }
    else {
      edge->dcPerScan.c[0] = ( c2.c[0]- c1.c[0]) / dscan; 
      edge->dcPerScan.c[1] = ( c2.c[1]- c1.c[1]) / dscan;
      edge->dcPerScan.c[2] = ( c2.c[2] - c1.c[2]) / dscan;
      
      edge->dnPerScan.v[0] = ( n2.v[0]- n1.v[0]) / dscan; 
      edge->dnPerScan.v[1] = ( n2.v[1]- n1.v[1]) / dscan;
      edge->dnPerScan.v[2] = ( n2.v[2] - n1.v[2]) / dscan;
      
      edge->dpPerScan.val[0] = ( p2.val[0] - p1.val[0] ) / dscan; 
      edge->dpPerScan.val[1] = ( p2.val[1] - p1.val[1] ) / dscan;
      edge->dpPerScan.val[2] = ( p2.val[2] - p1.val[2] ) / dscan;
      edge->dpPerScan.val[3] = ( p2.val[3] - p1.val[3] ) / dscan;
    }

  
  // Floating point version
  // if y is in the upper half of the pixel
  if ( (floor(y1) <= y1) && (y1 <= (floor(y1) + 0.5)) ) {
    d = floor(y1) + 0.5 - y1;
    edge->xIntersect = x1 + d*edge->dxPerScan;
    edge->zIntersect = 1.0/z1 + d*edge->dzPerScan;
    edge->sIntersect += d*edge->dsPerScan;
    edge->tIntersect += d*edge->dsPerScan;
    
    edge->cIntersect.c[0] += d*edge->dcPerScan.c[0];
    edge->cIntersect.c[1] += d*edge->dcPerScan.c[1];
    edge->cIntersect.c[2] += d*edge->dcPerScan.c[2];
    
    edge->nIntersect.v[0] += d*edge->dnPerScan.v[0];
    edge->nIntersect.v[1] += d*edge->dnPerScan.v[1];
    edge->nIntersect.v[2] += d*edge->dnPerScan.v[2];
    
    // check this!!!!!
    edge->pIntersect.val[0] += d*edge->dpPerScan.val[0];
    edge->pIntersect.val[1] += d*edge->dpPerScan.val[1];
    edge->pIntersect.val[2] += d*edge->dpPerScan.val[2];
    edge->pIntersect.val[3] += d*edge->dpPerScan.val[3];
  }
  // if y is in the lower half of the image
  else {
    d = ceil(y1) - y1;
    edge->xIntersect = x1 + (d+0.5)*edge->dxPerScan;
    edge->zIntersect = 1.0/z1 + (d+0.5)*edge->dzPerScan;
    edge->sIntersect += (d+0.5)*edge->dsPerScan;
    edge->tIntersect += (d+0.5)*edge->dtPerScan;
    
    edge->cIntersect.c[0] += (d+0.5)*edge->dcPerScan.c[0];
    edge->cIntersect.c[1] += (d+0.5)*edge->dcPerScan.c[1];
    edge->cIntersect.c[2] += (d+0.5)*edge->dcPerScan.c[2];
    
    edge->nIntersect.v[0] += (d+0.5)*edge->dnPerScan.v[0];
    edge->nIntersect.v[1] += (d+0.5)*edge->dnPerScan.v[1];
    edge->nIntersect.v[2] += (d+0.5)*edge->dnPerScan.v[2];
    
    edge->pIntersect.val[0] += (d+0.5)*edge->dpPerScan.val[0];
    edge->pIntersect.val[1] += (d+0.5)*edge->dpPerScan.val[1];
    edge->pIntersect.val[2] += (d+0.5)*edge->dpPerScan.val[2];
    edge->pIntersect.val[3] += (d+0.5)*edge->dpPerScan.val[3];

  }
  
  // Clip to the top edge and adjust lower.row and xIntersect as appropriate
  if ( y1 < 0 ) {
	edge->xIntersect += -y1*edge->dxPerScan;
	edge->zIntersect += -y1*edge->dzPerScan;
	edge->sIntersect += -y1*edge->dsPerScan;
	edge->tIntersect += -y1*edge->dtPerScan;
	
	edge->cIntersect.c[0] += -y1*edge->dcPerScan.c[0];
    edge->cIntersect.c[1] += -y1*edge->dcPerScan.c[1];
    edge->cIntersect.c[2] += -y1*edge->dcPerScan.c[2];
    
    edge->nIntersect.v[0] += -y1*edge->dnPerScan.v[0];
    edge->nIntersect.v[1] += -y1*edge->dnPerScan.v[1];
    edge->nIntersect.v[2] += -y1*edge->dnPerScan.v[2];
    
    edge->pIntersect.val[0] += -y1*edge->dpPerScan.val[0];
    edge->pIntersect.val[1] += -y1*edge->dpPerScan.val[1];
    edge->pIntersect.val[2] += -y1*edge->dpPerScan.val[2];
    edge->pIntersect.val[3] += -y1*edge->dpPerScan.val[3];

    
    lower.val[1] = 0;
  }
  // do i need this test (clipping to bottom edge)??? it might be handled by yUpper???
  if (y1 > src->rows - 1) {
    upper.val[1] = src->rows - 1;
  }
  
  // Set yUpper to endRow-1 and check it against the bottom of the image
  edge->yUpper = endRow - 1;
  
  if (edge->yUpper > src->rows-1) {
    edge->yUpper = src->rows-1;
  }
  
  // insert the edge
  insertEdge(edges[startRow], edge);

  return;
}


// builds the edge list by going over every pair of points
void buildEdgeList(Polygon *p, Edge *edges[], Image *src, int *min, int *max);
void buildEdgeList(Polygon *p, Edge *edges[], Image *src, int *min, int *max) {
	Edge *edge;
	Point v1, v2;
	Color c1, c2;
	Vector n1, n2;
	Point p1, p2;
	Color defaultC = {{0.0,0.0,0.0}};
	Vector defaultN = {{0.0,0.0,0.0,0.0}};
	Point defaultP = {{0.0,0.0,0.0,0.0}};
	TextureCoord defaultTC;
	TextureCoord tc1, tc2;
	int i;
	
	defaultTC.t = 0;
	defaultTC.s = 0;

	/* Initialize the process with the last point (connects to the first point) */
	v1 = p->vertex[p->nVertex-1];
	if (p->color != NULL)
	  c1 = p->color[p->nVertex-1];
	else
	  c1 = defaultC;
	
	//printf("anchor 1 \n");
	if (p->wNormal != NULL)
	  n1 = p->wNormal[p->nVertex-1];
	else
	  n1 = defaultN;
	
	//printf("anchor 2 \n");
	if (p->wVertex != NULL)
	  p1 = p->wVertex[p->nVertex-1];
	else
	  p1 = defaultP;
	  
	if (p->texCoord != NULL)
	  tc1 = p->texCoord[p->nVertex-1];
	else
	  tc1 = defaultTC;
	  
	
	//printf("IN BUILD EDGE LIST ...\n");
	//Color_print(&c1,stdout);

	// min and max rows
	*min = (int)v1.val[1];
	*max = (int)v1.val[1] + 1;

	/* for each line segment */
	for(i=0;i<p->nVertex;i++) {

		/* end of the segment is the current point */
		v2 = p->vertex[i];
		
		if (p->color != NULL)
		  c2 = p->color[i];
		else
	      c2 = defaultC;
	    
	    if (p->wNormal != NULL)
	      n2 = p->wNormal[i];
	    else
	      n2 = defaultN;
	      
	    if (p->wVertex != NULL)
	      p2 = p->wVertex[i];
	    else
	      p2 = defaultP;

		if (p->texCoord != NULL)
	      tc2 = p->texCoord[i];
	    else
	      tc2 = defaultTC;

		*min = (int)v2.val[1] < *min ? (int)v2.val[1] : *min;
		*max = (int)v2.val[1] > *max ? (int)v2.val[1] : *max;
		
		if((int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5)) { /* not a horizontal line */

			/* create a new edge with v1.row less than v2.row */
			edge = (Edge *)malloc(sizeof(Edge));
			if(v1.val[1] < v2.val[1]){ 
				makeEdgeRec(v1, v2, c1, c2, n1, n2, p1, p2,
							tc1.t, tc2.t, tc1.s, tc2.s, edge, edges, src);
			}
			else
				makeEdgeRec(v2, v1, c2, c1, n2, n1, p2, p1,
							tc2.t, tc1.t, tc2.s, tc1.s, edge, edges, src);
		}
		v1 = v2;
		c1 = c2;
		n1 = n2;
		p1 = p2;
		tc1 = tc2;
	}

	// clip to the image
	*min = *min < 0 ? 0 : *min;
	*max = *max >= src->rows ? src->rows-1 : *max;

	return;
}


void buildActiveList(int scan, Edge *active, Edge *edges[]);
void buildActiveList(int scan, Edge *active, Edge *edges[]) {
  Edge *p, *q;

  p = edges[scan]->next;
  while(p) {
    q = p->next;
    insertEdge(active, p);
    p = q;
  }
}

// Given a list of edges (active) fill in the scanline (scan is the row)
void fillScan(int scan, Edge *active, DrawState* ds, int oneSided, Lighting* light, Image *src);
void fillScan(int scan, Edge *active, DrawState* ds, int oneSided, Lighting* light,  Image *src) {
  Edge *p1, *p2;
  int i;
  float curZ, dzPerColumn, curS, curT, dsPerCol, dtPerCol, dsPerY, dtPerY;
  Color curC, dcPerColumn;
  Vector curN, dnPerColumn;
  Point curV, dvPerColumn;
  Color tColor = {{0.0,0.0,0.0}};
  p1 = active->next;
  while(p1) {
    p2 = p1->next;

    if( !p2 ) {
	    break;
    }
    
    // Calculate the starting column (p1), ending column (p2), and fill in the row
    // Be sure to check for the image boundaries.
    int start = (int) (p1->xIntersect);
    int end = (int) (p2->xIntersect);  // ????
    curZ = p1->zIntersect;
    curS = p1->sIntersect;
    curT = p1->tIntersect;

    dzPerColumn = (p1->zIntersect - p2->zIntersect)/(start-end);
    dsPerCol = (p1->sIntersect - p2->sIntersect)/(start-end);
    dtPerCol = (p1->tIntersect - p2->tIntersect)/(start-end);
    
    // update dsPerCol and dtPerCol to avoid aliasing
    // should we use p1->dsPerScan or p2->dsPerScan???
    dsPerY = p1->dsPerScan - (p1->dxPerScan * dsPerCol);
    dtPerY = p1->dtPerScan - (p1->dxPerScan * dtPerCol);
    
    curC = p1->cIntersect;
    dcPerColumn.c[0] = (p1->cIntersect.c[0] - p2->cIntersect.c[0])/((float)(start-end));
    dcPerColumn.c[1] = (p1->cIntersect.c[1] - p2->cIntersect.c[1])/((float)(start-end));
    dcPerColumn.c[2] = (p1->cIntersect.c[2] - p2->cIntersect.c[2])/((float)(start-end));

    curN = p1->nIntersect;
    dnPerColumn.v[0] = (p1->nIntersect.v[0] - p2->nIntersect.v[0])/((float)(start-end));
    dnPerColumn.v[1] = (p1->nIntersect.v[1] - p2->nIntersect.v[1])/((float)(start-end));
    dnPerColumn.v[2] = (p1->nIntersect.v[2] - p2->nIntersect.v[2])/((float)(start-end));    
    
    curV = p1->pIntersect;
    dvPerColumn.val[0] = (p1->pIntersect.val[0] - p2->pIntersect.val[0])/((float)(start-end));
    dvPerColumn.val[1] = (p1->pIntersect.val[1] - p2->pIntersect.val[1])/((float)(start-end));
    dvPerColumn.val[2] = (p1->pIntersect.val[2] - p2->pIntersect.val[2])/((float)(start-end)); 
    dvPerColumn.val[3] = (p1->pIntersect.val[3] - p2->pIntersect.val[3])/((float)(start-end)); 


    for (i=start; i<end; i++) {
      if (i >= src->cols) {
      }
      else if (i < 0) {
      }
      else {
		  /********* USING Z BUFFER ***************************/
		  if (ds->zBufferFlag == 1) {
			//if the current 1/z value > the current z-buffer value at (row, col)
			if (curZ > Image_getz(src, scan, i)) {
			  if (ds->tex != NULL) {
			    //printf("anchor 4 %lf %lf\n",curS, curT);    
			    Point corner[4];
			    Point_set2D(&(corner[0]),curS/curZ, curT/curZ);
			    Point_set2D(&(corner[1]),curS/curZ + dsPerY/curZ, curT/curZ + dtPerY/curZ);
			    Point_set2D(&(corner[2]),curS/curZ + dsPerCol/curZ + dsPerY/curZ, curT/curZ + dtPerCol/curZ + dtPerY/curZ);
			    Point_set2D(&(corner[3]),curS/curZ + dsPerCol/curZ, curT/curZ + dtPerCol/curZ);
			    
			    tColor = Texture_value(ds->tex,corner);
			    //Color_print(&(tColor),stdout);
			  }
			  //update the current z-buffer value to the current 1/z value
			  Image_setz(src,scan,i,curZ);
			  //draw the current color into the image at (row, col)
			  if (ds->shade == ShadeDepth) {
			    float red = ds->color.c[0];
				float green = ds->color.c[1];
				float blue = ds->color.c[2];
				Color newColor = {{(1-1.0/curZ)*red, (1-1.0/curZ)*green, (1-1.0/curZ)*blue}};
				Image_setColor(src, scan, i, newColor);
			  }
			  // for now: (ds->shade == ShadeConstant)
			  else if (ds->shade == ShadeConstant) {
				Image_setColor(src, scan, i, ds->color);
			  }
			  else if (ds->shade == ShadeFlat) {
			    Image_setColor(src, scan, i, ds->flatColor);
			  }
			  // grow shading!
			  else if (ds->shade == ShadeGouraud) {
			    Color newColor = {{curC.c[0] * 1/curZ, curC.c[1] * 1/curZ, curC.c[2] * 1/curZ}};
				if (ds->tex != NULL) {
                  newColor.c[0] = newColor.c[0] * tColor.c[0];
                  newColor.c[1] = newColor.c[1] * tColor.c[1];
                  newColor.c[2] = newColor.c[2] * tColor.c[2];
                }
				Image_setColor(src, scan, i, newColor);
			  }
			  else if (ds->shade == ShadePhong) {
			    Color newColor;
				Vector view;
				Vector tempCurN;
				Point tempCurV;
				
				tempCurV.val[0] = curV.val[0]/curZ;
				tempCurV.val[1] = curV.val[1]/curZ;
				tempCurV.val[2] = curV.val[2]/curZ;
				tempCurV.val[3] = curV.val[3]/curZ;
				
				tempCurN.v[0] = curN.v[0]/curZ;
				tempCurN.v[1] = curN.v[1]/curZ;
				tempCurN.v[2] = curN.v[2]/curZ;

				view.v[0] = - tempCurV.val[0]  + ds->viewer.val[0]; 
      			view.v[1] = - tempCurV.val[1] + ds->viewer.val[1];
      			view.v[2] = - tempCurV.val[2] + ds->viewer.val[2];  			

			    Lighting_shading(light, &tempCurN, &view, 
                       &(tempCurV), &(ds->body), &(ds->surface), 32,
                       oneSided, &newColor);
                if (ds->tex != NULL) {
                  newColor.c[0] = newColor.c[0] * tColor.c[0];
                  newColor.c[1] = newColor.c[1] * tColor.c[1];
                  newColor.c[2] = newColor.c[2] * tColor.c[2];
                }               
                printf("newColor: ========\n");
                Color_print(&newColor,stdout);
                Image_setColor(src,scan,i,newColor);
			  }
			}
		  }
		  /************** NOT USING Z BUFFER********************/
		  else {		  	
		  	if (ds->shade == ShadeDepth) {
			    float red = ds->color.c[0];
				float green = ds->color.c[1];
				float blue = ds->color.c[2];
				Color newColor = {{(1-1.0/curZ)*red, (1-1.0/curZ)*green, (1-1.0/curZ)*blue}};
				Image_setColor(src, scan, i, newColor);
			  }
			  // for now: (ds->shade == ShadeConstant)
			  else if (ds->shade == ShadeConstant) {
				Image_setColor(src, scan, i, ds->color);
			  }
			  // grow shading!
			  else if (ds->shade == ShadeGouraud) {
			    //Color newColor = {{curC.c[0], curC.c[1] * curZ, curC.c[2] * curZ}};
				//Color_print(&curC,stdout);
				Image_setColor(src, scan, i, curC);
			  }
		  }
    }
    curZ += dzPerColumn;
    curC.c[0] += dcPerColumn.c[0];
    curC.c[1] += dcPerColumn.c[1];
    curC.c[2] += dcPerColumn.c[2];
    
    curN.v[0] += dnPerColumn.v[0];
    curN.v[1] += dnPerColumn.v[1];
    curN.v[2] += dnPerColumn.v[2];
    
    curV.val[0] += dvPerColumn.val[0];
    curV.val[1] += dvPerColumn.val[1];
    curV.val[2] += dvPerColumn.val[2];
    curV.val[3] += dvPerColumn.val[3];

    curS += dsPerCol;
    curT += dtPerCol;
    }
    p1 = p2->next;
  }
      
}


void deleteAfter(Edge *q);
void deleteAfter(Edge *q) {
  Edge *p = q->next;

  q->next = p->next;
  free(p);
}

void updateActiveList(int scan, Edge *active);
void updateActiveList(int scan, Edge *active) {
  Edge *q = active, *p = active->next;

  while(p) {
    /* if the edge has ended, get rid of it */
    if(scan >= p->yUpper) {
      p = p->next;
      deleteAfter(q);
    }
    /*  otherwise, update the xIntersect value */
    else {
      p->xIntersect = p->xIntersect + p->dxPerScan;
      p->zIntersect = p->zIntersect + p->dzPerScan;
      p->cIntersect.c[0] += p->dcPerScan.c[0];
      p->cIntersect.c[1] += p->dcPerScan.c[1];
      p->cIntersect.c[2] += p->dcPerScan.c[2];
      
      p->nIntersect.v[0] += p->dnPerScan.v[0];
      p->nIntersect.v[1] += p->dnPerScan.v[1];
      p->nIntersect.v[2] += p->dnPerScan.v[2];
      
      p->pIntersect.val[0] += p->dpPerScan.val[0];
      p->pIntersect.val[1] += p->dpPerScan.val[1];
      p->pIntersect.val[2] += p->dpPerScan.val[2];
      p->pIntersect.val[3] += p->dpPerScan.val[3];
      
      p->sIntersect += p->dsPerScan;
      p->tIntersect += p->dtPerScan;
      
      q = p;
      p = p->next;
    }
  }
}


void resortActiveList(Edge *active);
void resortActiveList(Edge *active) {
  Edge *q, *p = active->next;

  active->next = NULL;
  while(p) {
    q = p->next;
    insertEdge(active, p);
    p = q;
  }
}


/*
Draw a filled polygon
This is fairly wasteful of memory.
Expanding it to handling a whole array of polygons would make more sense.
 */

void Polygon_drawFill( Polygon *p, Image *src, DrawState *ds, Lighting* light ) {
  Edge **edges, *active;
  int i, scan, max, min;
  
  // allocate an array of edge pointers
  edges = (Edge **)malloc(sizeof(Edge *) * src->rows);
  if(edges == NULL) {
    printf("Allocation error\n");
    exit(0);
  }

  // allocate the head of a linked list for each row
  for(i=0;i<src->rows;i++) {
    edges[i] = (Edge *)malloc(sizeof(Edge));
    edges[i]->next = NULL;
  }

  // build the edge list
  printf("before build edge \n");
  buildEdgeList(p, edges, src, &min, &max);

  // allocate the active edge list
  active = (Edge *)malloc(sizeof(Edge));
  active->next = NULL;
  
  // go through each scanline that covers the polygons
  for(scan = min; scan < max; scan++) {
    buildActiveList(scan, active, edges);
    if(active->next) {
      printf("before fill scan \n");
      fillScan(scan, active, ds, p->oneSided, light, src);
      updateActiveList(scan, active);
      resortActiveList(active);
    }
  }

  /* free malloced edges! */
  for(i=0;i<src->rows;i++)
    free(edges[i]);

  free(active);

  free(edges);

  return;
}



/*
 * Draw a shaded polygon
 */
void Polygon_drawShade( Polygon *p, Image *src, DrawState* ds, Lighting *light) {  
  switch(ds->shade)
  {
    case ShadeFrame:
      Polygon_drawFrame(p,src,ds->color);
      break;
    
    case ShadeConstant:
      printf("Polygon_drawFill: ShadeConstant... \n");
      Polygon_drawFill(p,src,ds, light);
      break;
    
    case ShadeDepth:
      printf("Polygon_drawFill: ShadeDepth... \n");
      Polygon_drawFill(p,src,ds, light);
	  break;
    
    case ShadeFlat:
      printf("Polygon_drawFill: ShadeFlat... \n");
      Polygon_drawFill(p,src,ds, light);      
      break;
      
    case ShadeGouraud:
      printf("Polygon_drawFill: ShadeGouraud... \n");
      Polygon_drawFill(p,src,ds, light);
      printf("after draw fill \n");
      break;
      
    // fill the polygon using a Phong shading method based on the normals 
    // in the polygon data structure, the parameters in the DrawState, 
    // and the lighting in the Lighting data structure.
    case ShadePhong:
      Polygon_drawFill(p,src,ds, light);
      break;
      
    default:
      printf("shading method not supported.. \n");
      break;
  }

  return;
}
