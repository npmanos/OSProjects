# Project D - Writing and Deleting Files

## Objective
Implement functions for deleting and writing files to produce a fully functional single-process operating system about as powerful as CP/M.

## Steps
See the [instructions](docs/OSProjectD_2021.pdf) for details.
 - [x] Step 1: Write Sector
 - [x] Step 2: List the directory contents
 - [x] Step 3: Delete File
 - [x] Step 4: Writing a file
 - [x] Step 5: Copying a file
 - [x] Step 6: Creating a text file

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
5. After booting, the following should be printed to the screen:
   ```
   COMP 350 OS vD

   /$ 
   ```
6. At the shell prompt, type `dir`. The shell should print:
   ```
   kernel (5 sectors)
   shell (5 sectors)
   messag (1 sector)
   tstpr1 (1 sector)
   tstpr2 (1 sector)
   ```
7. At the shell prompt, type `del messag`. Then type `type messag`. The shell should print:
   ```
   Cannot type 'messag': No such file
   ```
8. At the shell prompt, type `copy shell sh`. Then type `exec sh`. You should get a functioning shell prompt.
9. At the shell prompt, type `create textfl`. Enter one or more lines of text, ending with a blank line. Then type `type textfl` and the shell should print the text you entered.