# Spec

A small C library that rotates points, vectors and triangles in 3D around the X, Y and Z axes and towards a heading and tilt, with a console demo and a Python ctypes wrapper over a prebuilt DLL.

Every `##` section below is one task. `Status` is one of To Do, In Progress, In Review, Done. The whole file is one ticket in Jira project KAN (the `Jira` line below), where every task is a checkbox: tick it there and the sync marks the task Done here; add one there and it is appended here.

Jira: KAN-59

## Rotate points, vectors and triangles around an axis
Status: Done

`rotate_point_around_x`, `_y` and `_z` in `src/rotate.c` turn two coordinates by an angle in degrees and write the result back through the pointers. `rotate_vector_around_*` apply them to a `struct Vector`, and `rotate_triabgle_around_*` (misspelling kept because `python/rotate.py` binds by name) to each vertex of a `struct Triangle`; the API is declared in `include/rotate.h`. GoogleTest cases in `tests/rotate_test.cpp` cover quarter turns, a full turn and a turn with its reverse.

## Rotate towards a heading and tilt
Status: Done

`rotate_by_diraction` in `src/rotate.c` turns a point by `-angle_a` around Z, tilts it by `angle_b` around Y, and turns it back by `angle_a` around Z. `rotate_triangle_by_direction` does this to all three vertices. Tests in `tests/rotate_test.cpp` check that a zero tilt leaves a point unchanged, that heading 0 with tilt 90 takes X to -Z, and that a rotated triangle keeps its side length.

## Equilateral triangle from its side, and vertex heights
Status: Done

`calculate_triangle_points_by_side` in `src/rotate.c` takes a `uint8_t` side (0 to 255) and places an equilateral triangle centred on the origin at z = 0, with the first vertex on the X axis. `get_heights` writes the z of each of the three vertices through three float pointers. Both are covered in `tests/rotate_test.cpp`, including side 0 giving a single point.

## Console demo
Status: Done

`src/main.c` builds the `triangle_rotation` executable: it places a triangle of side 3, applies `rotate_triangle_by_direction(90, 90, ...)`, and prints the vertices before and after plus the three heights. `.\recipe.ps1 run` builds and runs it, and `docs/manual.md` documents the expected heights 0, -1.5 and 1.5.

## Build, test and lint recipe
Status: Done

`recipe.ps1` builds the static `rotate` library, the demo and `rotate_tests` with CMake and Ninja (`CMakeLists.txt`, GoogleTest 1.13.0 fetched by URL and SHA256). Its `test` verb runs `ctest` (14 GoogleTest cases plus the `tests/test_manual_list.py` pytest), `clang-tidy`, `clang-format -i`, and a `git grep` that fails if `include` or `src` contain lock types or `throw`/`try`/`catch`. `publish` copies `include` and `src` to a versioned folder under `$LAB_SHARE`, and `.gitea/workflows/test.yaml` runs `recipe.ps1 test` followed by `git diff --exit-code` on pushes to `test`.

## Python ctypes wrapper
Status: Done

`python/rotate.py` declares `Vector` and `Triangle` as ctypes structures, loads the checked-in `python/rotate.dll` by a path relative to the current directory, sets argument types for every function declared in `include/rotate.h`, and wraps them as Python functions, plus a `number` helper that returns 0 for values under 1e-5. `python/main.py` repeats the demo through it (side 3, `rotate_triangle_by_direction(90, 90)`, heights).

## Fix the Python X and Y point-rotation wrappers
Status: To Do

`rotate_point_around_x` and `rotate_point_around_y` in `python/rotate.py` pass three pointers (x, y, z) to C functions that take two. The C side therefore rotates the wrong pair: a 90 degree turn of (0, 1, 0) around X returns (-1, 0, 0) instead of (0, 0, 1). `rotate_by_diraction` also passes two extra `vector` and `triangle` arguments the C function does not take. Acceptance: each wrapper passes only the coordinates its C function takes, and a pytest under `tests/` that loads the DLL checks quarter turns around X, Y and Z against the values `tests/rotate_test.cpp` expects.

## Build rotate.dll from source
Status: To Do

`CMakeLists.txt` builds `rotate` only as a STATIC library, and the `Makefile` writes a main.dll into bin, so nothing rebuilds the `python/rotate.dll` that `python/rotate.py` loads. That DLL was last committed in 2023 and still exports `main`, `test_rotate_x` and globals such as `cosTheta` that the current `src/rotate.c` no longer defines, so it was built from older source. Acceptance: `.\recipe.ps1 build` produces a shared rotate.dll from `src/rotate.c`, and the Python wrapper loads that build output instead of a committed binary.

## Fix the Blender triangle script
Status: To Do

`python/triangle_simulator.py` loads the DLL from `os.path.abspath('/python/rotate.dll')`, which points to the drive root, not the repository. After drawing, it calls `rotate.get_heights(new_triangle)` and `rotate.number(...)` on the raw `ctypes.CDLL`: the first raises `TypeError: this function takes at least 4 arguments (1 given)` and the second does not exist in the DLL. Acceptance: run inside Blender, the script loads the DLL relative to its own file, creates the `triangle_object` mesh, and prints the three heights without an error.

## Add entries to the manual test list
Status: To Do

`tests/manual/tests.json` has an empty `tests` list, so the manual round that `CONTRIBUTING.md` requires before `test` goes into `release` has nothing to check. Acceptance: the list holds at least one entry that passes `tests/test_manual_list.py`, for example running `.\recipe.ps1 run` and checking the printed heights against 0, -1.5 and 1.5 from `docs/manual.md`.
