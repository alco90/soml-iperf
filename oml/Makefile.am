# Override autoconf's default pkgdatadir to honour program-{pre,suf}fix passed
# to configure script
pkgdatadir = $(datadir)/$(installed_name)

if LIBOML2
pkgdata_DATA = iperf-5.3.rb iperf-5.4.rb
else !LIBOML2
pkgdata_DATA =
endif !LIBOML2

CLEANFILES = $(pkgdata_DATA)
EXTRA_DIST= iperf-5.3.rb.in iperf-5.4.rb.in

iperf-5.3.rb: iperf-5.3.rb.in
	sed -e 's|@bindir[@]|$(bindir)|g' -e '/a\.path/s/$(PACKAGE)/$(installed_name)/' $< > $@

iperf-5.4.rb: iperf-5.4.rb.in
	sed -e 's|@bindir[@]|$(bindir)|g' -e '/a\.path/s/$(PACKAGE)/$(installed_name)/' $< > $@
