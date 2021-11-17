#! /usr/bin/env sh

bcc -0 -ansi -c -o kernel_c.o ./src/c/kernel.c
as86 ./src/asm/kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o
dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
# ./loadFile kernel
# ./loadFile message.txt