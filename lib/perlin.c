/* Dan Nelson
 * Graphics Package
 * perlin.c
 * Creates perlin noise
 */


#include <math.h>
#include <time.h>
#include "cb_image.h"
#include "perlin.h"

// Use this for a random number generator because I don't know how to seed
// C's random function with a double
double find_noise(double x,double y)
{
  int n = (int)x+(int)y*57;
  n = (n<<13)^n;
  double nn = (n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
  return 1.0-((double)nn/1073741824.0);
}

// Cosine interpolation
double interpolate(double a, double b, double x)
{
  double ft = (double) x * 3.1415927;
  double f = (1.0-cos(ft))* 0.5;
  return a*(1.0-f)+b*f;
}

// Creates noise in the image
double noise(double x,double y)
{
  double floorx, floory;
  double s,t,u,v;
  double interp1, interp2;
  
  floorx = (double)((int)x);
  floory = (double)((int)y);

  // Check the surrounding pixels
  s = find_noise(floorx,floory);
  t = find_noise(floorx+1,floory);
  u = find_noise(floorx,floory+1);
  v = find_noise(floorx+1,floory+1);
  
  // Interpolate between the values in 2 dimension
  interp1 = interpolate(s,t,x-floorx);
  interp2 = interpolate(u,v,x-floorx);
  return interpolate(interp1,interp2,y-floory);
}

// Creates perlin noise
// Zoom usually is equal to  75
// P is usually equal to 0.5
// Ocatves is usually from 2-6
Image *Image_perlin(int rows, int cols, double zoom, double p, int octaves) {
  double color;
  double frequency,amplitude;
  int i,j,k;
  
  // create an empty image
  Image *image;
  image = Image_create(rows,cols);
  
  // loop trough all the pixels
  for(i=0;i<rows;i++) {
    for(j=0;j<cols;j++) {
      color = 0;
	  for(k=0;k<octaves;k++) {
		  frequency = pow(2,k);
		  amplitude = pow(p,k);
	      color += noise(((double)i)*frequency/zoom,((double)j)/zoom*frequency)*amplitude;
	  }
      // scale the color between 0 and 255
	  color = (color*128)+128;
	  
	  // make sure the color is a legal value
      if(color > 255) {
        color = 255;
      }
      if(color < 0) {
        color = 0;
      }
            
      // this colors the image with the RGB values given at the beginning in the function.
      FPixel p;
      p.rgb[0] = (double) (color / 255.0);
      p.rgb[1] = (double) (color / 255.0);
      p.rgb[2] = (double) (color / 255.0);
      Image_setf(image, i, j, p);      
    }
  }

  return image;
}