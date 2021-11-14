/* 
    COMP 350 OS Kernel
    Project B
    Author: Nick Manos
    kernel.c
*/

void printChar(char);
void printString(char *);
void readString(char *);
void readSector(char *, int);

void main()
{
    char lineIn[82];
    char nextLineIn[82];
    char buffer[512];
    char nextBuffer[512];
    char* CRLF = "\xd\xa\0";
    interrupt(0x10, 0x3, 0, 0, 0); /* clear screen by setting video mode */

    /* Step 1: Printing to the screen */
    printString("STEP 1\0");
    printString(CRLF);
    printString("Hello World\0");

    printString(CRLF);
    printString(CRLF);

    /* Step 2: Reading from the keyboard */
    printString("STEP 2\0");
    printString(CRLF);
    printString("Enter a line: \0");
    readString(&lineIn);
    printString(&lineIn);

    printString(CRLF);

    /* Step 3: Read a sector from the disk */
    printString("STEP 3\0");
    printString(CRLF);
    readSector(&buffer, 30);
    printString(&buffer);

    printString(CRLF);
    printString(CRLF);

    /* Steps 4 and 5: Make your own interrupts/
       printString, readString, and readSector interrupt calls */
    makeInterrupt21();
    interrupt(0x21, 0, "STEPS 4 & 5\xd\xa\0", 0, 0);
    /*interrupt(0x21, 0, 0, 0, 0);*/ /* Step 4 */
    interrupt(0x21, 0, "Enter another line: \0", 0, 0); /* readString and printString */
    interrupt(0x21, 1, &nextLineIn, 0, 0);
    interrupt(0x21, 0, &nextLineIn, 0, 0);

    interrupt(0x21, 2, &nextBuffer, 30, 0); /* readSector */
    interrupt(0x21, 0, &nextBuffer, 0, 0);

    interrupt(0x21, 0, CRLF, 0, 0);

    interrupt(0x21, 3, 0, 0, 0); /* error */

    while (1);
}

void printChar(char c) {
    interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
}

void printString(char *str) {
    while (*str != '\0')
    {
        printChar(*str);
        str++;
    }
}

void readString(char *str) {
    int i = 0;
    while (i < 80)
    {
        int c = interrupt(0x16, 0, 0, 0, 0);
        switch (c) {
            case 0xd:
                *str = c;
                str++;
                *str = 0xa;
                str++;
                *str = '\0';
                printChar(c);
                printChar(0xa);
                return;
            case 0x8:
                printChar(0x8);
                printChar(' ');
                printChar(0x8);
                str--;
                i--;
                break;
            default:
                *str = c;
                printChar(c);
                str++;
                break;
        }
        i++;
    }
}

void readSector(char* buffer, int sector) {
    interrupt(0x13, 2 * 256 + 1, buffer, sector + 1, 0x80);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    /*printString("Hello interrupt 21!\0");*/ /* Step 4 */

    switch (ax) {
        case 0x0:
            printString(bx);
            break;
        case 0x1:
            readString(bx);
            break;
        case 0x2:
            readSector(bx, cx);
            break;
        default:
            printString("ERROR! Invalid instruction\0");
            break;
    }
}