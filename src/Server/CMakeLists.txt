project(Server)

set(sources
	main.cpp
	)

Dominion_Executable(TARGET Server
	SOURCES ${sources}
	DEPENDS Net
)
