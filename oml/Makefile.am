# Override autoconf's default pkgdatadir to honour program-{pre,suf}fix pas$(SED)
# to configure script
installed_name = `echo -n "$(PACKAGE)" | $(SED) '$(transform)'`
pkgdatadir = $(datadir)/$(installed_name)

# SED is otherwise provided by libtool.m4
SED=sed

SCAFFOLD = oml2-scaffold

BUILT_SOURCES = iperf.rb \
		iperf-5.3.rb

if LIBOML2
pkgdata_DATA = iperf-5.3.rb iperf.rb

BUILT_SOURCES += iperf_oml.h

install-data-hook:
	cd $(DESTDIR)$(pkgdatadir) && \
		$(LN_S) -f iperf.rb iperf-5.4.rb
uninstall-hook:
	rm -f $(DESTDIR)$(pkgdatadir)/iperf.rb
	rm -f $(DESTDIR)$(pkgdatadir)/iperf-5.3.rb
	rm -f $(DESTDIR)$(pkgdatadir)/iperf-5.4.rb
else !LIBOML2
pkgdata_DATA =
endif !LIBOML2

EXTRA_DIST= iperf.rb.in \
	    iperf-5.3.rb.in

CLEANFILES = iperf_oml.h \
	     iperf.rb \
	     iperf-5.3.rb

iperf.rb: iperf.rb.in
	VER=`echo $(VERSION) | $(SED) "s/.*+oml//;s/\([0-9]\{1,\}\(\.[0-9]\{1,\}\)\{1,2\}\).*/\1/;s/\./, /g"`; \
	    $(SED) -e 's|@bindir[@]|$(bindir)|g' \
		-e "/app\.path/s/$(PACKAGE)/$(installed_name)/" \
		-e "s/app\.version.*/app.version($$VER)/" \
		$< > $@

iperf-5.3.rb: iperf-5.3.rb.in
	VER=`echo $(VERSION) | $(SED) "s/.*+oml//;s/\([0-9]\{1,\}\(\.[0-9]\{1,\}\)\{1,2\}\).*/\1/;s/\./, /g"`; \
	    $(SED) -e 's|@bindir[@]|$(bindir)|g' \
		-e "/app\.path/s/$(PACKAGE)/$(installed_name)/" \
		-e "s/app\.version.*/app.version($$VER)/" \
		$< > $@

iperf_oml.h: iperf.rb
	$(SCAFFOLD) -f --oml $<
