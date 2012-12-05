bin_PROGRAMS = iperf

LIBCOMPAT_LDADDS = @STRIP_BEGIN@ \
		   $(top_builddir)/compat/libcompat.a \
		   @STRIP_END@

AM_CPPFLAGS = @STRIP_BEGIN@ \
	      -I$(top_srcdir)/include \
	      -I$(top_builddir)/include \
	      @STRIP_END@
if LIBOML2
AM_CPPFLAGS += -I$(top_builddir)/oml
endif LIBOML2

AM_CXXFLAGS = -Wall
AM_CFLAGS = -Wall

iperf_LDFLAGS = @CFLAGS@ @PTHREAD_CFLAGS@ @WEB100_CFLAGS@ @DEFS@

iperf_SOURCES = \
		Client.cpp \
		Extractor.c \
		Launch.cpp \
		List.cpp \
		Listener.cpp \
		Locale.c \
		PerfSocket.cpp \
		ReportCSV.c \
		ReportDefault.c \
		Reporter.c \
		Server.cpp \
		Settings.cpp \
		SocketAddr.c \
		gnu_getopt.c \
		gnu_getopt_long.c \
		main.cpp \
		service.c \
		sockets.c \
		stdio.c \
		tcp_window_size.c
if LIBOML2
nodist_iperf_SOURCES = $(top_builddir)/oml/iperf_oml.h
iperf_SOURCES += ReportOML.c
endif LIBOML2


iperf_LDADD = $(LIBCOMPAT_LDADDS)
