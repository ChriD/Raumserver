#!/bin/bash
./getActualLibs.sh
make arch=ARMV5TE dbg=1 clean
make arch=ARMV5TE dbg=1
/bin/cp -rf libs/linux_ARMV5/raumserver.so bin/linux_ARMV5/libs/linux_ARMV5/raumserver.so
/bin/cp -rf libs/linux_ARMV5/raumkernel.so bin/linux_ARMV5/libs/linux_ARMV5/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_ARMV5/raumsrvDaemon
/bin/cp -rf raumserver.xml bin/linux_ARMV5/raumserver.xml
make arch=ARMV5TE dbg=1 clean
