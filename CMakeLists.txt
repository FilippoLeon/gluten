cmake_minimum_required(VERSION 2.8)

project(gluten)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(GLM REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLM REQUIRED glm)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

add_executable(${PROJECT_NAME} "main.cpp")

message(STATUS ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLM_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${GLM_LIBRARIES} ${OPENGL_LIBRARIES})
