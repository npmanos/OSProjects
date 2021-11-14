# Project B - System Calls

## Objective
Use some of the system calls provided by BIOS and then write system calls to print a string to video, read a line from the keyboard, and read a sector from the disk.

## Steps
See the [instructions](docs/OSProjectB_2021.pdf) for details.
 - [x] Step 1: Printing to the screen - Interrupt 0x10
 - [x] Step 2: Reading from the keyboard - Interrupt 0x16
 - [x] Step 3: Read a sector from the disk - Interrupt 0x13
 - [x] Step 4: Create your own interrupt
 - [x] Step 5: Make printString, readString, and readSector interrupt calls

## Build and Verify
1. Build the kernel
   ```
   ./build.sh
   ```
2. Run the simulator
   ```
   java -jar simulator.jar
   ```
3. Load `diskc.img` and confirm the cylinders, heads, and sectors are correct
4. Click "Boot Disk C:"
5. After booting, follow the on screen instructions. When the program is complete, the following output should be printed to the screen:
   ```
   STEP 1
   Hello World

   STEP 2
   Enter a line: [INPUT]
   [INPUT]

   STEP 3
   If this message prints out, then your readSector function is working correctly!

   STEPS 4 & 5
   Enter another line: [MORE INPUT]
   [MORE INPUT]
   If this message prints out, then your readString function is working correctly!
   ERROR! Invalid instruction
   ```