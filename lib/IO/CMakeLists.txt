
project(IO)

set(sources
	save.cpp
	)
set(headers
	save.hpp
	)

Dominion_Library(TARGET IO
	SOURCES ${sources}
	HEADERS ${headers}
)
