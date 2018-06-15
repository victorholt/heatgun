# Header and src files.
file (GLOB MAP_GENERATOR_HEADERS
    "${HEATGUN_SRC_DIR}/map_generator/*.h"
    "${HEATGUN_SRC_DIR}/map_generator/*.hpp"
)

file (GLOB MAP_GENERATOR_SRC
    "${HEATGUN_SRC_DIR}/map_generator/*.cpp"
)

add_library(MapGenerator SHARED
    ${OPENSIMPLEX_HEADERS} ${OPENSIMPLEX_SRC} ${MAP_GENERATOR_HEADERS} ${MAP_GENERATOR_SRC}
)

target_link_libraries(
    MapGenerator
    ${GodotAppleLibs}
    GodotCppLib
)

if (WIN32)
    set_property(TARGET MapGenerator PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/MapGenerator.dll)
    set_property(TARGET MapGenerator PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/MapGenerator.a)
else ()
    set_property(TARGET MapGenerator PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/MapGenerator.dylib)
endif ()
set_property(TARGET MapGenerator PROPERTY COMPILE_DEFINITIONS)
