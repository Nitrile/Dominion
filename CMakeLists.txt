project(Dominion)
cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCE_DIR     ${CMAKE_SOURCE_DIR}/src)
set(PROJECT_TEST_DIR       ${CMAKE_SOURCE_DIR}/test)

# Build paths
if (CMAKE_BUILD_TYPE MATCHES Debug)
	set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin_debug)
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
	set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin_exp)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
	set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin_release)
else ()
	message(FATAL_ERROR "Unknown build type " ${CMAKE_BUILD_TYPE})
endif()

set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR}/app)
set(LIBRARY_OUTPUT_PATH    ${BIN_DIR}/lib)
#set(DEPLOY_OUTPUT_PATH     ${BIN_DIR}/deploy)
set(CMAKE_BINARY_DIR       ${BIN_DIR}/build)

add_subdirectory(${PROJECT_SOURCE_DIR})
add_subdirectory(${PROJECT_TEST_DIR})
