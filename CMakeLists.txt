cmake_minimum_required(VERSION 3.9)
project(Gluten VERSION 0.0.1 LANGUAGES C CXX)

#find_package(GLEW REQUIRED)
#find_package(GLFW3 REQUIRED)

message(STATUS "Build directory (thanks, MNVS): ${CMAKE_BINARY_DIR}")

# Download glad: file:///home/filippo/Workspace/gluten/glad
set(GLAD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glad")
add_subdirectory(${GLAD_ROOT})

set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.2.1")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(glfw3)
if(NOT glfw3_FOUND)
    set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
    add_subdirectory(${GLFW_ROOT})
endif(NOT glfw3_FOUND)

find_package(glm)
if(NOT glm_FOUND)
    set(GLM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glm")
    add_subdirectory(${GLM_ROOT})
endif(NOT glm_FOUND)

set(FMT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/fmt")
add_subdirectory(${FMT_ROOT} EXCLUDE_FROM_ALL)

find_package(Freetype)
if(NOT Freetype_FOUND)
    set(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/freetype-2.9")
    add_subdirectory(${FREETYPE_ROOT})
else()
    message(STATUS "Found Freetype at ${FREETYPE_INCLUDE_DIRS}")
    message(STATUS "Found Freetype build include at ${FREETYPE_INCLUDE_DIR_ft2build}")
    message(STATUS "Found Freetype inlcude at ${FREETYPE_INCLUDE_DIR_freetype2}")
endif(NOT Freetype_FOUND)

### COMPILER SPECIFIC FLAGS
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_definitions(-Wall -Wextra -pedantic)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    add_definitions(-Wall -Wextra -pedantic)
    # Fix for experimental folder
    add_definitions(-DEXPERIMENTAL_FILESYSTEM)
endif()

add_subdirectory(src)