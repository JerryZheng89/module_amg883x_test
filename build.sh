#!/bin/sh
make clean
make
ls --sort=extension *.c *.o -lhF

find ./ -type f -name "*" ! -name "build.sh" ! -name "Makefile" ! -name "*.json" ! -name "*.o" ! -name "*.c" \
    -exec scp {} jerryzheng@172.16.81.29:/Users/jerryzheng/Public/nfshome/home/root \;