# triangle-rotation: software requirements specification

Reverse-engineered from the code on 2026-11-18 (commit `5062236`, branch `release`) and checked again on 2026-11-20 against this branch, `tr-59/docs/spec-md` at commit `a52fce0`, which is `test` (`aa49d43`) plus the first version of this file. The code is the same on both: `git diff --stat 5062236 a52fce0` lists only `CHANGELOG.md` and `SPEC.md`. Every requirement states behaviour that is implemented, with the file and line it comes from. Planned or broken behaviour is listed separately in [3.9](#39-declared-but-not-implemented).

The task list of this file mirrors Jira ticket KAN-59: every `### 3.N` subsection of section 3 from 3.1 to 3.8 is a done task, and every row of the tables in [3.9](#39-declared-but-not-implemented) is an open one. The first version of this file (2026-11-17) held one `##` section per task; its six done tasks are covered by 3.1 to 3.6, and its four open tasks are the rows of the second table in 3.9. Subsections 3.7 and 3.8 and the rows of the first table in 3.9 are new in this version.

<!-- jira-sync:start -->
Jira: KAN-59

## Tasks

This list is one ticket in Jira project KAN, [KAN-59](https://igorsvolohovs.atlassian.net/browse/KAN-59). Each line is a checkbox there: tick it in Jira and the sync ticks it here; add a checkbox there and it is appended here. Done items are the features described in section 3; open items are what the project declares but the code does not do yet.

- [x] Rotate points, vectors and triangles around an axis (section 3.1) <!-- spec-rotate-points-vectors-and-triangles-around-an-axis -->
- [x] Rotate towards a heading and tilt (section 3.2) <!-- spec-rotate-towards-a-heading-and-tilt -->
- [x] Equilateral triangle from its side, and vertex heights (section 3.3) <!-- spec-equilateral-triangle-from-its-side-and-vertex-heig -->
- [x] Console demo (section 3.4) <!-- spec-console-demo -->
- [x] Python ctypes wrapper (section 3.5) <!-- spec-python-ctypes-wrapper -->
- [x] Build, test and lint recipe (section 3.6) <!-- spec-build-test-and-lint-recipe -->
- [x] Manual test list and page (section 3.7) <!-- spec-manual-test-list-and-page -->
- [x] CI, hooks and wiki (section 3.8) <!-- spec-ci-hooks-and-wiki -->
- [ ] Wiki API pages for the C core (declared, not implemented) <!-- spec-wiki-api-pages-for-the-c-core -->
- [ ] Compliance with lab requirements 4.0.0 (declared, not implemented) <!-- spec-compliance-with-lab-requirements-4-0-0 -->
- [ ] Fix the Python X and Y point-rotation wrappers - `rotate_point_around_x` and `rotate_point_around_y` in `python/rotate.py` pass three pointers (x, y, z) to C functions that take two. The C side therefore rotates the wrong pair: a 90 degree turn of (0, 1, 0) around X returns (-1, 0, 0) instead of (0, 0, 1). `rotate_by_diraction` also passes two extra `vector` and `triangle` arguments the C function does not take. Acceptance: each wrapper passes only the coordinates its C function takes, and a pytest under `tests/` that loads the DLL checks quarter turns around X, Y and Z against the values `tests/rotate_test.cpp` expects. (declared, not implemented) <!-- spec-fix-the-python-x-and-y-point-rotation-wrappers-rot-4928e85806 -->
- [ ] Build rotate.dll from source - `CMakeLists.txt` builds `rotate` only as a STATIC library, and the `Makefile` writes a main.dll into bin, so nothing rebuilds the `python/rotate.dll` that `python/rotate.py` loads. That DLL was last committed in 2023 and still exports `main`, `test_rotate_x` and globals such as `cosTheta` that the current `src/rotate.c` no longer defines, so it was built from older source. Acceptance: `.\recipe.ps1 build` produces a shared rotate.dll from `src/rotate.c`, and the Python wrapper loads that build output instead of a committed binary. (declared, not implemented) <!-- spec-build-rotate-dll-from-source-cmakelists-txt-builds-c408ab9e65 -->
- [ ] Fix the Blender triangle script - `python/triangle_simulator.py` loads the DLL from `os.path.abspath('/python/rotate.dll')`, which points to the drive root, not the repository. After drawing, it calls `rotate.get_heights(new_triangle)` and `rotate.number(...)` on the raw `ctypes.CDLL`: the first raises `TypeError: this function takes at least 4 arguments (1 given)` and the second does not exist in the DLL. Acceptance: run inside Blender, the script loads the DLL relative to its own file, creates the `triangle_object` mesh, and prints the three heights without an error. (declared, not implemented) <!-- spec-fix-the-blender-triangle-script-python-trianglesim-ece23efbce -->
- [ ] Add entries to the manual test list - `tests/manual/tests.json` has an empty `tests` list, so the manual round that `CONTRIBUTING.md` requires before `test` goes into `release` has nothing to check. Acceptance: the list holds at least one entry that passes `tests/test_manual_list.py`, for example running `.\recipe.ps1 run` and checking the printed heights against 0, -1.5 and 1.5 from `docs/manual.md`. (declared, not implemented) <!-- spec-add-entries-to-the-manual-test-list-tests-manual-t-c854173e39 -->

<!-- jira-sync:end -->

## 1. Purpose and scope

triangle-rotation is a C library that turns points, vectors and triangles in 3D around the X, Y and Z axes and towards a direction given by a heading and a tilt, places an equilateral triangle around the origin from its side, and answers the heights of its vertices (`README.md:3`).

In scope:

- a C library (`include/rotate.h`, `src/rotate.c`) that rotates points, vectors and triangles around the X, Y and Z axes and towards a direction, builds an equilateral triangle from its side, and reports vertex heights;
- the console demo `triangle_rotation` (`src/main.c`);
- the Python `ctypes` wrapper and demo (`python/rotate.py`, `python/main.py`) and the Blender script (`python/triangle_simulator.py`);
- the build and test recipe (`recipe.ps1`, `CMakeLists.txt`, `Makefile`), the tests, and the lab tooling in the repository: Gitea workflows, git hooks, the manual-test page, the wiki generator.

Out of scope (not present in the code): rotation by matrices or quaternions, arbitrary-axis rotation, triangles other than equilateral ones built from an integer side, input validation, persistence, a network or hardware interface for the library, an installer (`recipe.ps1:18`).

## 2. Context

| Actor or system | Role | Source |
|---|---|---|
| C or C++ caller | Includes `rotate.h`, links `rotate` | `include/rotate.h:6-8`, `CMakeLists.txt:23-24` |
| Python caller | Uses `python/rotate.py` over `python/rotate.dll` | `python/rotate.py:18-41` |
| Blender | Runs `python/triangle_simulator.py` (`import bpy`) | `python/triangle_simulator.py:1`, `docs/install.md:15` |
| Developer | Runs `recipe.ps1`; commits pass the git hooks | `CLAUDE.md:8-19`, `CONTRIBUTING.md` |
| Tester | Records manual results through `tests/manual/index.html` | `tests/manual/serve.py:1-8` |
| Gitea server and its Actions runner (label `windows`) | Runs `.gitea/workflows`, holds statuses, releases, packages, wiki | `.gitea/workflows/*.yaml` |
| Lab share (`LAB_SHARE`) | Receives `include` and `src` from `publish` | `recipe.ps1:92-102` |
| GitHub | Hosts the public copy (the `origin` remote of this clone) and pull request #2 of this branch into `test`; runs nothing, no `.github/` directory | `git remote -v`, `git ls-files`, `gh pr view 2` |
| Jira (ticket KAN-59) | Holds the task list of this file as checkboxes | first version of this file, `git show a52fce0:SPEC.md` lines 5-7 |

Runtime environment:

- Language: C11 for the library and demo, C++17 for the tests (`CMakeLists.txt:4-10`); Python 3 for the wrapper, tests and tools.
- OS: Windows. Evidence: `recipe.ps1` is PowerShell with Windows paths (`recipe.ps1:47`); the workflows run on `windows` with `shell: powershell`; `.clang-tidy` targets `x86_64-w64-mingw32`; the committed DLLs are PE32+ x86-64. The library source uses only `<math.h>` and `<stdint.h>`; whether it is meant to run elsewhere is not stated.
- Hardware: none addressed by the code.

## 3. Functional requirements

### 3.1 Rotate points, vectors and triangles around an axis

**FR-1.** The system shall rotate the pair `(y, z)` around the X axis by an angle in degrees, in place: `y' = y cos a - z sin a`, `z' = y sin a + z cos a`.
Source: `src/rotate.c:13-23`, `src/rotate.c:6`. Verification: `RotatePoint.AQuarterTurnAroundXTakesYToZ`, `RotatePoint.ATurnAndItsReverseCancel`.

**FR-2.** The system shall rotate the pair `(x, z)` around the Y axis by an angle in degrees, in place: `x' = x cos a + z sin a`, `z' = -x sin a + z cos a`.
Source: `src/rotate.c:25-35`. Verification: `RotatePoint.AQuarterTurnAroundYTakesXToMinusZ`.

**FR-3.** The system shall rotate the pair `(x, y)` around the Z axis by an angle in degrees, in place: `x' = x cos a - y sin a`, `y' = x sin a + y cos a`.
Source: `src/rotate.c:37-47`. Verification: `RotatePoint.AQuarterTurnAroundZTakesYToMinusX`, `RotatePoint.AFullTurnLeavesThePointWhereItWas`.

**FR-4.** The system shall store three given coordinates into a `struct Vector`.
Source: `src/rotate.c:61-65`. Verification: `SetVector.StoresTheThreeCoordinates`.

**FR-5.** The system shall rotate a `struct Vector` around X, Y or Z by applying FR-1, FR-2 or FR-3 to its two coordinates other than the axis, leaving the axis coordinate unchanged.
Source: `src/rotate.c:67-69`. Verification: `RotateVector.TurnsLikeItsPointAndKeepsTheThirdAxis`.

**FR-6.** The system shall rotate a `struct Triangle` around X, Y or Z by applying FR-5 to `v1`, `v2` and `v3` (functions `rotate_triabgle_around_x/_y/_z`; the spelling is kept because `python/rotate.py` binds the names, `README.md:11`).
Source: `src/rotate.c:71-85`. Verification: `RotateTriangle.TurnsEveryVertex`.

### 3.2 Rotate towards a heading and tilt

**FR-7.** The system shall rotate a point `(x, y, z)` towards a direction by turning it by `-angle_a` around Z, then by `angle_b` around Y, then by `angle_a` around Z (`rotate_by_diraction`). The README describes the direction as a heading and a tilt, and the demo's `(90, 90)` as heading 90 and tilt 90 (`README.md:3`, `18`), so `angle_a` is the heading and `angle_b` the tilt. At heading 0 a tilt of +90 takes `(1, 0, 0)` to `(0, 0, -1)`, and a tilt of 0 leaves any point where it was.
Source: `src/rotate.c:50-59`, `tests/rotate_test.cpp:119-138`. Verification: `RotateByDirection.AZeroTiltLeavesThePointAlone`, `RotateByDirection.TiltsAboutTheAxisAtTheGivenHeading`.

**FR-8.** The system shall apply FR-7 to each vertex of a `struct Triangle` (`rotate_triangle_by_direction`), which keeps the lengths of its sides.
Source: `src/rotate.c:87-91`. Verification: `RotateTriangleByDirection.KeepsTheSidesOfTheTriangle` (side 3 after `(90, 90)`, `tests/rotate_test.cpp:140-147`).

### 3.3 Equilateral triangle from its side, and vertex heights

**FR-9.** The system shall return, for a side `s` of type `uint8_t` (0 to 255), a triangle centred on the origin at z = 0 with circumradius `r = s * 0.57735` and vertices `v1 = (r, 0, 0)`, `v2 = (-0.5r, 0.866r, 0)`, `v3 = (-0.5r, -0.866r, 0)`; a side of 0 gives three vertices at the origin.
Source: `src/rotate.c:93-102`, constants `src/rotate.c:7-10`, `include/rotate.h:40`. Verification: `CalculateTriangle.PlacesAnEquilateralTriangleAroundTheOrigin`, `CalculateTriangle.ASideOfZeroIsAPoint`.

**FR-10.** The system shall write the z coordinates of `v1`, `v2`, `v3` of a triangle into three given floats (`get_heights`).
Source: `src/rotate.c:104-108`. Verification: `GetHeights.AnswersTheZOfEachVertex`.

**FR-11.** The system shall declare its API with C linkage when the header is included from C++.
Source: `include/rotate.h:6-8`, `44-46`. Verification: `rotate_tests` (C++) links against the C library (`CMakeLists.txt:49-50`).

### 3.4 Console demo

**FR-12.** The demo `triangle_rotation` shall build a triangle of side 3, print its vertices, apply `rotate_triangle_by_direction(90, 90, ...)`, print the vertices and the three heights, and exit with 0.
Source: `src/main.c:4-27`. Verification: no test; manual run of `.\recipe.ps1 run` against `docs/manual.md:5`, `31` (heights 0, -1.5, 1.5). On 2026-11-18, and again on 2026-11-20 on this branch, `src/main.c` and `src/rotate.c` compiled with clang (not the recipe's gcc) printed heights `0.000000 -1.499955 1.499955` and exited 0. The 2026-11-20 build used clang 20.1.0 with `-std=c11 -Wall -Wextra -Wpedantic` and gave one warning: `src/main.c` ends without a newline (`-Wnewline-eof`).

### 3.5 Python ctypes wrapper

**FR-13.** `python/rotate.py` shall declare `ctypes` structures `Vector` and `Triangle` with the field layout of `rotate.h`, load `rotate.dll` from the current working directory, and set `argtypes` for the 14 C functions and `restype` for `calculate_triangle_points_by_side`.
Source: `python/rotate.py:7-41`. Verification: no test; `python/main.py` run from `python/` on 2026-11-18 and on 2026-11-20 completed with exit 0.

**FR-14.** `python/rotate.py` shall wrap the vector, triangle and direction-triangle rotations, the triangle constructor (side passed as `c_uint8`) and `get_heights` (returned as a tuple of three floats), and provide `number(v)` that returns `0.0` when `|v| < 1e-5`.
Source: `python/rotate.py:43-53`, `76-122`. Verification: no test; the runs of 2026-11-18 and 2026-11-20 printed heights `0.0 -1.4999552965164185 1.4999552965164185`.

**FR-15.** `python/main.py` shall repeat the demo of FR-12 through the wrapper and print the values through `number`.
Source: `python/main.py:1-23`. Verification: no test; run on 2026-11-18 and 2026-11-20.

### 3.6 Build, test and lint recipe

**FR-16.** `recipe.ps1` shall accept exactly one of the verbs `build`, `install`, `uninstall`, `clean`, `run`, `publish`, `info`, `test`, followed by optional arguments.
Source: `recipe.ps1:1-7`. Verification: no test; `.\recipe.ps1 info` run on 2026-11-18 and 2026-11-20.

**FR-17.** Before any verb, `recipe.ps1` shall load `KEY=VALUE` lines from `.claude/.env` into the environment for variables not already set, and prepend the LLVM and mingw folders to `PATH` when they exist.
Source: `recipe.ps1:36-49`. Verification: no test.

**FR-18.** `build` shall configure CMake with Ninja, `gcc`, `g++` and `Release` into `build\`, build, and exit 1 on either failure.
Source: `recipe.ps1:51-57`, `74-77`. Verification: CI step "Build installer" / "Build as publishing will" (`release-gate.yaml:55-57`, `test.yaml:59-60`).

**FR-19.** `test` shall set `core.hooksPath` to `.claude/hooks/git`, build, run `ctest --no-tests=error`, run `clang-tidy` over `src/*.c` and `tests/*.cpp`, run `clang-format -i` over `include/*.h`, `src/*.c`, `tests/*.cpp`, and exit 1 at the first failure.
Source: `recipe.ps1:111-124`. Verification: CI steps calling `.\recipe.ps1 test` (`propose.yaml:146`, `test.yaml:56`, `release-gate.yaml:51`).

**FR-20.** `test` shall fail when `git grep -w` finds a lock word (`mutex` and its variants, `lock_guard`, `condition_variable`, `counting_semaphore`, `binary_semaphore`, `latch`, `barrier`, `call_once`, `CRITICAL_SECTION`, `pthread_mutex_t` and others) or `throw`, `try`, `catch` in `include` or `src`, when either folder has no tracked file, or when `git grep` exits with anything but 1.
Source: `recipe.ps1:59-69`, `125-126`. Verification: no test of the check itself.

**FR-21.** `run` shall build and then run `build\triangle_rotation.exe` with the remaining arguments, exiting with its exit code.
Source: `recipe.ps1:86-91`. Verification: no test.

**FR-22.** `clean` shall delete `dist`, `build` and `out` where present.
Source: `recipe.ps1:80-85`. Verification: no test.

**FR-23.** `publish` shall exit 1 when `LAB_SHARE` is unset, and otherwise copy `include` and `src` into `$LAB_SHARE\needs\triangle-rotation\v1.0.0`.
Source: `recipe.ps1:92-102`. Verification: no test; no workflow calls it (`grep` of `.gitea/workflows`).

**FR-24.** `info` shall print name, code, version, summary and provided folders; `install` and `uninstall` shall print that there is nothing to do and exit 0.
Source: `recipe.ps1:78-79`, `103-110`. Verification: `info` run on 2026-11-18 and 2026-11-20.

**FR-25.** The CMake build shall produce the static library `rotate` (linked with `m` outside MSVC), the executable `triangle_rotation`, and, when `BUILD_TESTING` is on, `rotate_tests` with GoogleTest v1.13.0 (14 cases, `tests/rotate_test.cpp:25-161`) and two CTest tests, `rotate_tests` and `manual_list`.
Source: `CMakeLists.txt:14-59`. Verification: `ctest` inside `recipe.ps1 test`.

**FR-26.** `make` shall compile `src/*.c` with `gcc -Wall -Wextra -ggdb -shared` into `bin/main.dll`; `make clean` shall run `rm bin/*`.
Source: `Makefile:1-22`. Verification: no test.

### 3.7 Manual test list and page

**FR-27.** The test suite shall reject `tests/manual/tests.json` when it starts with a UTF-8 BOM, its `version` is not 1, `tests` is not a list, an `id` does not match `[a-z0-9][a-z0-9_-]{0,60}` or is `verdict` or repeats, `title`, `expected` or `steps` have the wrong type, optional `issue`, `blocking`, `needs` have the wrong type, or any string holds a control character.
Source: `tests/test_manual_list.py:23-49`. Verification: `test_the_list_has_a_known_shape`, `test_every_entry_is_complete` (2 passed on 2026-11-18 and on 2026-11-20).

**FR-28.** `tests/manual/serve.py` shall serve `tests/manual/` on `127.0.0.1:8765`, answer `/repo.json`, forward `/api/v1/` and `/api/packages/` requests to the server of the `origin` remote with `Authorization: token <GITEA_TOKEN>`, and redirect other unknown paths to that server.
Source: `tests/manual/serve.py:23-30`, `115-158`. Verification: no test.

**FR-29.** `serve.py` shall refuse a request whose `Host` is not `127.0.0.1:8765` or `localhost:8765` (403), an API request without header `X-Manual-Page: 1` (403), and a write outside the API (405); it shall exit when no key is found, when the port is taken, or when `origin` is not `http(s)://<server>/<owner>/<repository>`.
Source: `tests/manual/serve.py:40-49`, `103-113`, `132-140`, `168-180`. Verification: no test.

**FR-30.** The manual-test page shall find the open pull request from `test` into `release`, post commit status `manual/<id>` as `pending`, `success`, `failure` or `warning` (N/A, non-blocking tests only, note required), optionally upload an evidence file to the generic package `<name>-manual-evidence`, and on "Done" post `manual/verdict` as `success` only when every test is passed or N/A.
Source: `tests/manual/index.html:79-110`, `132-163`, `212-237`. Verification: no test.

### 3.8 CI, hooks and wiki

**FR-31.** Workflow `propose` shall run on pull requests into `test` and check the branch name, the commit wording, `.\recipe.ps1 test` followed by `git diff --exit-code`, then request review.
Source: `.gitea/workflows/propose.yaml:1-5`, `33`, `77`, `144-154`. Verification: runs on the Gitea server.

**FR-32.** Workflow `test` shall run on pushes to `test`, daily at 03:00 and on demand: check runner health, run `.\recipe.ps1 test` and `git diff --exit-code`, build, and regenerate the wiki with `tools/wiki.py`.
Source: `.gitea/workflows/test.yaml:1-7`, `32-110`. Verification: runs on the Gitea server.

**FR-33.** Workflow `release-gate` shall run on pull requests into `release`, admit only `test` as the head, run the recipe tests and build, then wait for the manual verdict; an empty manual list shall pass automatically.
Source: `.gitea/workflows/release-gate.yaml:1-5`, `27`, `49-57`, `86-150`, `221`. Verification: runs on the Gitea server.

**FR-34.** Workflow `release` shall run on pushes to `release`, read `$Version` from `recipe.ps1`, skip when tag `v<version>` exists, and otherwise create release `v<version>` with notes, `manual-report.md` and `sbom.md`, close the shipped issues, and write the changelog onto `test`; it brings `test` level with `release` whether or not a release was made.
Source: `.gitea/workflows/release.yaml:1-4`, `45-60`, `76`, `279-340`, `356`, `402`. Verification: runs on the Gitea server; its changelog commit for v1.0.0, `aa49d43` by `lab-bot` ("docs(docs): changelog for v1.0.0", `CHANGELOG.md`), is the tip of `test` and the parent of this branch's commit `a52fce0`.

**FR-35.** Workflow `sweep` shall run on Mondays at 04:00, delete work branches already merged into `test`, comment on pull requests idle for 30 days, and close those idle for 60.
Source: `.gitea/workflows/sweep.yaml:3-4`, `23-80`. Verification: runs on the Gitea server.

**FR-36.** The `commit-msg` hook shall refuse a message that is not `<type>(<scope>): ...` with a type and scope from `recipe.ps1`, lacks a `Signed-off-by` trailer, or starts with a BOM; the `pre-push` hook shall refuse a branch outside `<code>-<issue>/<type>/<slug>` and a push to `release` without the override marker and a `Why:` line.
Source: `.claude/hooks/git/commit-msg.ps1:21-53`, `.claude/hooks/git/pre-push.ps1:54-93`. Verification: no test in the repository; on 2026-11-18, and again on 2026-11-20, `commit-msg.ps1` run by hand accepted `fix(core): x` with a `Signed-off-by` trailer and refused `fix: x`, a message without the trailer, and one starting with a BOM.

**FR-37.** The Claude Code hook `gate.ps1` shall deny recursive deletes outside `$env:TEMP`, force-pushes other than `--force-with-lease`, history rewrites on `test` and `release`, and tool calls it cannot parse; `log.ps1` shall append one JSON line per hook event to `.claude/logs/ai.jsonl`.
Source: `.claude/hooks/gate.ps1:34-36`, `89-177`, `.claude/hooks/log.ps1:1-26`, `.claude/settings.json:7-18`. Verification: no test.

**FR-38.** `tools/wiki.py` shall build the pages `Home`, `api/<group>`, `Development-plans`, `Install`, `Manual` and `_Sidebar` from docstrings of `*.py` files under the `$Provides` folders, `README.md` and `docs/`; without `--push` or `--out` it shall only print the page names.
Source: `tools/wiki.py:230-303`, `377-404`. Verification: dry runs on 2026-11-18 and on 2026-11-20 (`--sha a52fce0`) printed 5 pages.

### 3.9 Declared but not implemented

Every row of the two tables below is an open task of this file's task list. The first table is new in this version; the second holds the four open tasks of the first version, which are already checkboxes of Jira KAN-59.

| Item | What is declared | What the code does | Source |
|---|---|---|---|
| Wiki API pages for the C core | `$Provides` is "what the wiki reads" | Wiki reads only `*.py`; 0 API pages for `include`, `src` | `recipe.ps1:19`, `tools/wiki.py:238` |
| Compliance with lab requirements 4.0.0 | Claimed with nine exceptions, issues #8 to #16 | The requirements text is not in the repository | `README.md:32` |

Tasks proposed on 2026-11-17 in the first version of this file. The first cell is the task's text as it stands in Jira; the second condenses it; the third is what the code of this branch does, checked on 2026-11-20.

| Task | In short | What the code does | Source |
|---|---|---|---|
| Fix the Python X and Y point-rotation wrappers - `rotate_point_around_x` and `rotate_point_around_y` in `python/rotate.py` pass three pointers (x, y, z) to C functions that take two. The C side therefore rotates the wrong pair: a 90 degree turn of (0, 1, 0) around X returns (-1, 0, 0) instead of (0, 0, 1). `rotate_by_diraction` also passes two extra `vector` and `triangle` arguments the C function does not take. Acceptance: each wrapper passes only the coordinates its C function takes, and a pytest under `tests/` that loads the DLL checks quarter turns around X, Y and Z against the values `tests/rotate_test.cpp` expects. | The Python point wrappers around X and Y, and `rotate_by_diraction`, should pass the C functions only the arguments they take, checked by a pytest that loads the DLL. | `python/rotate.py:59`, `66` pass `x, y, z` to C functions taking two pointers (`include/rotate.h:25-26`). Measured on 2026-11-18 and 2026-11-20: a 90 degree turn of `(0, 1, 0)` around X gave `(-1, 0, 0)`, of `(1, 0, 0)` around Y gave `(0, -1, 0)`; the Z wrapper gave the expected `(0, 1, 0)`. `python/rotate.py:94-98` requires and forwards two extra arguments `vector`, `triangle`. | Jira KAN-59 (task proposed 2026-11-17) |
| Build rotate.dll from source - `CMakeLists.txt` builds `rotate` only as a STATIC library, and the `Makefile` writes a main.dll into bin, so nothing rebuilds the `python/rotate.dll` that `python/rotate.py` loads. That DLL was last committed in 2023 and still exports `main`, `test_rotate_x` and globals such as `cosTheta` that the current `src/rotate.c` no longer defines, so it was built from older source. Acceptance: `.\recipe.ps1 build` produces a shared rotate.dll from `src/rotate.c`, and the Python wrapper loads that build output instead of a committed binary. | `.\recipe.ps1 build` should produce a shared `rotate.dll` from `src/rotate.c`, and the Python wrapper should load that build output instead of the DLL committed in 2023. | CMake builds `rotate` only as a static library (`CMakeLists.txt:23`) and `make` writes `bin/main.dll` (`Makefile:10`). `python/rotate.dll` was committed once, on 2023-09-08 (`0d16c01`); `llvm-objdump -p` (2026-11-18 and 2026-11-20) shows it exports `main`, `test_rotate_x/_y/_z` and globals such as `cosTheta`, `new_x`, `radius` that `src/rotate.c` does not define, under the DLL name `main.dll`. The README keeps the old names "because `python/rotate.py` binds them by name" (`README.md:11`). | Jira KAN-59 (task proposed 2026-11-17) |
| Fix the Blender triangle script - `python/triangle_simulator.py` loads the DLL from `os.path.abspath('/python/rotate.dll')`, which points to the drive root, not the repository. After drawing, it calls `rotate.get_heights(new_triangle)` and `rotate.number(...)` on the raw `ctypes.CDLL`: the first raises `TypeError: this function takes at least 4 arguments (1 given)` and the second does not exist in the DLL. Acceptance: run inside Blender, the script loads the DLL relative to its own file, creates the `triangle_object` mesh, and prints the three heights without an error. | The Blender script should load the DLL relative to its own file, create the `triangle_object` mesh and print the three heights without an error. | `python/triangle_simulator.py:20` resolves `/python/rotate.dll` at the drive root. Lines 141 and 144 call `get_heights` with one argument and `number` on the raw `ctypes.CDLL`, before the mesh code at lines 151-157. Outside Blender, with the same four `argtypes`, the one-argument call raised "this function takes at least 4 arguments (1 given)", and the DLL has no `number` export (checked 2026-11-18 and 2026-11-20). The script was not run in Blender. | Jira KAN-59 (task proposed 2026-11-17) |
| Add entries to the manual test list - `tests/manual/tests.json` has an empty `tests` list, so the manual round that `CONTRIBUTING.md` requires before `test` goes into `release` has nothing to check. Acceptance: the list holds at least one entry that passes `tests/test_manual_list.py`, for example running `.\recipe.ps1 run` and checking the printed heights against 0, -1.5 and 1.5 from `docs/manual.md`. | `tests/manual/tests.json` should hold at least one entry that passes `tests/test_manual_list.py`, for example a run of the demo checked against the heights in `docs/manual.md`. | `tests/manual/tests.json:3` is an empty `tests` list; `CONTRIBUTING.md:11-12` asks for "manual tests on the cell" before `release`; with no entries the gate posts `manual/verdict` as success, "no manual tests defined" (`.gitea/workflows/release-gate.yaml:139-150`). | Jira KAN-59 (task proposed 2026-11-17) |

## 4. Interfaces

### 4.1 C API (`include/rotate.h`)

| Function | Parameters | Effect |
|---|---|---|
| `set_vector` | `struct Vector *v, float x, float y, float z` | FR-4 |
| `rotate_point_around_x` | `float angle, float *y, float *z` | FR-1 |
| `rotate_point_around_y` | `float angle, float *x, float *z` | FR-2 |
| `rotate_point_around_z` | `float angle, float *x, float *y` | FR-3 |
| `rotate_vector_around_x/_y/_z` | `float angle, struct Vector *v` | FR-5 |
| `rotate_triabgle_around_x/_y/_z` | `float angle, struct Triangle *t` | FR-6 |
| `rotate_by_diraction` | `float angle_a, float angle_b, float *x, float *y, float *z` | FR-7 |
| `rotate_triangle_by_direction` | `float angle_a, float angle_b, struct Triangle *t` | FR-8 |
| `calculate_triangle_points_by_side` | `uint8_t side` → `struct Triangle` | FR-9 |
| `get_heights` | `struct Triangle *t, float *h1, float *h2, float *h3` | FR-10 |

Angles are degrees. The spellings `triabgle` and `diraction` are part of the interface (`README.md:11`).

### 4.2 Command line

| Command | Arguments | Exit code |
|---|---|---|
| `recipe.ps1 <verb>` | verb from FR-16; `run` passes the rest to the demo | 0 success, 1 failure; `run` returns the demo's code |
| `triangle_rotation` | none read | 0 |
| `tools/wiki.py` | `--push`, `--wiki-url URL`, `--token TOKEN`, `--sha SHA`, `--out DIR` | 0; 1 when `--push` lacks URL or token (`tools/wiki.py:400-403`), or the wiki clone or push fails (`tools/wiki.py:334-363`) |
| `tests/manual/serve.py` | none | runs until Ctrl+C; exits with a message on a missing key, a busy port, a bad `origin` |

### 4.3 HTTP (`tests/manual/serve.py`, bound to `127.0.0.1:8765`)

| Method and path | Response |
|---|---|
| `GET /`, `GET /<file in tests/manual>` | The file, `Cache-Control: no-store` |
| `GET /repo.json` | `{"repo": "<owner>/<name>"}` |
| `GET`, `POST`, `PUT`, `PATCH`, `DELETE` on `/api/v1/...`, `/api/packages/...` with `X-Manual-Page: 1` | Forwarded to the `origin` server with the token; upstream status and body; 502 JSON when unreachable |
| `GET` other path | 302 to the same path on the `origin` server |
| `POST/PUT/PATCH/DELETE` outside the API | 405 |
| wrong `Host`, or API call without the header | 403 |

## 5. Data

| Entity | Form | Where |
|---|---|---|
| `struct Vector` | three `float`: `x`, `y`, `z` | `include/rotate.h:10-14` |
| `struct Triangle` | three `struct Vector`: `v1`, `v2`, `v3`, taken from the origin | `include/rotate.h:16-21` |
| Manual test list | JSON, UTF-8 without BOM: `{"version": 1, "tests": [{"id", "title", "steps": [...], "expected", "issue"?, "blocking"?, "needs"?}]}` | `tests/manual/tests.json`, rules in FR-27 |
| Manual results | Gitea commit statuses `manual/<id>`, `manual/verdict`; evidence in generic package `<name>-manual-evidence/<sha>/` | `tests/manual/index.html:56-62`, `137-138` |
| Hook log | JSON lines with `ts`, `event`, `session`, `tool`, `detail` (first 200 characters), and `decision` from the gate; rotated above 10 MB; gitignored | `.claude/hooks/log.ps1:4-17`, `.claude/hooks/gate.ps1:6-23`, `.claude/hooks/session-start-stop.ps1:27-30`, `.gitignore:17` |
| Keys | `.claude/.env`, gitignored, never committed | `.gitignore:24`, `CLAUDE.md:20-24` |
| Changelog | `CHANGELOG.md`, written onto `test` by the `release` workflow; holds v1.0.0 (2026-11-08) | `CHANGELOG.md:1-36`, `.gitea/workflows/release.yaml:402-418` |

The library keeps no data between calls and writes no files.

## 6. Non-functional requirements

**NFR-1.** The library and demo shall compile as C11 without extensions, the tests as C++17 (`CMakeLists.txt:4-10`).

**NFR-2.** Outside MSVC, every target shall compile with `-Wall -Wextra -Wpedantic -Werror` (`CMakeLists.txt:16-20`).

**NFR-3.** `clang-tidy` checks `clang-diagnostic-*`, `clang-analyzer-*`, `bugprone-*` (except `easily-swappable-parameters`), `performance-*`, `portability-*`, all as errors (`.clang-tidy`).

**NFR-4.** Sources shall match `.clang-format` (LLVM base, 120 columns, 4 spaces); CI fails when `recipe.ps1 test` leaves a diff (`propose.yaml:144-152`).

**NFR-5.** `include` and `src` shall contain no lock and no exception keyword (`recipe.ps1:125-126`).

**NFR-6.** The library shall hold no mutable file-scope state: all file-scope objects are `static const` (`src/rotate.c:6-10`; commit `236ac7b` "keep the rotations' state local"). Verification: inspection; NFR-5 search.

**NFR-7.** Arithmetic is single-precision `float` with `cosf`/`sinf`; `cos 30` and `1/sqrt(3)` are stored to 3 and 5 digits (`src/rotate.c:6-10`). The tests accept a deviation of `1e-3` per coordinate and `1e-2` on a squared side (`tests/rotate_test.cpp:7`, `146`).

**NFR-8.** The GoogleTest dependency shall be fetched as an archive verified by SHA256, not by a git clone (`CMakeLists.txt:37-44`).

**NFR-9.** Time limits: workflow jobs 60 minutes (`propose`, `test`, `release`, `release-gate/checks`), manual gate 170 minutes with a 150-minute wait polled every 30 s (`release-gate.yaml:91`, `101`, `337`), `sweep` 15 minutes; `serve.py` gives upstream calls 120 s and the port probe 0.5 s (`serve.py:67`, `151`).

**NFR-10.** The test workflow shall fail when drive C: has under 5 GB free, warn under 20 GB, and fail when `python`, `ruff`, `pytest` or `mypy` does not answer (`test.yaml:32-45`).

**NFR-11.** `serve.py` shall listen on loopback only with exclusive address use, never print the key, and never hand the key to the page (`serve.py:1-8`, `23`, `73-86`, `103-113`).

**NFR-12.** Line endings: LF by default, CRLF for `*.ps1`, `*.bat`, `*.cmd`; `*.pdf`, `*.png`, `*.jpg`, `*.zip`, `*.7z` via Git LFS; `*.exe` binary (`.gitattributes`).

## 7. Constraints and dependencies

No lock file exists (no `uv.lock`, `requirements*.txt`, `pyproject.toml`). Versions as the files give them:

| Dependency | Version | Source |
|---|---|---|
| CMake | >= 3.28 | `CMakeLists.txt:1` |
| GoogleTest | v1.13.0, gmock off, not installed | `CMakeLists.txt:39-47` |
| mingw gcc, g++ | 13 or newer | `docs/install.md:7` |
| Ninja, LLVM `clang-tidy`/`clang-format` | not pinned | `recipe.ps1:53`, `120`, `123` |
| C math library `m` | system | `CMakeLists.txt:25-27` |
| Python | not pinned; inference: >= 3.10, since `tools/wiki.py:172` uses `ast.FunctionDef \| ast.AsyncFunctionDef` in `isinstance` | `tools/wiki.py` |
| pytest | not pinned | `CMakeLists.txt:57` |
| Blender (`bpy`) | not pinned | `python/triangle_simulator.py:1` |
| `serve.py` | standard library only | `tests/manual/serve.py:7` |
| `actions/checkout` | v4 | `.gitea/workflows/*.yaml` |
| `actions/upload-artifact` | v3 | `.gitea/workflows/release-gate.yaml:442` |
| Windows PowerShell | 5.1 (PR checklist) | `.gitea/pull_request_template.md:16` |

## 8. Acceptance

1. `.\recipe.ps1 test` exits 0 and `git diff --exit-code` reports nothing afterwards: this covers FR-1 to FR-11, FR-19, FR-20, FR-25, FR-27, NFR-1 to NFR-5.

| FR | Test |
|---|---|
| FR-1 | `RotatePoint.AQuarterTurnAroundXTakesYToZ`, `RotatePoint.ATurnAndItsReverseCancel` |
| FR-2 | `RotatePoint.AQuarterTurnAroundYTakesXToMinusZ` |
| FR-3 | `RotatePoint.AQuarterTurnAroundZTakesYToMinusX`, `RotatePoint.AFullTurnLeavesThePointWhereItWas` |
| FR-4 | `SetVector.StoresTheThreeCoordinates` |
| FR-5 | `RotateVector.TurnsLikeItsPointAndKeepsTheThirdAxis` |
| FR-6 | `RotateTriangle.TurnsEveryVertex` |
| FR-7 | `RotateByDirection.AZeroTiltLeavesThePointAlone`, `RotateByDirection.TiltsAboutTheAxisAtTheGivenHeading` |
| FR-8 | `RotateTriangleByDirection.KeepsTheSidesOfTheTriangle` |
| FR-9 | `CalculateTriangle.PlacesAnEquilateralTriangleAroundTheOrigin`, `CalculateTriangle.ASideOfZeroIsAPoint` |
| FR-10 | `GetHeights.AnswersTheZOfEachVertex` |
| FR-27 | `test_the_list_has_a_known_shape`, `test_every_entry_is_complete` |

2. Manual checks for what no test covers:
   - FR-12: `.\recipe.ps1 run` prints heights close to 0, -1.5 and 1.5 (`docs/manual.md:31`);
   - FR-13 to FR-15: from `python/`, `python main.py` prints the same heights (seen on 2026-11-18 and 2026-11-20);
   - FR-24: `.\recipe.ps1 info` prints `Version : 1.0.0` and `Provides: include, src` (seen on 2026-11-18 and 2026-11-20);
   - FR-38: `python tools/wiki.py --sha <sha>` lists the pages without writing (seen on 2026-11-18 and 2026-11-20: 5 pages);
   - FR-28 to FR-35: only with a Gitea server (and for FR-31 to FR-35 a runner) and a key; not checked here;
   - FR-36, FR-37: local hooks; `commit-msg.ps1` was run by hand on 2026-11-18 and 2026-11-20 (see FR-36), `pre-push.ps1` and the Claude Code hooks were not.

## 9. Open questions

1. Which `SPEC.md` stays? Settled on this branch on 2026-11-20: one file. The first version (one `##` section per task, mirrored as checkboxes of Jira KAN-59) and the requirements specification written from `release` on 2026-11-18 are merged here; the done tasks are the subsections 3.1 to 3.8 of section 3, the open ones the rows of 3.9. Whether the pull request into `test` is merged is open.
2. Who uses the library, and through what: which lab project, if any, takes it via `publish` and `$Needs`?
3. Is `python/rotate.dll` meant to be rebuilt from `src/rotate.c`, and by which command? Is `bin/main.exe` still needed? Partial answer, inference: the export table of `python/rotate.dll` gives its DLL name as `main.dll`, the `Makefile`'s output name (`Makefile:10`), so the committed file was most likely built by `make` and copied; no rule in the repository does that copy.
4. Does anything rely on the current results of the Python X and Y point wrappers?
5. What do `angle_a` and `angle_b` mean as a physical direction (heading and tilt per `README.md:3`; `direction` and `tilt` in `python/triangle_simulator.py:124-125`), and which sign convention is intended? Partial answer from the code: `angle_a` is the heading and `angle_b` the tilt, and the test fixes the tilt's sign: at heading 0 a tilt of +90 takes `(1, 0, 0)` to `(0, 0, -1)` (`tests/rotate_test.cpp:129-138`). Whether that sign is the one intended is still open.
6. Why are `cos 30` and `1/sqrt(3)` stored to 3 and 5 digits, and what precision is required?
7. Why is the side an integer from 0 to 255?
8. Does a Gitea runner currently serve this repository? The GitHub copy runs nothing. Partial answer: one did on 2026-11-08: commit `aa49d43` "docs(docs): changelog for v1.0.0" by `lab-bot`, under three minutes after the merge `5062236` into `release`, is what the `release` workflow's changelog step writes (`.gitea/workflows/release.yaml:402-418`); it is the tip of `test` and the parent of this branch's commit `a52fce0`, while `5062236` itself is on `release` and not on `test`. Whether a runner serves it now is unknown.
9. What do lab requirements revision 4.0.0 and the excepted rules R-02 to R-42 say? The text is on a server outside this repository.
10. Which Blender version is the script for?
11. Is the CMake project version `1.0` or the recipe's `1.0.0` authoritative? Answered by the code: the recipe's. The `release` workflow tags and names the release from `$Version` (`.gitea/workflows/release.yaml:45-60`, `292`; tag `v1.0.0`), `publish` uses it for the folder name (`recipe.ps1:95`), and nothing reads the CMake project version (`git grep` finds no `PROJECT_VERSION` or `TriangleRotation_VERSION`; the CMake version is declared at `CMakeLists.txt:2` and used nowhere).
