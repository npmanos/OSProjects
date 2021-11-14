/* 
    COMP 350 OS Kernel
    Project A
    Author: Nick Manos
    kernel.c
*/

void main() {

    int startVidMem = 0xb800;
    int vidMemOffset = 0x0;
    int white = 0x7;
    char *letters = "Hello World\0";

    while(*letters != 0x0) {
        putInMemory(startVidMem, vidMemOffset, *letters);
        vidMemOffset++;
        putInMemory(startVidMem, vidMemOffset, white);
        vidMemOffset++;
        // advance letters pointer
        letters++;
    }

    while(1);
}