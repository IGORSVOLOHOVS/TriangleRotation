# Manual

## The demo

`.\recipe.ps1 run` builds `triangle_rotation` and runs it. It takes no argument. It places a triangle of side 3 around the origin, turns it with `rotate_triangle_by_direction(90, 90, ...)`, and prints the vertices before and after and the three heights.

## The functions

Angles are in degrees. Every function changes what its pointers point to and keeps no state of its own, so calls do not affect one another.

```c
#include "rotate.h"

float y = 1.0f, z = 0.0f;
rotate_point_around_x(90.0f, &y, &z);
// y is 0 and z is 1: a quarter turn around X takes Y to Z.

struct Vector v;
set_vector(&v, 1.0f, 0.0f, 5.0f);
rotate_vector_around_z(90.0f, &v);
// v is (0, 1, 5): the coordinate along the axis stays.

struct Triangle t = calculate_triangle_points_by_side(3);
// v1 is (1.732, 0, 0), v2 (-0.866, 1.5, 0) and v3 (-0.866, -1.5, 0): side 3, centred on the origin.

rotate_triangle_by_direction(90.0f, 90.0f, &t);
// each vertex turns by -90 around Z, tilts by 90 around Y, and turns back by 90 around Z.

float h1, h2, h3;
get_heights(&t, &h1, &h2, &h3);
// h1 is 0, h2 -1.5 and h3 1.5: the z of each vertex.
```

`rotate_triabgle_around_x`, `_y` and `_z` turn all three vertices of a triangle around one axis, and `rotate_by_diraction` does to one point what `rotate_triangle_by_direction` does to a triangle.

## The tests

`build\rotate_tests.exe` takes GoogleTest's flags, for example `--gtest_list_tests` or `--gtest_filter=RotatePoint.*`.
