#!/bin/bash
./getActualLibs.sh
make arch=ARMV7HF dbg=1 clean
make arch=ARMV7HF dbg=1 
/bin/cp -rf libs/linux_ARMV7/raumserver.so bin/linux_ARMV7/libs/linux_ARMV7/raumserver.so
/bin/cp -rf libs/linux_ARMV7/raumkernel.so bin/linux_ARMV7/libs/linux_ARMV7/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_ARMV7/raumsrvDaemon
/bin/cp -rf raumserver.xml bin/linux_ARMV7/raumserver.xml
make arch=ARMV7HF dbg=1 clean
