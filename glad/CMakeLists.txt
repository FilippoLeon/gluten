add_library(glad "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")
if(CMAKE_DL_LIBS)
    target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
endif()
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

export(TARGETS glad FILE glad.cmake) 