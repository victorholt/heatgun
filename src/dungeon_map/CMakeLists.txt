# Header and src files.
file (GLOB DUNGEON_MAP_HEADERS
    "${HEATGUN_SRC_DIR}/dungeon_map/*.h"
    "${HEATGUN_SRC_DIR}/dungeon_map/*.hpp"
)

file (GLOB DUNGEON_MAP_SRC
    "${HEATGUN_SRC_DIR}/dungeon_map/*.cpp"
)

add_library(DungeonMap SHARED
    ${DUNGEON_MAP_HEADERS} ${DUNGEON_MAP_SRC}
)

target_link_libraries(
    DungeonMap
    ${GodotAppleLibs}
    GodotCppLib
)

if (WIN32)
    set_property(TARGET DungeonMap PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/DungeonMap.dll)
    set_property(TARGET DungeonMap PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/DungeonMap.a)
else ()
    set_property(TARGET DungeonMap PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/DungeonMap.dylib)
endif ()
set_property(TARGET DungeonMap PROPERTY COMPILE_DEFINITIONS)
