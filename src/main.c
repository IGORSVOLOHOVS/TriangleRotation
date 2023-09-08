#include "rotate.h"
#include "test.h"
#include <stdio.h>

int main()
{
    struct Triangle t = calculate_triangle_points_by_side(3);

    printf("Before rotation:\n");
    printf("v1: %f %f %f\n", t.v1.x, t.v1.y, t.v1.z);
    printf("v2: %f %f %f\n", t.v2.x, t.v2.y, t.v2.z);
    printf("v3: %f %f %f\n", t.v3.x, t.v3.y, t.v3.z);

    rotate_triangle_by_direction(90, 90, &t);

    printf("After rotation:\n");
    printf("v1: %f %f %f\n", t.v1.x, t.v1.y, t.v1.z);
    printf("v2: %f %f %f\n", t.v2.x, t.v2.y, t.v2.z);
    printf("v3: %f %f %f\n", t.v3.x, t.v3.y, t.v3.z);

    float h1, h2, h3;
    get_heights(&t, &h1, &h2, &h3);

    printf("Heights:\n");
    printf("h1: %f\n", h1);
    printf("h2: %f\n", h2);
    printf("h3: %f\n", h3);

    return 0;
}