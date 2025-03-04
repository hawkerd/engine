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
set(LIBS_DIR C:/Code/libs)

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
target_include_directories(renderer PRIVATE 
    ${LIBS_DIR}/include
    ${CMAKE_SOURCE_DIR}/${INCLUDE_DIR}
)


# link library directories
target_link_directories(renderer PRIVATE ${LIBS_DIR}/lib)

# link libaries
target_link_libraries(renderer glfw3 opengl32 gdi32 assimp)

# move libraries and assets into bin dir at runtime
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${ASSETS_DIR}
        ${CMAKE_BINARY_DIR}/${BIN_DIR}/${ASSETS_DIR}
    COMMENT "Copying assets and libraries..."
)

# output dir
set_target_properties(renderer PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
)

# Custom target to run the executable
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${BIN_DIR}/renderer
    DEPENDS renderer copy_assets
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR}
    COMMENT "Running renderer..."
)