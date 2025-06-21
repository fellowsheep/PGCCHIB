#pragma once
#include <cmath>

#define PI 3.141592653589793

float length(float *v);
float length2D(float *v);
void normalise(float *vn);
void normalise2D(float *vn);
float dot2D(float *a, float *b);
float dot(float *a, float *b);
float* cross(float *a, float *b);
float triangleArea2D(float *triangle);
bool triangleCollidePoint2D(float *triangle, float *point);
bool collideByDotProduct(float *triangle, float *point);