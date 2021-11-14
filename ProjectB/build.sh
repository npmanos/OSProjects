#! /usr/bin/env sh

bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o
dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
dd if=message.txt of=diskc.img bs=512 count=1 seek=30 conv=notrunc