AM_CPPFLAGS = $(CXX17_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

common_source = node.hh node.cc \
	        flow.hh flow.cc \
	        parking_lot.hh parking_lot.cc \
	        optimizer.hh optimizer.cc

bin_PROGRAMS = pcc_convergence

pcc_convergence_SOURCES = pcc_convergence.cc $(common_source)
