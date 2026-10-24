cmake_minimum_required(VERSION 3.16)
project(opengl-playground C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE GLPG_SOURCES CONFIGURE_DEPENDS src/*.cc)
add_executable(opengl-playground ${GLPG_SOURCES} vendors/glad/src/gl.c)
target_include_directories(opengl-playground PRIVATE src vendors/glad/include)

target_link_libraries(opengl-playground PUBLIC glfw OpenGL::GL)