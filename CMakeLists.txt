cmake_minimum_required(VERSION 3.20)
project(GoogleScraper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# adding all libraries to the executables
if(UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
    set(CURL_STATICLIB ON CACHE BOOL "" FORCE)
    set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
    set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL ON CACHE BOOL "" FORCE)
    set(OPENSSL_USE_STATIC_LIBS ON CACHE BOOL "" FORCE)
endif()

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
    set(CURL_STATICLIB ON CACHE BOOL "" FORCE)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
endif()

include(FetchContent)

# === CPR (with static curl) ===
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.13.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(cpr)

# === yaml-cpp (static) ===
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
    GIT_SHALLOW ON
)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(yaml-cpp)

add_executable(main src/main.cpp)

target_link_libraries(main
    PRIVATE
        cpr::cpr
        yaml-cpp::yaml-cpp
)

if(UNIX AND NOT APPLE)
    target_link_libraries(main PRIVATE
        pthread
        dl
        ssl
        crypto
        z
    )
endif()