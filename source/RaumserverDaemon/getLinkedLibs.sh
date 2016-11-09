#!/bin/bash
ARCH=$1
cp ../../../RaumkernelLib/source/build/linux_$ARCH/libs/libraumkernel.so libs/linux_$ARCH/libraumkernel.so
cp ../../../RaumkernelLib/source/build/linux_$ARCH/libs/libraumkernel.a libs/linux_$ARCH/libraumkernel.a
cp ../../../RaumkernelLib/source/libs/linux_$ARCH/libohNetCore.a libs/linux_$ARCH/libohNetCore.a
cp ../../../RaumkernelLib/source/libs/linux_$ARCH/libohNetDevices.a libs/linux_$ARCH/libohNetDevices.a
cp ../../../RaumkernelLib/source/libs/linux_$ARCH/libohNetProxies.a libs/linux_$ARCH/libohNetProxies.a
cp ../../../RaumserverLib/source/build/linux_$ARCH/libs/libraumserver.so libs/linux_$ARCH/libraumserver.so
cp ../../../RaumserverLib/source/build/linux_$ARCH/libs/libraumserver.a libs/linux_$ARCH/libraumserver.a
