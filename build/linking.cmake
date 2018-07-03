
include(CMakeParseArguments)

# Use this macro to define a library.
#
# Example:
#
# Builder_Library(TARGET IO
#                  HEADERS save.hpp
#                  SOURCES save.cpp
#                  DEPENDS ...
#                 )
#

macro(Builder_EnsureNonempty ${TARGET})
	if (TARGET)
	elseif()
		message(FATAL_ERROR Target is empty: ${TARGET})
	endif()
endmacro()

function(Builder_Link TARGET DEPENDS)
	Builder_EnsureNonempty(${TARGET})

	if (DEPENDS)
		add_dependencies(${TARGET} ${DEPENDS})
		target_link_libraries(${TARGET} ${DEPENDS})
		foreach (dependency ${DEPENDS})
			target_include_directories(${TARGET}
				PUBLIC $<TARGET_PROPERTY:${dependency},INTERFACE_INCLUDE_DIRECTORIES>
			)
		endforeach ()
	endif()
endfunction()

function(Builder_Library)
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

	Builder_Link(${Lib_TARGET} "${Lib_DEPENDS}")

	get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

	target_include_directories(${Lib_TARGET}
		INTERFACE ${PARENT_DIR}
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	)

endfunction()

function(Builder_Executable)
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

	Builder_Link(${Exe_TARGET} "${Exe_DEPENDS}")
endfunction()

function(Builder_Test)
	set(arg_oneval NAME)
	set(arg_mutlival SOURCES DEPENDS)
	cmake_parse_arguments(Exe
		""
		"${arg_oneval}"
		"${arg_mutlival}"
		${ARGN})

	set(targetname "Test_${Exe_NAME}")

	add_executable(${targetname}
		${Exe_SOURCES}
	)
	add_test(${targetname}
		${TEST_OUTPUT_PATH}/${targetname}
	)

	set_target_properties(${targetname}
		PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH}
	)

	Builder_Link(${targetname} "${Exe_DEPENDS}")
endfunction()
