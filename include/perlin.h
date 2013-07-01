// Dan Nelson and Hieu Phan
// Project 2
// September 19, 2011
// perlin.h

#ifndef PERLIN_H

#define PERLIN_H


double find_noise(double x,double y);

double interpolate(double a,double b,double x);

double noise(double x,double y);

Image *Image_perlin(int rows, int cols, double zoom, double p, int octaves);


#endif