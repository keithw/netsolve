AM_CPPFLAGS = $(CXX17_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

common_source = node.hh node.cc \
	        flow.hh flow.cc \
	        parking_lot.hh parking_lot.cc \
	        optimizer.hh optimizer.cc

bin_PROGRAMS = demo

demo_SOURCES = demo.cc $(common_source)
