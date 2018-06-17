# Header and src files.
file (GLOB OPENSIMPLEX_HEADERS
    "${HEATGUN_SRC_DIR}/opensimplex/*.h"
    "${HEATGUN_SRC_DIR}/opensimplex/lib/*.h"
)

file (GLOB OPENSIMPLEX_SRC
    "${HEATGUN_SRC_DIR}/opensimplex/*.cpp"
    "${HEATGUN_SRC_DIR}/opensimplex/lib/*.c"
)

add_library(OpenSimplex SHARED
    ${OPENSIMPLEX_HEADERS} ${OPENSIMPLEX_SRC}
)

target_link_libraries(
    OpenSimplex
    ${GodotAppleLibs}
    GodotCppLib
)

if (WIN32)
    set_property(TARGET OpenSimplex PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/OpenSimplex.dll)
    set_property(TARGET OpenSimplex PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/OpenSimplex.a)
else ()
    set_property(TARGET OpenSimplex PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/builds/${CMAKE_BUILD_TYPE}/OpenSimplex.dylib)
endif ()
set_property(TARGET OpenSimplex PROPERTY COMPILE_DEFINITIONS)
