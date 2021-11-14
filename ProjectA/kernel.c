/* 
    COMP 350 OS Kernel
    Author: Nick Manos
    kernel.c
*/

void main()
{
    printMem("Hello World\0", 0x0);

    while(1);
}

void printMem(char *str, int offset)
{
    while (*str != 0x0)
    {
        putInMemory(0xb800, offset, *str);
        offset++;
        putInMemory(0xb800, offset, 0x7);
        offset++;
        str++;
    }
}