cmake_minimum_required(VERSION 3.16)

project(CprDemo
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# I'm too dumb to fix this issue during compiling
# So i just disable it. if it works don't touch it
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.12.0
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        0.8.0          # latest stable as of 2025
    # or use "master" for bleeding edge
)
FetchContent_MakeAvailable(yaml-cpp)

add_executable(main main.cpp)
target_link_libraries(main 
    PRIVATE 
        cpr::cpr 
        yaml-cpp
)