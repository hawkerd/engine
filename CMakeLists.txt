# cmake -S . -B build -G "MinGW Makefiles"
# cmake -S . -B build -G "Unix Makefiles"

# CMake version
cmake_minimum_required(VERSION 3.10)

# project name
project(GameEngine)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# aliases for directories
set(SRC_DIR src)
set(BIN_DIR bin)
set(INCLUDE_DIR include)
set(LIB_DIR libs)

# include directories
include_directories(${INCLUDE_DIR})

# source files
set(SOURCES 
    ${SRC_DIR}/renderer.cpp 
    ${SRC_DIR}/stb_image.cpp 
    ${SRC_DIR}/glad.c
)

# main executable
add_executable(renderer ${SOURCES})

# link GLFW and OpenGL
if (APPLE)
    target_link_libraries(renderer glfw OpenGL Cocoa IOKit)
else()
    target_link_directories(renderer PRIVATE ${LIB_DIR}/glfw/lib-mingw-w64)
    target_link_libraries(renderer glfw3 opengl32 gdi32)
endif()

# copy assets for easy access during runtime
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(renderer copy_assets)

# output dir
set_target_properties(renderer PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)

# Custom target to run the executable
add_custom_target(run
    COMMAND renderer
    DEPENDS renderer
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running renderer..."
)