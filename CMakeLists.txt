cmake_minimum_required(VERSION 3.20)
project(conan_cpp_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)

# Find only the packages we actually link against
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)
find_package(sqlite3 REQUIRED)
find_package(cxxopts REQUIRED)

add_library(example_lib
	src/http_client.cpp
	src/sql_runner.cpp
	src/json_utils.cpp
)

target_include_directories(example_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(example_lib
	PUBLIC
		fmt::fmt
		spdlog::spdlog
		nlohmann_json::nlohmann_json
		cpr::cpr
		sqlite3::sqlite3
)

add_executable(conan_cpp_example
	src/main.cpp
)

target_link_libraries(conan_cpp_example
	PRIVATE
		example_lib
		cxxopts::cxxopts
)

install(TARGETS conan_cpp_example RUNTIME DESTINATION bin)


