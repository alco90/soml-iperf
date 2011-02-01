
if LIBOML2
pkgdata_DATA = iperf.app
else !LIBOML2
pkgdata_DATA =
endif !LIBOML2

CLEANFILES = $(pkgdata_DATA)
EXTRA_DIST= iperf.app.in

iperf.app: iperf.app.in
	sed -e 's|@bindir[@]|$(bindir)|g' $< > $@
