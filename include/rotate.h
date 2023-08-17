#pragma once
#include <math.h>

#define M_PI 3.14159265358979323846

#define X 0
#define Y 1
#define Z 2

#define V1 0
#define V2 1
#define V3 2

struct Vector{
    float x;
    float y;
    float z;
};

// Вектора строятся относительно центра координат
struct Triangle{
    struct Vector v1;
    struct Vector v2;
    struct Vector v3;
};

struct Direction;

void set_vector(struct Vector *v, float x, float y, float z);

void rotate_around_x(float angle, float *y, float *z);
void rotate_around_y(float angle, float *x, float *z);
void rotate_around_z(float angle, float *x, float *y);

//void rotate_direction(struct Triangle* triangle, struct Direction *direction);


