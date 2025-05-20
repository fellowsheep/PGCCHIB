#include <math.h>
#include <iostream>

#define PI 3.141592653589793

using namespace std;

float length (float *v) {
    return sqrt (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float length2D (float *v) {
    return sqrt (v[0] * v[0] + v[1] * v[1]);
}

void normalise (float *vn) {
    float l = length (vn);
    if (0.0f == l) {
        vn[0] = vn[1] = vn[2] = 0;
        return;
    }
    vn[0] = vn[0] / l;
    vn[1] = vn[1] / l;
    vn[2] = vn[2] / l;
    return;
}

void normalise2D (float *vn) {
    float l = length2D(vn);
    if (0.0f == l) {
        vn[0] = vn[1] = 0;
        return;
    }
    vn[0] = vn[0] / l;
    vn[1] = vn[1] / l;
    return;
}

float dot2D (float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1];
}

float dot (float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float * cross (float *a, float *b) {
    float x = a[1] * b[2] - a[2] * b[1];
    float y = a[2] * b[0] - a[0] * b[2];
    float z = a[0] * b[1] - a[1] * b[0];
    float v[] = {x, y, z};
    return v;
}





// t={p1x, p1y,  p2x, p2y, p3x, p3y }
float triangleArea2D(float *triangle){
    return fabs(((triangle[2] - triangle[0])*(triangle[5] - triangle[1]) - (triangle[4] - triangle[0]) * (triangle[3] - triangle[1]))/2);
}

// tests: triangle area X point--sub-triangles areas
bool triangleCollidePoint2D(float *triangle, float *point){
	float a = triangleArea2D(triangle);

    float subtri1[] = {triangle[0], triangle[1], triangle[2], triangle[3], point[0], point[1]};
    float subtri2[] = {triangle[0], triangle[1], point[0], point[1], triangle[4], triangle[5]};
    float subtri3[] = {point[0], point[1], triangle[2], triangle[3], triangle[4], triangle[5]};
    
	float a1 = triangleArea2D(subtri1);
	float a2 = triangleArea2D(subtri2);
	float a3 = triangleArea2D(subtri3);

    // cout << "\tDEBUG => a: " << a << " a1: " << a1 << " + a2: " << a2 << " + a3: " << a3 << " = " << (a1+a2+a3) << endl;
	
	return a == (a1+a2+a3);
}

bool collideByDotProduct(float *triangle, float *point){
    float ab[] = {triangle[2] - triangle[0], triangle[3] - triangle[1]};
    normalise2D(ab);
    float ac[] = {triangle[4] - triangle[0], triangle[5] - triangle[1]};
    normalise2D(ac);
    
    float a_bc = acos(dot2D(ab, ac)) / PI * 180.0f;
    
    float ap[] = {point[0] - triangle[0], point[1] - triangle[1]};
    normalise2D(ap);
    
    float a_pb = acos(dot2D(ap, ab)) / PI * 180.0f;
    float a_cp = acos(dot2D(ac, ap)) / PI * 180.0f;
    
    // cout << "\tDEBUG => DOT A_BC=" << a_bc << " A_CP=" << a_cp << " A_PB=" << a_pb << endl;
    
    return (a_bc > a_cp) && (a_bc > a_pb);
}


