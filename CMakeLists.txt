cmake_minimum_required(VERSION 3.9)
project(Gluten VERSION 0.0.1 LANGUAGES C CXX)

#find_package(GLEW REQUIRED)
#find_package(GLFW3 REQUIRED)

message(STATUS "Build directory (thanks, MNVS): ${CMAKE_BINARY_DIR}")

set(GLAD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glad")
add_subdirectory(${GLAD_ROOT})

set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.2.1")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_ROOT})

set(GLM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glm")
add_subdirectory(${GLM_ROOT})

set(FMT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/fmt")
add_subdirectory(${FMT_ROOT} EXCLUDE_FROM_ALL)

set(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/freetype-2.9")
add_subdirectory(${FREETYPE_ROOT})

add_subdirectory(src)