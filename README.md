
# 3D Geometry Rotation Library

## Overview

This C library provides a set of functions to perform 3D rotations on points and triangles. It includes rotation around individual axes (X, Y, Z) and also allows for rotation based on a given direction vector. The library is designed to be easy to integrate and use in larger projects requiring 3D geometry manipulation.

## Features

- Rotate points around X, Y, and Z axes.
- Rotate vectors around X, Y, and Z axes.
- Rotate triangles around X, Y, and Z axes.
- Rotate points and triangles by a given direction vector.
- Calculate triangle points based on side length.
- Retrieve the heights of the vertices of a triangle.

## How to Use

### Include the Header

Include the header file `rotate.h` in your project.

```c
#include "rotate.h"
```

### Basic Usage

#### Rotate Point Around X-Axis

```c
float y = 1.0, z = 0.0;
rotate_point_around_x(90.0, &y, &z);
```

#### Rotate Vector Around Y-Axis

```c
struct Vector v;
set_vector(&v, 1.0, 0.0, 0.0);
rotate_vector_around_y(90.0, &v);
```

#### Rotate Triangle Around Z-Axis

```c
struct Triangle t;
// Initialize t with vertices
rotate_triangle_around_z(45.0, &t);
```

#### Rotate Point by Direction Vector

```c
float x = 1.0, y = 0.0, z = 0.0;
rotate_by_direction(90.0, 45.0, &x, &y, &z);
```

### Advanced Usage

#### Calculate Triangle Points by Side Length

```c
struct Triangle t = calculate_triangle_points_by_side(10);
```

#### Get Heights of Triangle Vertices

```c
float h1, h2, h3;
get_heights(&t, &h1, &h2, &h3);
```

## Dependencies

- Math library for trigonometric functions (`<math.h>`)
