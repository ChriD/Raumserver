#!/bin/bash
./getActualLibs.sh
make arch=ARMV5TE dbg=1 clean
make arch=ARMV5TE dbg=1
/bin/cp -rf libs/linux_ARMV5/raumserver.so bin/linux_ARMV5/libs/linux_ARMV5/raumserver.so
/bin/cp -rf libs/linux_ARMV5/raumkernel.so bin/linux_ARMV5/libs/linux_ARMV5/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_ARMV5/raumsrvDaemon
/bin/cp -rf settings.xml bin/linux_ARMV5/settings.xml
/bin/cp -rf S99raumserver bin/linux_ARMV5/S99raumserver
/bin/cp -rf docroot bin/linux_ARMV5/docroot
make arch=ARMV5TE dbg=1 clean
