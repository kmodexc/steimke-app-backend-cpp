# Repository Guidelines

## Project Structure & Module Organization

This is a C++ REST server built with CMake. The executable entry point is `main.cpp`; reusable server code is in `rlservlib/`. Within that library, `models/` contains domain types, `datastore/` handles SQLite persistence, `jsonconv/` handles JSON serialization, and `webservice/` provides HTTP and socket adapters. Vendored or bundled dependencies live under `rlservlib/modules/`. GoogleTest sources and the test executable are under `gtest/`. CMake helper modules are in `cmake/`.

## Build, Test, and Development Commands

Use an out-of-source build:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The final command runs the registered `TEST_ALL` suite; `build/gtest/gtest_all` can be run directly when debugging tests. `docker compose up --build` builds the Ubuntu-based image and exposes the HTTP service on host port `30000`, forwarding to container port `80`.

## Coding Style & Naming Conventions

Keep the existing C++ style: four-space or tab indentation consistent with the surrounding file, braces on the same line for functions and tests, and descriptive PascalCase class names (for example, `JSONSerializer` and `MockConHandle`). Use lower camel case for methods and variables where established. Place declarations in `.h` files and implementations in matching `.cpp` files. No repository-wide formatter or linter is configured, so keep changes narrowly formatted and avoid unrelated reformatting.

## Testing Guidelines

Add GoogleTest cases in the relevant `gtest/*tests.cpp` file and name tests as `TEST(SuiteName, CaseName)`. Cover API behavior, persistence, and serialization changes at the layer they affect. Build first, then run `ctest --test-dir build --output-on-failure`; tests may create `logs/` and SQLite data under the build directory.

## Commit & Pull Request Guidelines

Recent commits use short, lowercase summaries such as `changed ports` and `updated readme`. Follow that concise style while stating the user-visible change. Pull requests should explain the behavior change, list validation commands and results, link the relevant issue when one exists, and include request/response examples or screenshots for API or deployment changes.

## Security & Configuration Tips

Never commit TLS keys, certificates, SQLite files, logs, or build output; the repository ignores these patterns. TLS termination is expected to happen at an upstream proxy, so keep the proxy-to-server network private or otherwise protected.
