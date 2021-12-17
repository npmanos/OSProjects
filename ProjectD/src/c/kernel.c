/* 
    COMP 350 OS Kernel
    Project D
    Author: Nick Manos
    kernel.c
*/

void printChar(char);
void printString(char *);
void readString(char *);
void readSector(char *, int);
void writeSector(char *, int);
void readFile(char *, char *, int *);
void writeFile(char *, char *, int);
void deleteFile(char*);
void executeProgram(char *);
void terminate();

char shell[6];

void main()
{
    // char buffer[13312];
    // int sectorsRead = 0;
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';

    makeInterrupt21();
    interrupt(0x10, 0x3, 0, 0, 0); /* clear screen by setting video mode */

    // STEP 1
    // interrupt(0x21, 3, "messag", buffer, &sectorsRead);
    // if (sectorsRead > 0) {
    //     interrupt(0x21, 0, buffer, 0, 0);
    // } else {
    //     interrupt(0x21, 0, "File \"messag\" not found.\r\n");
    // }

    //STEP 4
    // interrupt(0x21, 8, "this is a test message", "tstmg", 3);

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
    while (i < 4096)
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
    interrupt(0x13, (2 << 8) + 1, buffer, sector + 1, 0x80);
}

void writeSector(char* buffer, int sector) {
    interrupt(0x13, (3 << 8) + 1, buffer, sector + 1, 0x80);
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
        for (offset = 0; offset < 6 && dir[entry + offset] != '\0'; offset++)
        {
            if (dir[entry + offset] != filename[offset])
            {
                match = 0;
                break;
            }

            match = 1;
        }

        if (match) {
            break;
        }
    }

    if (!match) {
        return;
    }

    for (offset = 6; dir[entry + offset] != 0; offset++)
    {
        interrupt(0x21, 2, buffer, dir[entry + offset], 0);
        *sectorsRead += 1;
        buffer += 512;
    }
}

void writeFile(char* buffer, char* filename, int size) {
    char map[512];
    char dir[512];
    int byteSize = 512 * size;
    char zBuf[512];
    int sectors[255];
    int entry;
    int offset;
    int sector;
    int toWrite;
    int i;
    int j;
    int bufSize = sizeof(buffer);

    readSector(&map, 1);
    readSector(&dir, 2);

    for (entry = 0; entry < 512 && dir[entry] != '\0'; entry += 32);

    if (entry == 512) {
        return;
    }

    for (sector = 3, j = 0; sector < 512 && j < size; sector++) {
        if (map[sector] == '\0') {
            sectors[j++] = sector;
        }
    }

    if (j < size) {
        return; // not enough free sectors
    }

    for (offset = 0; *filename != '\0'; offset++, filename++)
    {
        dir[entry + offset] = *filename;
    }

    for (; offset < 6; offset++) {
        dir[entry + offset] = '\0';
    }

    for (i = 0; i < size && offset < 32; i++, offset++, buffer += 512) {
        writeSector(zBuf, sectors[i]);
        writeSector(buffer, sectors[i]);
        map[sectors[i]] = 0xFF;
        dir[entry + offset] = sectors[i];
    }
    
    writeSector(&map, 1);
    writeSector(&dir, 2);
}

void deleteFile(char* filename) {
    char map[512];
    char dir[512];
    int entry;
    int offset;
    int match;

    readSector(map, 1);
    readSector(dir, 2);

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

        if (match) {
            break;
        }
    }

    if (!match) {
        return;
    }

    dir[entry] = '\0';

    for (entry, offset; dir[entry + offset] != 0; offset++)
    {
        map[dir[entry + offset]] = '\0';
    }

    writeSector(map, 1);
    writeSector(dir, 2);
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
        case 0x6:
            writeSector(bx, cx);
            break;
        case 0x7:
            deleteFile(bx);
            break;
        case 0x8:
            writeFile(bx, cx, dx);
            break;
        default:
            printString("ERROR! Invalid instruction\0");
            break;
    }
}