# Install

The library needs no installing: put `include` on your include path, compile `src/rotate.c` with your project as C11, and link the math library (`-lm`).

To build and test it here:

1. Install mingw gcc and g++ 13 or newer, CMake 3.28 or newer, Ninja, and LLVM for `clang-tidy` and `clang-format`.
2. Clone this repository.
3. Run `.\recipe.ps1 test`. The first run downloads GoogleTest, whose archive `CMakeLists.txt` pins by hash.

`recipe.ps1` looks for gcc and g++ in `C:\ProgramData\mingw64\mingw64\bin` and for LLVM in `C:\Program Files\LLVM\bin`, and adds those folders to its own PATH.

`.\recipe.ps1 build` compiles the library, the demo `triangle_rotation` and the tests into `build\`. `.\recipe.ps1 install` does nothing: there is no installer.

The `Makefile` is the original route: `make` builds `bin/main.dll` from `src/*.c` with gcc. `python/rotate.py` loads `rotate.dll` from the folder it runs in, and `python/triangle_simulator.py` runs only inside Blender. No gate runs either.
