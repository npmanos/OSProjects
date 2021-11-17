
/* 
    COMP 350 OS Kernel
    Project C
    Author: Nick Manos
    kernel.c
*/

void printChar(char);
void printString(char *);
void readString(char *);
void readSector(char *, int);
void readFile(char *, char *, int *);

void main()
{
    char buffer[13312];
    int sectorsRead = 0;
    makeInterrupt21();
    interrupt(0x21, 3, "messag", buffer, &sectorsRead);
    if (sectorsRead > 0) {
        interrupt(0x21, 0, buffer, 0, 0);
    } else {
        interrupt(0x21, 0, "File \"messag\" not found.\r\n");
    }
    

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

void readFile(char* filename, char* buffer, int* sectorsRead) {
    char dir[512];
    int entry;
    int offset;
    int match = 1;
    *sectorsRead = 0;

    readSector(&dir, 2);

    for (entry = 0; entry < 512; entry = entry + 32) {
        for (offset = 0; offset < 6; offset++)
        {
            if (dir[entry + offset] != filename[offset])
            {
                match = 0;
                break;
            }
        }

        if (match) {
            break;
        } else {
            match = 1;
        }
    }

    if (!match) {
        return;
    }

    for (entry, offset; dir[entry + offset] != 0; offset++) {
        interrupt(0x21, 2, buffer, dir[entry + offset], 0);
        *sectorsRead += 1;
        buffer += 512;
    }
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
        case 0x3:
            readFile(bx, cx, dx);
            break;
        default:
            printString("ERROR! Invalid instruction\0");
            break;
    }
}