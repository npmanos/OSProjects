# Project C - Loading Files and Executing Programs

## Objective
Write routines to read files into memory and execute programs and write a basic shell program.

## Steps
See the [instructions](docs/OSProjectB_2021.pdf) for details.
 - [x] Step 1: Read a file and print it
 - [x] Step 2: Load a program and execute it
 - [x] Step 3: Terminate program system call
 - [x] Step 4: The Shell - making your own user program
 - [x] Step 5: Shell Command "type"
 - [x] Step 6: Shell Command "exec"

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
   COMP 350 OS vC

   /$ 
   ```
6. At the shell prompt, type `ls`. The shell should print:
   ```
   ERROR! Command not found: ls
   ```
7. At the shell prompt, type `type messag`. The shell should print:
   ```
   If this message prints out, then your readFile function is working correctly!
   ```
8. At the shell prompt, type `type hellow`. The shell should print:
   ```
   Cannot type 'hellow': No such file
   ``` 
9.  At the shell prompt, type `exec tstpr2`. The shell should print:
   ```
   tstpr2 is working!
   COMP 350 OS vC

   /$
   ```
10. At the shell prompt, type `exec msword`. The shell should print:
    ```
    ERROR! Program not found: msword
    ```