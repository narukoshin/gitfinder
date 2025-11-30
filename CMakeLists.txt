cmake_minimum_required(VERSION 3.20)
project(GoogleScraper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Your sacred CPR settings
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(BUILD_CPR_TESTS OFF CACHE BOOL "" FORCE)

include(FetchContent)

# === 1. CPR ===
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.13.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(cpr)

# === 2. yaml-cpp (optional) ===
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(yaml-cpp)

add_executable(main src/main.cpp)
target_link_libraries(main 
    PRIVATE 
    cpr::cpr 
    yaml-cpp::yaml-cpp
)

target_compile_options(main PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)