#include "rotate.h"

#include <math.h>

// Degrees to radians, and the 30 degree angles of an equilateral triangle.
static const float c_radians = 3.14159265358979323846f / 180.0f;
static const float c_sin_30 = 0.5f;
static const float c_cos_30 = 0.866f;
// The circumradius of an equilateral triangle is its side times this.
static const float c_radius_per_side = 0.57735f;

// rotation of a point around the X axis
void rotate_point_around_x(float angle, float *y, float *z) {
    const float radians = c_radians * angle;
    const float cosTheta = cosf(radians);
    const float sinTheta = sinf(radians);

    const float new_y = *y * cosTheta - *z * sinTheta;
    const float new_z = *y * sinTheta + *z * cosTheta;

    *y = new_y;
    *z = new_z;
}
// rotation of a point around the Y axis
void rotate_point_around_y(float angle, float *x, float *z) {
    const float radians = c_radians * angle;
    const float cosTheta = cosf(radians);
    const float sinTheta = sinf(radians);

    const float new_x = *x * cosTheta + *z * sinTheta;
    const float new_z = -*x * sinTheta + *z * cosTheta;

    *x = new_x;
    *z = new_z;
}
// rotation of a point around the Z axis
void rotate_point_around_z(float angle, float *x, float *y) {
    const float radians = c_radians * angle;
    const float cosTheta = cosf(radians);
    const float sinTheta = sinf(radians);

    const float new_x = *x * cosTheta - *y * sinTheta;
    const float new_y = *x * sinTheta + *y * cosTheta;

    *x = new_x;
    *y = new_y;
}

// rotation of a point towards a direction
void rotate_by_diraction(float angle_a, float angle_b, float *x, float *y, float *z) {
    // around Z by -angle_a: the axes turn to x' and y'
    rotate_point_around_z(-angle_a, x, y);

    // around Y by angle_b: the point is lowered or raised
    rotate_point_around_y(angle_b, x, z);

    // around Z by angle_a: the axes turn back to x and y
    rotate_point_around_z(angle_a, x, y);
}

void set_vector(struct Vector *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void rotate_vector_around_x(float angle, struct Vector *v) { rotate_point_around_x(angle, &v->y, &v->z); }
void rotate_vector_around_y(float angle, struct Vector *v) { rotate_point_around_y(angle, &v->x, &v->z); }
void rotate_vector_around_z(float angle, struct Vector *v) { rotate_point_around_z(angle, &v->x, &v->y); }

void rotate_triabgle_around_x(float angle, struct Triangle *t) {
    rotate_vector_around_x(angle, &t->v1);
    rotate_vector_around_x(angle, &t->v2);
    rotate_vector_around_x(angle, &t->v3);
}
void rotate_triabgle_around_y(float angle, struct Triangle *t) {
    rotate_vector_around_y(angle, &t->v1);
    rotate_vector_around_y(angle, &t->v2);
    rotate_vector_around_y(angle, &t->v3);
}
void rotate_triabgle_around_z(float angle, struct Triangle *t) {
    rotate_vector_around_z(angle, &t->v1);
    rotate_vector_around_z(angle, &t->v2);
    rotate_vector_around_z(angle, &t->v3);
}

void rotate_triangle_by_direction(float angle_a, float angle_b, struct Triangle *t) {
    rotate_by_diraction(angle_a, angle_b, &t->v1.x, &t->v1.y, &t->v1.z);
    rotate_by_diraction(angle_a, angle_b, &t->v2.x, &t->v2.y, &t->v2.z);
    rotate_by_diraction(angle_a, angle_b, &t->v3.x, &t->v3.y, &t->v3.z);
}

struct Triangle calculate_triangle_points_by_side(uint8_t side) {
    const float radius = (float)side * c_radius_per_side;

    struct Triangle t;
    // the first vertex on the X axis, the other two 120 degrees either side of it, all at z = 0
    set_vector(&t.v1, radius, 0.0f, 0.0f);
    set_vector(&t.v2, -radius * c_sin_30, radius * c_cos_30, 0.0f);
    set_vector(&t.v3, -radius * c_sin_30, -radius * c_cos_30, 0.0f);
    return t;
}

void get_heights(struct Triangle *t, float *h1, float *h2, float *h3) {
    *h1 = t->v1.z;
    *h2 = t->v2.z;
    *h3 = t->v3.z;
}
