cmake_minimum_required(VERSION 3.19)
project(godot_gameplay_attributes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## the name must be like lib{project_name}.{platform}.{debug/release}.{arch}.{ext}
## the platform are windows, linux, android, etc
## the envs are debug or release
## the arch is x86_64, arm32, arm64
## the ext is dll, so, dylib, etc

if (WIN32)
    set(PLATFORM "windows")
    set(EXT "dll")
elseif (UNIX AND NOT APPLE)
    ## use uname -m to get the actual arch
    set(PLATFORM "linux")
    set(EXT "so")
elseif (ANDROID)
    set(PLATFORM "android")
    set(EXT "so")
elseif (APPLE)
    set(PLATFORM "macos")
    set(EXT "dylib")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_RELEASE "debug")
else ()
    set(DEBUG_RELEASE "release")
endif ()

if (NOT DEFINED ARCH)
    if (CMAKE_SIZEOF_VOID_P GREATER 4)
        set(ARCH x86_64)
    else ()
        set(ARCH x86)
    endif ()
endif ()


include(FetchContent)

FetchContent_Declare(
        GDExtension
        GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
        GIT_TAG 4.4
)

FetchContent_MakeAvailable(GDExtension)

add_subdirectory(src)
