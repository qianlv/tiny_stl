
AM_CXXFLAGS = --std=c++11

SOURCES = unit_test.cpp \
		  test_alloc.cpp

noinst_PROGRAMS = unit_test
unit_test_SOURCES = $(SOURCES)
unit_test_LDADD = -lgtest 
unit_test_LDFLAGS = -pthread
unit_test_CPPFLAGS = -I$(srcdir)/tinyStl
