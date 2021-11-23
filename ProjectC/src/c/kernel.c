
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
void executeProgram(char *);
void terminate();

char shell[6];

void main()
{
    // char buffer[13312];
    // int sectorsRead = 0;
    // char shell[6];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';

    makeInterrupt21();
    interrupt(0x10, 0x3, 0, 0, 0); /* clear screen by setting video mode */

    // interrupt(0x21, 3, "messag", buffer, &sectorsRead);
    // if (sectorsRead > 0) {
    //     interrupt(0x21, 0, buffer, 0, 0);
    // } else {
    //     interrupt(0x21, 0, "File \"messag\" not found.\r\n");
    // }

    interrupt(0x21, 4, shell, 0, 0);

    // while (1);
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
    while (i < 128)
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
                i++;
                return;
            case 0x8:
                if (i > 0) 
                {
                    printChar(0x8);
                    printChar(' ');
                    printChar(0x8);
                    str--;
                    i--;
                }
                break;
            default:
                *str = c;
                printChar(c);
                str++;
                i++;
                break;
        }
    }
}

void readSector(char* buffer, int sector) {
    interrupt(0x13, 2 * 256 + 1, buffer, sector + 1, 0x80);
}

void readFile(char* filename, char* buffer, int* sectorsRead) {
    char dir[512];
    int entry;
    int offset;
    int match;
    *sectorsRead = 0;

    readSector(&dir, 2);

    for (entry = 0; entry < 512; entry = entry + 32) {
        match = 0;
        for (offset = 0; offset < 6; offset++)
        {
            if (dir[entry + offset] != filename[offset])
            {
                match = 0;
                break;
            }

            match = 1;
        }

        if (match == 1) {
            break;
        }
    }

    if (match == 0) {
        return;
    }

    for (entry, offset; dir[entry + offset] != 0; offset++)
    {
        interrupt(0x21, 2, buffer, dir[entry + offset], 0);
        *sectorsRead += 1;
        buffer += 512;
    }
}

void executeProgram(char* progName) {
    char buf[13312];
    int sectorsRead = 0;
    int i;

    interrupt(0x21, 3, progName, buf, &sectorsRead);

    if (sectorsRead > 0) {
        for (i = 0; i < 512 * sectorsRead; i++) {
            putInMemory(0x2000, i, buf[i]);
        }

        launchProgram(0x2000);
    }
}

void terminate() {
    // char shell[6];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';
    interrupt(0x21, 4, shell, 0, 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
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
        case 0x4:
            executeProgram(bx);
            break;
        case 0x5:
            terminate();
            break;
        default:
            printString("ERROR! Invalid instruction\0");
            break;
    }
}