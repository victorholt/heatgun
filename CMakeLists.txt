cmake_minimum_required(VERSION 3.10)
project(HeatGun)

set(CMAKE_CXX_STANDARD 17)

# Setup the build locations.
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DHEATGUN_DEBUG)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/builds/debug)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/builds/debug)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/builds/debug)
else ()
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/builds/release)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/builds/release)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/builds/release)
endif ()

# Apple-specific libraries.
list (APPEND GameAppleLibs "")
if (APPLE)
    if (IOS OR TVOS)
        list (APPEND GodotAppleLibs
            "-framework Foundation"
            "-framework QuartzCore"
        )
    else ()
        list (APPEND GodotAppleLibs
            "-framework CoreFoundation"
            "-framework SystemConfiguration"
            "-framework CoreServices"
        )
    endif ()
endif ()

# Set the base directory for the source.
set (GODOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
set (HEATGUN_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Setup our common directory.
set (GODOT_INCLUDES
    "${GODOT_SOURCE_DIR}/godot-cpp/include"
    "${GODOT_SOURCE_DIR}/godot-cpp/include/core"
    "${GODOT_SOURCE_DIR}/godot-cpp/godot_headers"
    "${HEATGUN_SRC_DIR}/common"
    "${HEATGUN_SRC_DIR}"
)
set (HEATGUN_SOURCES)

# Setup our includes.
include_directories(${GODOT_INCLUDES})

include(${GODOT_SOURCE_DIR}/CMakeLists.txt)
include(${HEATGUN_SRC_DIR}/opensimplex/CMakeLists.txt)
include(${HEATGUN_SRC_DIR}/common/heatgun/CMakeLists.txt)
include(${HEATGUN_SRC_DIR}/map_generator/CMakeLists.txt)
include(${HEATGUN_SRC_DIR}/dungeon_map/CMakeLists.txt)

#add_executable(GodotModules ${GODOT_CPP_SOURCES} main.cpp)
