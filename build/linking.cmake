
include(CMakeParseArguments)

# Use this macro to define a library.
#
# Example:
#
# Dominion_Library(TARGET IO
#                  HEADERS save.hpp
#                  SOURCES save.cpp
#                 )
#
function(Dominion_Library)
	set(arg_oneval TARGET)
	set(arg_mutlival HEADERS SOURCES DEPENDS)
	cmake_parse_arguments(Lib
		""
		"${arg_oneval}"
		"${arg_mutlival}"
		${ARGN}
	)

	add_library(${Lib_TARGET}
		SHARED ${Lib_SOURCES}
	)

	if (Lib_DEPENDS)
		add_dependencies(${Lib_TARGET} ${Lib_DEPENDS})
		target_link_libraries(${Lib_TARGET} ${Lib_DEPENDS})
		foreach (TARGET ${Lib_DEPENDS})
			target_include_directories(${Lib_TARGET}
				PUBLIC $<TARGET_PROPERTY:${TARGET},INTERFACE_INCLUDE_DIRECTORIES>
			)
		endforeach ()
	endif()
	get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

	message(Lib: ${Lib_TARGET})
	message(Interface: ${PARENT_DIR})
	message(Private: ${CMAKE_CURRENT_SOURCE_DIR})

	target_include_directories(${Lib_TARGET}
		INTERFACE ${PARENT_DIR}
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	)

endfunction()

function(Dominion_Executable)
	set(arg_oneval TARGET)
	set(arg_mutlival SOURCES DEPENDS)
	cmake_parse_arguments(Exe
		""
		"${arg_oneval}"
		"${arg_mutlival}"
		${ARGN})

	add_executable(${Exe_TARGET}
		${Exe_SOURCES}
	)

	if (Exe_DEPENDS)
		add_dependencies(${Exe_TARGET} ${Exe_DEPENDS})
		target_link_libraries(${Exe_TARGET} ${Exe_DEPENDS})
		# At the moment this
		foreach (TARGET ${Exe_DEPENDS})
			target_include_directories(${Exe_TARGET}
				PUBLIC $<TARGET_PROPERTY:${TARGET},INTERFACE_INCLUDE_DIRECTORIES>
			)
		endforeach ()
	endif()
endfunction()
