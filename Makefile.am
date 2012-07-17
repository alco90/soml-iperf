AUTOMAKE_OPTIONS = foreign
SUBDIRS = compat doc include src man oml

# Not sure why automake does net clean these anymore...
CLEANFILES = depcomp config.guess install-sh config.sub missing

AM_CPPFLAGS = @STRIP_BEGIN@ \
	      -I$(top_srcdir)/include \
	      -I$(top_builddir)/include \
	      @STRIP_END@

AM_CXXFLAGS = -Wall
AM_FLAGS = -Wall
