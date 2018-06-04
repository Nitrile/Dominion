project(Server)

set(source
	main.cpp
	)

add_executable(Server ${source})
target_include_directories(Server PRIVATE ${PROJECT_LIB_DIR})

# Link library dependencies
target_link_libraries(Server PRIVATE Net)
