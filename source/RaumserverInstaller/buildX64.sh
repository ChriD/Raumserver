#!/bin/bash
./getActualLibs.sh
make arch=X64 clean
make arch=X64
/bin/cp -rf libs/linux_x64/raumserverInstaller.so bin/linux_x64/libs/linux_x64/raumserverInstaller.so
/bin/cp -rf libs/linux_x64/libssh.so bin/linux_x64/libs/linux_x64/libssh.so
/bin/cp -rf libs/linux_x64/libssh.so.4 bin/linux_x64/libs/linux_x64/libssh.so
/bin/cp -rf libs/linux_x64/libssh.so.4.4.0 bin/linux_x64/libs/linux_x64/libssh.so.4.4.0
/bin/cp -rf libs/linux_x64/libssh_threads.so bin/linux_x64/libs/linux_x64/libssh_threads.so
/bin/cp -rf libs/linux_x64/libssh_threads.so.4 bin/linux_x64/libs/linux_x64/libssh_threads.so.4
/bin/cp -rf libs/linux_x64/libssh_threads.so.4.4.0 bin/linux_x64/libs/linux_x64/libssh_threads.so.4.4.0
#/bin/cp -rf libs/linux_x64/libsciter-gtk-64.so bin/linux_x64/libs/linux_x64/libsciter-gtk-64.so
/bin/cp -rf libs/linux_x64/libsciter-gtk-64.so bin/linux_x64/libsciter-gtk-64.so
/bin/cp -rf raumsrvInstaller bin/linux_x64/raumsrvInstaller
/bin/cp -rf settings.xml bin/linux_x64/settings.xml
/bin/cp -rf installSettings.xml bin/linux_x64/installSettings.xml
make arch=X64 clean
