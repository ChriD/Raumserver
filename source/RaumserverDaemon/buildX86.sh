#!/bin/bash
make arch=X86 clean
make arch=X86
/bin/cp -rf libs/linux_x86/raumserver.so bin/linux_x86/libs/linux_x86/raumserver.so
/bin/cp -rf libs/linux_x86/raumkernel.so bin/linux_x86/libs/linux_x86/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_x86/raumsrvDaemon
/bin/cp -rf raumserver bin/linux_x86/raumserver
/bin/cp -rf raumserver.xml bin/linux_x86/raumserver.xml
make arch=X86 clean
