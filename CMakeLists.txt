cmake_minimum_required(VERSION 3.10)
project(ogl_start)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SHADER_DIR "./shaders")

include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/libs/glm)

file(GLOB SOURCES "src/*.cpp" "src/*.c")  


add_subdirectory(libs/glfw)
include_directories(libs/glfw)
include_directories(libs/glfw/deps)
include_directories(libs/glfw/include)

add_subdirectory(libs/glm)

set(GLAD_SRC "./include/glad/glad.c")

set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")


set(SOURCE_FILES
    ./src/main.cpp
    ${GLAD_SRC}
)

add_executable(ogl_start ${SOURCE_FILES} ${TINYCTHREAD} ${GETOPT})

target_link_libraries(ogl_start glfw ${GLFW_LIBRARIES})


find_package(OpenGL REQUIRED)
target_link_libraries(ogl_start OpenGL::GL)

add_library(glad STATIC "./include/glad/glad.c")
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include/glad)
target_link_libraries(ogl_start glad)