file(GLOB SOURCES "**/*.cpp")
file(GLOB HEADERS "**/*.hpp")

add_library(gluten ${SOURCES})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(FREETYPE_FOUND)
	target_include_directories(gluten PRIVATE "${FREETYPE_INCLUDE_DIR_ft2build}")
else()
	target_include_directories(gluten PRIVATE "${CMAKE_SOURCE_DIR}/freetype-2.9/include")
endif()

# Support for experimental filesystem
if(${CMAKE_CXX_COMPILER_ID} STREQUA "GNU")
	target_link_libraries(gluten PRIVATE stdc++fs)
endif()

get_filename_component(CURRENT_DIRECTORY_PARENT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
#target_include_directories(gluten PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(gluten PUBLIC ${CURRENT_DIRECTORY_PARENT} "${CMAKE_SOURCE_DIR}/lib/include")
target_link_libraries(gluten PUBLIC glad glfw glm fmt-header-only) 
target_link_libraries(gluten PRIVATE freetype) 

target_compile_features(gluten PUBLIC cxx_attributes)

set_property(TARGET gluten PROPERTY CXX_STANDARD 17)
set_property(TARGET gluten PROPERTY CXX_STANDARD_REQUIRED ON)
 
# if(CMAKE_VERSION VERSION_LESS 3.10 AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# 	message(STATUS "Handpicking c++17 flag.")   
# 	target_compile_definitions(gluten PUBLIC "/std:c++17")
# endif()   

add_definitions(-DGLUTEN_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_definitions(gluten PUBLIC USE_FMT=0)
else()
	target_compile_definitions(gluten PUBLIC USE_FMT=1)
endif()

file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shader/examples/*")
file(COPY ${SHADERS} DESTINATION "${CMAKE_BINARY_DIR}/shader/examples/")
message(STATUS "Copied shaders ${SHADERS} to ${CMAKE_BINARY_DIR}/shader/examples/")

add_custom_command(TARGET gluten PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${SHADERS} "${CMAKE_BINARY_DIR}/shader/examples/"
)

# export(TARGETS gluten glad glfw glm fmt-header-only freetype FILE GlutenLib.cmake)