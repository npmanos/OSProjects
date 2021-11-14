# Project A - Booting

## Objective
Write a very small kernel that prints "Hello World" to the screen and hangs up.

## Steps
See the [instructions](docs/OSProjectA_2021.doc) for full details.

 - [x]  Bootloader
   - [x]  Assemble [bootload.asm](bootload.asm)
   - [x]  Create [diskc.img](diskc.img)
   - [x]  Copy [bootload](bootload) to beginning of diskc.img
 - [x]  A Hello World Kernel
   - [x]  Create [kernel.c](kernel.c)
   - [x]  Display "Hello World" on screen by writing to video memory
 - [x]  Compiling kernel.c
   - [x]  Compile [kernel.c](kernel.c) using bcc
   - [x]  Assemble [kernel.asm](kernel.asm)
   - [x]  Link kernel_c.o and kernel_asm.o
   - [x]  Copy kernel to sector 3 of diskc.img
 - [x]  Scripts
 - [ ]  Running on a Real Computer (optional)

## Building
### Dependencies
 - Linux
 - Java runtime >= 1.8
 - [Emumaker86](https://github.com/mdblack/simulator/raw/master/simulator.jar) - An x86 process simulator
 - bcc (Bruce Evan's C Compiler) - A 16-bit C compiler
 - as86, ld86 - A 16 bit assembler and linker
 - nasm - The Netwide Assembler
 - dd - A standard low-level copying utility (generally included with *nix OS's)

### Build and Test
1. Assemble the bootloader
   ```console
   $ nasm bootload.asm
   ```
2. Copy the bootloader to the beginning of diskc.img
   ```console
   $ dd if=bootload of=diskc.img bs=512 count=1 conv=notrunc
   ```
3. Run the build script
   ```console
   $ ./build.sh
   ```
4. Run the simulator
   ```console
   $ java -jar simulator.jar
   ```
5. Click the "Choose" button next to Hard Disk C and select `diskc.img`
6. Set Cylinders to 63, Heads to 255, and Sectors to 1000
7. Click "Boot Disk C:"