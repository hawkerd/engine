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
set(ASSETS_DIR assets)
set(BIN_DIR bin)
set(INCLUDE_DIR include)
set(LIB_DIR libs)

# source files
set(SOURCES 
    ${SRC_DIR}/renderer.cpp 
    ${SRC_DIR}/stb_image.cpp 
    ${SRC_DIR}/glad.c
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/model.cpp
    ${SRC_DIR}/mesh.cpp
    ${SRC_DIR}/vao.cpp
    ${SRC_DIR}/vbo.cpp
    ${SRC_DIR}/ebo.cpp
    ${SRC_DIR}/vertex.cpp
)

# main executable
add_executable(renderer ${SOURCES})

# include directories
target_include_directories(renderer PRIVATE ${CMAKE_SOURCE_DIR}/${INCLUDE_DIR})

# link library directories
target_link_directories(renderer PRIVATE ${CMAKE_SOURCE_DIR}/${LIB_DIR}/lib)

# link libaries
target_link_libraries(renderer glfw3dll opengl32 gdi32 assimp)

# move libraries into bin dir at runtime
add_custom_command(TARGET renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/${LIB_DIR}/bin/glfw3.dll
        ${CMAKE_SOURCE_DIR}/${LIB_DIR}/bin/libassimp-5.dll
        ${CMAKE_BINARY_DIR}/${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${ASSETS_DIR}
        ${CMAKE_BINARY_DIR}/${BIN_DIR}/${ASSETS_DIR}
)

# output dir
set_target_properties(renderer PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)

# Custom target to run the executable
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${BIN_DIR}/renderer
    DEPENDS renderer
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
    COMMENT "Running renderer..."
)