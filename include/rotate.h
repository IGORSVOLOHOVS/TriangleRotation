#pragma once
#include <math.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846
#define SIN_30 0.5
#define COS_30 0.866
#define R_CONST 0.57735

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


void set_vector(struct Vector *v, float x, float y, float z);

void rotate_point_around_x(float angle, float *y, float *z);
void rotate_point_around_y(float angle, float *x, float *z);
void rotate_point_around_z(float angle, float *x, float *y);

void rotate_vector_around_x(float angle, struct Vector *v);
void rotate_vector_around_y(float angle, struct Vector *v);
void rotate_vector_around_z(float angle, struct Vector *v);

void rotate_triabgle_around_x(float angle, struct Triangle *t);
void rotate_triabgle_around_y(float angle, struct Triangle *t);
void rotate_triabgle_around_z(float angle, struct Triangle *t);

void rotate_by_diraction(float angle_a, float angle_b, float *x, float *y, float *z);
void rotate_triangle_by_direction(float angle_a, float angle_b, struct Triangle *t);

struct Triangle calculate_triangle_points_by_side(uint8_t side);

void get_heights(struct Triangle *t, float *h1, float *h2, float *h3);

