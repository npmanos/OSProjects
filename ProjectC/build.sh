#! /usr/bin/env sh

rm diskc.img
cp ".orig.diskc.img" diskc.img

bcc -0 -ansi -c -o kernel_c.o ./src/c/kernel.c
as86 ./src/asm/kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o

bcc -0 -ansi -c -o shell.o ./src/c/shell.c
as86 ./src/asm/userlib.asm -o userlib.o
ld86 -o shell -d shell.o userlib.o

./loadFile kernel
./loadFile shell
./loadFile message.txt
./loadFile tstpr1
./loadFile tstpr2