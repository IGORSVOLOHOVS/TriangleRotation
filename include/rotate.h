#ifndef TRIANGLE_ROTATION_ROTATE_H
#define TRIANGLE_ROTATION_ROTATE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Vector {
    float x;
    float y;
    float z;
};

// The vertices are taken from the origin of the coordinates.
struct Triangle {
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

#ifdef __cplusplus
}
#endif

#endif  // TRIANGLE_ROTATION_ROTATE_H
