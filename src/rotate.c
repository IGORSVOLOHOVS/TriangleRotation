#include "rotate.h"

const float c_radians = M_PI / 180.0f;

float radians = 0.0f; // Угол в радианах
float cosTheta = 0.0f; // cos(theta)
float sinTheta = 0.0f; // sin(theta)

float new_x = 0.0f;
float new_y = 0.0f;
float new_z = 0.0f;

void set_vector(struct Vector *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void rotate_around_x(float angle, float *y, float *z) {
    radians = c_radians* angle;
    
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    
    new_y = *y * cosTheta - *z * sinTheta;
    new_z = *y * sinTheta + *z * cosTheta;

    *y = new_y;
    *z = new_z;
}

void rotate_around_y(float angle, float *x, float *z) {
    radians = c_radians * angle;
    
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    
    new_x = *x * cosTheta + *z * sinTheta;
    new_z = -*x * sinTheta + *z * cosTheta;

    *x = new_x;
    *z = new_z;
}

void rotate_around_z(float angle, float *x, float *y) {
    radians = c_radians * angle;
    
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    
    new_x = *x * cosTheta - *y * sinTheta;
    new_y = *x * sinTheta + *y * cosTheta;

    *x = new_x;
    *y = new_y;
}
