#!/bin/bash
ARCH=$1
mkdir -p bin
mkdir -p bin/linux_$ARCH

#copy build stuff
rsync -aP build/linux_$ARCH/static/* bin/linux_$ARCH

#zip it
cd bin
cd linux_$ARCH
zip -r ../raumserverDaemon_linux_$ARCH.zip * 
cd ..
cd ..
