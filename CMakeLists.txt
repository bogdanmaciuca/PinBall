cmake_minimum_required(VERSION 3.15)
project(OpenGL_App LANGUAGES CXX)

# Compiler flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -O0 -g)
add_compile_definitions(GLFW_INCLUDE_NONE)

# Source files
file(GLOB SRCS src/*.cpp)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory(vendor/glad REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${SRCS})

# PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    glfw
    OpenGL::GL
    glad
    ${CMAKE_DL_LIBS}  # For Linux DL library
)

