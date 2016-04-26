#!/bin/bash
make arch=X86 clean
make arch=X86
/bin/cp -rf libs/linux_x86/raumserver.so bin/linux_x86/libs/linux_x86/raumserver.so
/bin/cp -rf libs/linux_x86/raumkernel.so bin/linux_x86/libs/linux_x86/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_x86/raumsrvDaemon
make arch=X86 clean
