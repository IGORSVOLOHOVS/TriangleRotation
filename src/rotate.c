#include "rotate.h"


const float c_radians = M_PI / 180.0f;

// для расчетов
float radius = 0.0f; // Радиус вписанной окружности
float radians = 0.0f; // Угол в радианах
float cosTheta = 0.0f; // cos(theta)
float sinTheta = 0.0f; // sin(theta)

float new_x = 0.0f;
float new_y = 0.0f;
float new_z = 0.0f;

// поворот точки вокруг оси X
void rotate_point_around_x(float angle, float *y, float *z) {
    radians = c_radians* angle;
    
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    
    new_y = *y * cosTheta - *z * sinTheta;
    new_z = *y * sinTheta + *z * cosTheta;

    *y = new_y;
    *z = new_z;
}
// поворот точки вокруг оси Y
void rotate_point_around_y(float angle, float *x, float *z) {
    radians = c_radians * angle;
    
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    
    new_x = *x * cosTheta + *z * sinTheta;
    new_z = -*x * sinTheta + *z * cosTheta;

    *x = new_x;
    *z = new_z;
}
// поворот точки вокруг оси Z
void rotate_point_around_z(float angle, float *x, float *y) {
    radians = c_radians * angle;
    
    cosTheta = cos(radians);
    sinTheta = sin(radians);
    
    new_x = *x * cosTheta - *y * sinTheta;
    new_y = *x * sinTheta + *y * cosTheta;

    *x = new_x;
    *y = new_y;
}

// поворот точки на определенный вектор
void rotate_by_diraction(float angle_a, float angle_b, float *x, float *y, float *z) {
    // поворот вокруг оси Z на угол -angle_a (меняем ось координат на x' и y')
    rotate_point_around_z(-angle_a, x, y);

    // поворот вокруг оси Y на угол angle_b(опускаем либо поднимаем точку )
    rotate_point_around_y(angle_b, x, z);

    // поворот вокруг оси Z на угол angle_a (возвращаем ось координат на x и y)
    rotate_point_around_z(angle_a, x, y);
}

// установка вектора
void set_vector(struct Vector *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void rotate_vector_around_x(float angle, struct Vector *v) {
    rotate_point_around_x(angle, &v->y, &v->z);
}
void rotate_vector_around_y(float angle, struct Vector *v) {
    rotate_point_around_y(angle, &v->x, &v->z);
}
void rotate_vector_around_z(float angle, struct Vector *v) {
    rotate_point_around_z(angle, &v->x, &v->y);
}

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

void rotate_triangle_by_direction(float angle_a, float angle_b, struct Triangle *t){
    rotate_by_diraction(angle_a, angle_b, &t->v1.x, &t->v1.y, &t->v1.z);
    rotate_by_diraction(angle_a, angle_b, &t->v2.x, &t->v2.y, &t->v2.z);
    rotate_by_diraction(angle_a, angle_b, &t->v3.x, &t->v3.y, &t->v3.z);
}

struct Triangle calculate_triangle_points_by_side(uint8_t side){
    radius = side * R_CONST;

    struct Triangle t;

    // обнуляем вспомогательные координаты
    new_x = 0.0f;
    new_y = 0.0f;
    new_z = 0.0f;

    // вычисления для первой точки
    new_x = radius;

    set_vector(&t.v1, new_x, new_y, new_z);

    // вычисления для второй точки
    new_x = -radius * SIN_30;
    new_y = radius * COS_30;

    set_vector(&t.v2, new_x, new_y, new_z);

    // вычисления для третьей точки
    new_y = -radius * COS_30;

    set_vector(&t.v3, new_x, new_y, new_z);

    return t;
}

void get_heights(struct Triangle *t, float *h1, float *h2, float *h3){
    *h1 = t->v1.z;
    *h2 = t->v2.z;
    *h3 = t->v3.z;
}