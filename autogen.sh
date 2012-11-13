#!/bin/sh

# autogen.sh: Automatically generate all build files from configure.ac

# Import Gnulib modules
gnulib-tool --update --quiet --quiet
chmod a+x build-aux/git-version-gen

# aclocal doesn't like it if you -I the system aclocal directory
SYS_ACLOCAL_DIR=`aclocal --print-ac-dir`
for i in m4 /usr/local/share/aclocal; do
    if [ ! $i = $SYS_ACLOCAL_DIR -a -d $i ]; then
        ACLOCAL_OPTS="$ACLOCAL_OPTS -I $i"
    fi
done
aclocal $ACLOCAL_OPTS || exit 1

autoheader || exit 1
autoconf || exit 1

automake --add-missing --include-deps --foreign || exit 1
