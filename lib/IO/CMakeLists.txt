
project(IO)

set(sources
	save.cpp
	)
set(headers
	save.hpp
	)

Builder_Library(TARGET IO
	SOURCES ${sources}
	HEADERS ${headers}
)
