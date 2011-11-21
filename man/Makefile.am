man_MANS = iperf.1
EXTRA_DIST = oml_section.man oml_authors.man oml_references.man iperf.1.in

iperf.1: iperf.1.in oml_section.man oml_authors.man oml_references.man
if LIBOML2
	sed -e "/@OML_SECTION@/r $(top_srcdir)/man/oml_section.man"		\
		-e "/@OML_AUTHORS@/r $(top_srcdir)/man/oml_authors.man"		\
		-e "/@OML_REFERENCES@/r $(top_srcdir)/man/oml_references.man"	\
		-e "/^@OML_[^@]\+@$$/d"						\
		$< > $@ 
else
	sed "/@OML_[^@]\+@$$/d" $< > $@
endif
