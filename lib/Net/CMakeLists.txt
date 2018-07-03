
project(Net)

set(sources
	net.cpp
	)

set(headers
	net.hpp
	)

Builder_Library(TARGET Net
	SOURCES ${sources}
	HEADERS ${headers}
)
