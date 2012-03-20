# Override autoconf's default pkgdatadir to honour program-{pre,suf}fix passed
# to configure script
# XXX: Not POSIX compliant
prefixedname = $(shell echo $(PACKAGE) | sed '$(transform)')
pkgdatadir = $(datadir)/$(prefixedname)

if LIBOML2
pkgdata_DATA = iperf.rb
else !LIBOML2
pkgdata_DATA =
endif !LIBOML2

CLEANFILES = $(pkgdata_DATA)
EXTRA_DIST= iperf.rb.in

iperf.rb: iperf.rb.in
	sed -e 's|@bindir[@]|$(bindir)|g' -e '/a\.path/s/$(PACKAGE)/$(prefixedname)/' $< > $@
