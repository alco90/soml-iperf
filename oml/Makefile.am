# Override autoconf's default pkgdatadir to honour program-{pre,suf}fix passed
# to configure script
installed_name = `echo -n "$(PACKAGE)" | sed '$(transform)'`
pkgdatadir = $(datadir)/$(installed_name)

SCAFFOLD = oml2-scaffold

BUILT_SOURCES = iperf.rb \
		iperf-5.3.rb

if LIBOML2
pkgdata_DATA = iperf-5.3.rb iperf.rb

BUILT_SOURCES += iperf_oml.h

install-data-hook:
	cd $(DESTDIR)$(pkgdatadir) && \
		$(LN_S) iperf.rb iperf-5.4.rb
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
	VER=`echo $(VERSION) | sed "s/.*+//;s/-[0-9a-z]\+\(-dirty\)$$//;s/-.*//;s/\(pre\|rc\)\./-/;s/[^-0-9.]//g;s/\./, /g"`; \
		sed -e 's|@bindir[@]|$(bindir)|g' \
		-e "/a\.path/s/$(PACKAGE)/$(installed_name)/" \
		-e "s/a\.version.*/a.version($$VER)/" \
		$< > $@

iperf-5.3.rb: iperf-5.3.rb.in
	VER=`echo $(VERSION) | sed "s/-[0-9a-z]\+\(-dirty\)$$//;s/-.*//;s/\(pre\|rc\)\./-/;s/[^-0-9.]//g;s/\./, /g"`; \
		sed -e 's|@bindir[@]|$(bindir)|g' \
		-e "/a\.path/s/$(PACKAGE)/$(installed_name)/" \
		-e "s/a\.version.*/a.version($$VER)/" \
		$< > $@

iperf_oml.h: iperf.rb
	$(SCAFFOLD) -f --oml $<
