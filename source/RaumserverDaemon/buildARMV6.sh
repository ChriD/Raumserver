#!/bin/bash
./getActualLibs.sh
make arch=ARMV6 clean
make arch=ARMV6
/bin/cp -rf libs/linux_ARMV6/raumserver.so bin/linux_ARMV6/libs/linux_ARMV6/raumserver.so
/bin/cp -rf libs/linux_ARMV6/raumkernel.so bin/linux_ARMV6/libs/linux_ARMV6/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_ARMV6/raumsrvDaemon
/bin/cp -rf settings.xml bin/linux_ARMV6/settings.xml
/bin/cp -rf S99raumserver bin/linux_ARMV6/S99raumserver
/bin/cp -rf docroot bin/linux_ARMV6/docroot
make arch=ARMV6 clean
