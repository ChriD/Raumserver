#!/bin/bash
cd bin/
find . -name '*.zip' -delete

zip -r binaries.zip linux_ARMV5/* linux_ARMV6/*

cd linux_ARMV5
zip -r ../raumserverDaemon_linux_ARMV5.zip * 
cd ..

cd linux_ARMV6
zip -r ../raumserverDaemon_linux_ARMV6.zip *
cd ..

cd linux_ARMV7
zip -r ../raumserverDaemon_linux_ARMV7.zip *
cd ..

cd linux_x86
zip -r ../raumserverDaemon_linux_x86.zip *
cd ..

cd linux_x64
zip -r ../raumserverDaemon_linux_x64.zip *
cd ..
