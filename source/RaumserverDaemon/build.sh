#!/bin/bash
ARCH=$1
./getLinkedLibs.sh $ARCH
rm -rf build/linux_$ARCH/
mkdir -p build
mkdir -p build/linux_$ARCH
mkdir -p build/linux_$ARCH/dynamic
mkdir -p build/linux_$ARCH/dynamic/libs
mkdir -p build/linux_$ARCH/dynamic/logs
mkdir -p build/linux_$ARCH/static/docroot
mkdir -p build/linux_$ARCH/static
mkdir -p build/linux_$ARCH/static/libs
mkdir -p build/linux_$ARCH/static/logs
mkdir -p build/linux_$ARCH/static/docroot
# copy binaries
#/bin/cp -rf libs/linux_$ARCH/libraumkernel.so build/linux_$ARCH/dynamic/libs/libraumkernel.so
#/bin/cp -rf libs/linux_$ARCH/libraumserver.so build/linux_$ARCH/dynamic/libs/libraumserver.so
rsync -aP libs/linux_$ARCH/*libunwind*.so build/linux_$ARCH/dynamic/libs/
rsync -aP libs/linux_$ARCH/*libunwind*.so build/linux_$ARCH/static/libs/
rsync -aP docroot/* build/linux_$ARCH/dynamic/docroot/
rsync -aP docroot/* build/linux_$ARCH/static/docroot/
/bin/cp -rf settings.xml build/linux_$ARCH/dynamic/settings.xml
/bin/cp -rf settings.xml build/linux_$ARCH/static/settings.xml
make arch=$ARCH dbg=1 clean
make arch=$ARCH dbg=1
/bin/cp -rf build/raumsrvDaemonDyn build/linux_$ARCH/dynamic/raumsrvDaemon
/bin/cp -rf build/raumsrvDaemonStat build/linux_$ARCH/static/raumsrvDaemon
make arch=$ARCH dbg=1 clean

