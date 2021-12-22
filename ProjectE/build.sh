#! /usr/bin/env sh

rm diskc.img
cp ".orig.diskc.img" diskc.img

bcc -0 -ansi -c -o kernel_c.o -C-d -A-l -A-m -Akernel.l -A-s -Akernel_c.syms ./src/c/kernel.c
as86 -0 ./src/asm/kernel.asm -o kernel_asm.o
ld86 -0 -M -o kernel -d kernel_c.o kernel_asm.o > kernel.syms

bcc -0 -ansi -c -o shell.o -C-d -A-l -A-m -Ashell.l -A-s -Ashell_c.syms ./src/c/shell.c
as86 -0 ./src/asm/userlib.asm -o userlib.o
ld86 -0 -M -o shell shell.o userlib.o > shell.syms

./loadFile kernel
./loadFile shell
./loadFile message.txt
./loadFile tstpr1
./loadFile tstpr2