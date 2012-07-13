# Override autoconf's default pkgdatadir to honour program-{pre,suf}fix passed
# to configure script
installed_name = `echo -n "$(PACKAGE)" | sed '$(transform)'`
pkgdatadir = $(datadir)/$(installed_name)

if LIBOML2
pkgdata_DATA = iperf-5.3.rb iperf.rb

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

CLEANFILES = $(pkgdata_DATA)
EXTRA_DIST= iperf-5.3.rb.in iperf.rb.in

iperf-5.3.rb: iperf-5.3.rb.in
	sed -e 's|@bindir[@]|$(bindir)|g' -e "/a\.path/s/$(PACKAGE)/$(installed_name)/" $< > $@

iperf.rb: iperf.rb.in
	sed -e 's|@bindir[@]|$(bindir)|g' -e "/a\.path/s/$(PACKAGE)/$(installed_name)/" $< > $@

