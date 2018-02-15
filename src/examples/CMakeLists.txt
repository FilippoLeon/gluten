macro(add_example source)
	get_filename_component(name ${source} NAME_WE)
	message(STATUS "Addin' example '${name}'.")
	add_executable(${name} ${source})

	set_property(TARGET ${name} PROPERTY CXX_STANDARD 17)
	set_property(TARGET ${name} PROPERTY CXX_STANDARD_REQUIRED ON)

	target_link_libraries(${name} PRIVATE gluten)
endmacro()

file(GLOB EXAMPLES "*.cpp")
foreach(EX ${EXAMPLES})
	add_example(${EX}) 
endforeach() 