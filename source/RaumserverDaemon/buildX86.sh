#!/bin/bash
./getActualLibs.sh
make arch=X86 clean
make arch=X86
/bin/cp -rf libs/linux_x86/raumserver.so bin/linux_x86/libs/linux_x86/raumserver.so
/bin/cp -rf libs/linux_x86/raumkernel.so bin/linux_x86/libs/linux_x86/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_x86/raumsrvDaemon
/bin/cp -rf raumserver bin/linux_x86/raumserver
/bin/cp -rf settings.xml bin/linux_x86/settings.xml
/bin/cp -rf S99raumserver bin/linux_x86/S99raumserver
/bin/cp -rf docroot bin/linux_x86/docroot
make arch=X86 clean
