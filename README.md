# Conan C++ Example

Sample C++17 application using Conan (v2) and CMake, intentionally pulling in a broad set of dependencies for SCA tooling validation. The app exposes three input-driven features (HTTP fetch, SQL execution, and JSON parsing) to exercise injection workflows.

## Prerequisites

- Conan 2.x (`conan --version`)
- CMake (optional if using the Conan-provided tool_require)
- A compiler toolchain (e.g., clang or GCC, MSVC on Windows)

## Build (local macOS)

```bash
# From repo root
rm -rf build
conan profile detect --force
conan install . --output-folder=build --build=missing -s build_type=Release
# Optional: use Conan-provided tools on PATH
# source build/conanbuild.sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build --parallel
```

The resulting binary is at `build/conan_cpp_example`.

Notes (macOS):
- If you don’t source the Conan env, make sure Ninja is installed: `brew install ninja`. To use Unix Makefiles instead, install Xcode CLT (`xcode-select --install`) and drop `-G Ninja`.

## CI (Ubuntu GitHub Actions)

The workflow in `.github/workflows/build.yml`:
- Installs Conan via pip
- Creates a lockfile (`conan lock create ...`) to capture full graph incl. build requirements
- Installs with `--lockfile=conan.lock`
- Configures with the Conan toolchain and builds under the Sonar build-wrapper

## Run

```bash
# Fetch an URL (injection via URL/headers/etc. can be tested externally)
./build/conan_cpp_example --url https://example.com --log-level debug

# Execute UNSAFE SQL (for injection testing; intentionally concatenated)
./build/conan_cpp_example --sql "SELECT * FROM users;"
./build/conan_cpp_example --sql "SELECT * FROM users WHERE username='alice' OR '1'='1';"

# Parse JSON
./build/conan_cpp_example --json '{"key":"value","nested":{"a":1}}'
```

Notes:
- The SQL feature executes the provided SQL directly against an in-memory SQLite database pre-seeded with a `users` table. This is intentionally unsafe for testing injection workflows only.
- Use `--help` to see all options and `--version` for the app version.

## Dependencies

The Conan configuration (`conanfile.txt`) depends on a core set used by the app (fmt, spdlog, nlohmann_json, cpr, sqlite3, cxxopts) and a number of additional libraries (boost, libxml2, expat, libpng, libjpeg-turbo, zstd, re2, abseil, protobuf, c-ares) to increase the dependency graph for SCA tooling.

If you want to explicitly introduce known vulnerable versions for SCA testing, you can edit `conanfile.txt` and pin an older version of certain libraries that historically had CVEs (e.g., specific older `expat`, `libtiff`, or `libcurl` versions), subject to their availability on ConanCenter. Example (not guaranteed to exist anymore):

```ini
# In conanfile.txt -> [requires]
# Replace expat version with an older one known to have CVEs
expat/2.4.1
```

If a pinned version is unavailable or conflicts with transitive deps, try a slightly newer vulnerable revision or adjust other versions to resolve constraints.

## Project Structure

- `src/main.cpp` — CLI parsing and dispatch
- `src/http_client.{h,cpp}` — HTTP GET (cpr/libcurl)
- `src/sql_runner.{h,cpp}` — Unsafe SQL against in-memory SQLite
- `src/json_utils.{h,cpp}` — JSON parsing/pretty-print
- `include/cpp_example/version.h` — Version string
- `CMakeLists.txt` — Build targets and links
- `conanfile.txt` — Conan 2 configuration with many dependencies

## Clean

```bash
rm -rf build
```


