# Header and src files.
file (GLOB GODOT_CPP_HEADERS
    "${GODOT_SOURCE_DIR}/godot-cpp/include/*.h"
    "${GODOT_SOURCE_DIR}/godot-cpp/include/core/*.h"
)

file (GLOB GODOT_CPP_SRC
    "${GODOT_SOURCE_DIR}/godot-cpp/src/*.cpp"
    "${GODOT_SOURCE_DIR}/godot-cpp/src/core/*.cpp"
)

add_library(GodotCppLib
    ${GODOT_CPP_HEADERS} ${GODOT_CPP_SRC}
)

target_link_libraries(
    GodotCppLib
    ${GodotAppleLibs}
)

# Include our src files.
list(APPEND GODOT_CPP_SOURCES ${GODOT_CPP_HEADERS})
