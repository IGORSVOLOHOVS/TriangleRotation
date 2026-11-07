# triangle-rotation

A small C library that turns points, vectors and triangles in 3D: around the X, Y and Z axes, and towards a direction given by a heading and a tilt. It also places an equilateral triangle around the origin from its side, and answers the heights of its vertices.

## What it does

- **Points, vectors and triangles around an axis**: `rotate_point_around_x`, `_y` and `_z` turn two coordinates by an angle in degrees; `rotate_vector_around_*` and `rotate_triabgle_around_*` turn a `struct Vector` or each vertex of a `struct Triangle`.
- **Towards a direction**: `rotate_by_diraction` turns a point by `-angle_a` around Z, tilts it by `angle_b` around Y, and turns it back by `angle_a`; `rotate_triangle_by_direction` does that to every vertex.
- **An equilateral triangle**: `calculate_triangle_points_by_side` places one of side 0 to 255 around the origin at z = 0, and `get_heights` answers the z of its three vertices.

The names keep their original spelling, `triabgle` and `diraction` among them, because `python/rotate.py` binds them by name.

## Layout

```text
include/rotate.h   the C API
src/rotate.c       the rotations
src/main.c         a demo: one triangle of side 3, turned towards heading 90 and tilted 90
tests/             GoogleTest cases for the library
python/            a ctypes wrapper over python/rotate.dll, and a Blender script that draws the triangle
Makefile           the original route: builds the shared library with gcc
```

## Build and test

`.\recipe.ps1 test` builds the library, the demo and the tests with CMake and Ninja, runs `ctest`, `clang-tidy` and `clang-format`, and searches `include` and `src` for locks and for `throw`, `try` and `catch`. It needs mingw gcc and g++, cmake, ninja and LLVM. `.\recipe.ps1 run` prints the demo. `docs/install.md` has the details, and `docs/manual.md` covers the functions.

MIT licensed: see `LICENSE`.

## Lab requirements

This repository complies with [revision 4.0.0](http://127.0.0.1:3000/igors-lab/software-requirements/src/tag/v4.0.0/BEST_REQUIREMENTS.md) of the lab requirements, except R-02 (#8), R-09 (#9), R-12 (#10), R-13 (#11), R-13 (#12), R-14 (#13), R-37 (#14), R-41 (#15), R-42 (#16).
