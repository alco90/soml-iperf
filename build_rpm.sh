#!/bin/bash

# make sure you install the build dependencies from the .spec file
# before using this script

version="2.0.5+oml1"
tmp="/tmp/iperf-rpmbuild"
package="oml2-iperf"

./autogen.sh
./configure
make PACKAGE=$package dist

if [ -d $tmp ]; then rm -rf $tmp; fi

mkdir -p $tmp
cd $tmp
mkdir -p BUILD RPMS SOURCES SPECS SRPMS
cd -
ln -s `pwd`/$package-$version.tar.gz $tmp/SOURCES/$package-2.0.5_oml1.tar.gz
rpmbuild -v -bb --clean $package.spec --define "_topdir $tmp" --nodeps
find $tmp -type f -name "*.rpm" -exec cp -v {} . \;
rm -rf $tmp

