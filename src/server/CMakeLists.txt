project(Server)

set(source
	main.c++
	)

add_executable(Server ${source})

# Link library dependencies
target_link_libraries(Server PRIVATE Net)
