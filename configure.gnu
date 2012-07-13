#!/bin/sh
# This script is a wrapper for the configure script, to be called during a full
# build of the oml-apps. It ensures the binary gets the proper -oml2 suffix.
OMLSUFFIX=-oml2
for ARG in "$@"; do
	if test "${issuffix}" = "yes"; then
		OMLSUFFIX="${OMLSUFFIX}${ARG}"
		break
	elif test "${ARG}" = "--program-suffix"; then 
		issuffix=yes
	fi
done
# In case of redundant '--program-suffix'es, the latter has precedence
`dirname $0`/configure "$@" --program-suffix "${OMLSUFFIX}"
