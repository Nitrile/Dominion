project(Server)

set(sources
	main.cpp
	)

Builder_Executable(TARGET Server
	SOURCES ${sources}
	DEPENDS Net
)
