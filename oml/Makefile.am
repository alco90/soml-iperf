pkgdata_DATA = iperf.app
CLEANFILES = $(pkgdata_DATA)
EXTRA_DIST= iperf.app.in

iperf.app: iperf.app.in
	sed -e 's|@bindir[@]|$(bindir)|g' $< > $@
