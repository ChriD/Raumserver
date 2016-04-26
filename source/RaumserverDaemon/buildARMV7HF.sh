#!/bin/bash
make arch=ARMV7HF clean
make arch=ARMV7HF
/bin/cp -rf libs/linux_ARMV7/raumserver.so bin/linux_ARMV7/libs/linux_ARMV7/raumserver.so
/bin/cp -rf libs/linux_ARMV7/raumkernel.so bin/linux_ARMV7/libs/linux_ARMV7/raumkernel.so
/bin/cp -rf raumsrvDaemon bin/linux_ARMV7/raumsrvDaemon
make arch=ARMV7HF clean
