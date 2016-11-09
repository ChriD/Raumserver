#!/bin/bash

mkdir -p bin
/bin/cp -rf version.xml bin/version.xml

cd bin/
find . -name '*.zip' -delete
cd ..

./zip.sh X64
./zip.sh ARMV7HF
./zip.sh ARMV5TE

cd bin/
zip -r binaries.zip linux_ARMV5TE/* linux_ARMV7HF/* version.xml


