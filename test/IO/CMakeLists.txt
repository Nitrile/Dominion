
set(sources
	main.cpp
)

Builder_Test(NAME IO
	SOURCES ${sources}
	DEPENDS IO
)
