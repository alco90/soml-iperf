# Override autoconf's default pkgdatadir to honour program-{pre,suf}fix passed
# to configure script
installed_name = `echo -n "$(PACKAGE)" | $(SED) '$(transform)'`
pkgdatadir = $(datadir)/$(installed_name)

# SED is otherwise provided by libtool.m4
SED=sed

SCAFFOLD = oml2-scaffold

BUILT_SOURCES = iperf.rb

if LIBOML2
pkgdata_DATA = iperf.rb

BUILT_SOURCES += iperf_oml.h

uninstall-hook:
	rm -f $(DESTDIR)$(pkgdatadir)/iperf.rb

else !LIBOML2
pkgdata_DATA =
endif !LIBOML2

EXTRA_DIST= iperf.rb.in

CLEANFILES = iperf_oml.h \
	     iperf.rb

iperf.rb: iperf.rb.in
	VER=`echo $(VERSION) | $(SED) "s/.*+oml//;s/\([0-9]\{1,\}\(\.[0-9]\{1,\}\)\{1,2\}\).*/\1/;s/\./, /g"`; \
	    $(SED) -e 's|@bindir[@]|$(bindir)|g' \
		-e "/app\.path/s/$(PACKAGE)/$(installed_name)/" \
		-e "s/app\.version.*/app.version($$VER)/" \
		$< > $@

iperf_oml.h: iperf.rb
	$(SCAFFOLD) -f --oml $< --ontology ./etsi-ontology/
