#!/bin/bash
ARCH=$1
rm -rf bin/linux_$ARCH/
mkdir -p bin
mkdir -p bin/linux_$ARCH
mkdir -p bin/linux_$ARCH/logs

#copy build stuff
rsync -aP build/linux_$ARCH/static/* bin/linux_$ARCH
/bin/cp -rf S99raumserver bin/linux_$ARCH/S99raumserver

#clear old logs
rm -rf bin/linux_$ARCH/logs/*
touch bin/linux_$ARCH/logs/_dummy.txt

#zip it
cd bin
cd linux_$ARCH
zip -r ../raumserverDaemon_linux_$ARCH.zip * 
cd ..
cd ..
