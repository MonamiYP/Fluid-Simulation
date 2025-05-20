# ────────────────────────────────────────────────
# Compiler settings
# ────────────────────────────────────────────────

cmake_minimum_required(VERSION 3.5.0)
project(Fluid_Sim)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# ────────────────────────────────────────────────
# Files
# ────────────────────────────────────────────────
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE IMGUI lib/imgui/*.cpp)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    lib/glad/glad.c
    ${IMGUI}
    ${SRC}
)

add_subdirectory(lib/glfw)

target_include_directories(${PROJECT_NAME}
    PUBLIC lib/glfw/include
    PUBLIC res
    PRIVATE lib
    PRIVATE include
    PRIVATE include/core
    PRIVATE include/graphics
    PRIVATE include/openGL
    PRIVATE include/physics
)

target_link_libraries(${PROJECT_NAME} glfw)

# ───────────────
# Copy `res/` next to binary
# ───────────────
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)