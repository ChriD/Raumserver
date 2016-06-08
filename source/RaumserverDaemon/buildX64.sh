#!/bin/bash
./getActualLibs.sh
make arch=X64 clean
make arch=X64
/bin/cp -rf libs/linux_x64/raumserver.so bin/linux_x64/libs/linux_x64/raumserver.so
/bin/cp -rf libs/linux_x64/raumkernel.so bin/linux_x64/libs/linux_x64/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_x64/raumsrvDaemon
/bin/cp -rf raumserver.xml bin/linux_x64/raumserver.xml
/bin/cp -rf S99raumserver bin/linux_x64/S99raumserver
make arch=X64 clean
