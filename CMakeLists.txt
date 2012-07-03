cmake_minimum_required(VERSION 2.8)
project(Ditto C)

find_package(SDL REQUIRED)
if (${SDL_FOUND} EQUAL "NO")
   message(FATAL_ERROR "'SDL' not found")
endif()
include_directories(${SDL_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
if (${OPENGL_FOUND} EQUAL "NO")
   message(FATAL_ERROR "'OpenGL' not found")
endif()
include_directories(${OpenGL_INCLUDE_DIR})

#find_package(GLU REQUIRED)
#if (${GLU_FOUND} EQUAL "NO")
#   message(FATAL_ERROR "'GLU' not found")
#endif()
#include_directories(${GLU_INCLUDE_DIR})

include_directories(include)
add_definitions(-g)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH ${Ditto_BINARY_DIR}/bin)

add_subdirectory(lib/Ditto lib)
add_subdirectory(src/Mew src)
