
set(sources
	main.cpp
	)

Builder_Executable(TARGET Client
	SOURCES ${sources}
	DEPENDS Net
)
