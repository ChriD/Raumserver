#!/bin/bash
./getActualLibs.sh
make arch=ARMV7HF dbg=1 clean
make arch=ARMV7HF dbg=1 
/bin/cp -rf libs/linux_ARMV7/raumserver.so bin/linux_ARMV7/libs/linux_ARMV7/raumserver.so
/bin/cp -rf libs/linux_ARMV7/raumkernel.so bin/linux_ARMV7/libs/linux_ARMV7/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_ARMV7/raumsrvDaemon
/bin/cp -rf settings.xml bin/linux_ARMV7/settings.xml
/bin/cp -rf S99raumserver bin/linux_ARMV7/S99raumserver
/bin/cp -rf docroot bin/linux_ARMV7/docroot
make arch=ARMV7HF dbg=1 clean
