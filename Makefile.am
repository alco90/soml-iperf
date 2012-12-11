ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign
SUBDIRS = compat doc include oml src man

EXTRA_DIST = autogen.sh \
	     autoclean.sh \
	     m4/gnulib-cache.m4 \
	     $(top_srcdir)/.version \
	     configure.gnu

if LIBOML2
# Phony target for Android OML2 packaging
iperf_oml.h:
	$(MAKE) -C oml iperf_oml.h

.PHONY: iperf_oml.h
endif LIBOML2

BUILT_SOURCES = $(top_srcdir)/.version

$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

# Not sure why automake does net clean these anymore...
CLEANFILES = depcomp config.guess install-sh config.sub missing

AM_CPPFLAGS = @STRIP_BEGIN@ \
	      -I$(top_srcdir)/include \
	      -I$(top_builddir)/include \
	      @STRIP_END@

AM_CXXFLAGS = -Wall
AM_FLAGS = -Wall
