/* 
    COMP 350 OS Shell
    Project D
    Author: Nick Manos
    shell.c
*/

#define COM_NUM 7
#define CRLF "\r\n"
#define TERM '\0'

void print(char *);
void printLn(char *);
void readLn(char *);
int mod(int, int);

void getArg(char *, char *);
void file(char *);
void type(char *);
void exec(char *);
void dir();
void del(char *);
void copy(char *);
void create(char *);

void main() {
    char *arg;
    char *commands[COM_NUM];
    char *typeStr = "type";
    char *execStr = "exec";
    char *dirStr = "dir";
    char *delStr = "del";
    char *copyStr = "copy";
    char *createStr = "create";
    char *inCom;
    int com;
    int match;
    char *prMatch;
    int c;
    commands[0] = CRLF;
    commands[1] = typeStr;
    commands[2] = execStr;
    commands[3] = dirStr;
    commands[4] = delStr;
    commands[5] = copyStr;
    commands[6] = createStr;

    printLn("COMP 350 OS vD.6\r\n");

    while (1) {
        char input[4096];
        print("/$ ");
        readLn(input);

        for (com = 0; com < COM_NUM; com++)
        {
            match = 0; 
            for (inCom = input, c = 0; *inCom != ' ' && commands[com][c] != '\0'; inCom++, c++)
            {
                if (*inCom != commands[com][c])
                {
                    match = 0;
                    break;
                }
                else
                {
                    match = 1;
                }
            }

            if (match)
            {
                break;
            }
        }

        switch (com) {
            case 0:
                break;
            case 1:
                getArg(inCom, arg);
                type(++arg);
                print(CRLF);
                break;
            case 2:
                getArg(inCom, arg);
                exec(++arg);
                print(CRLF);
                break;
            case 3:
                dir();
                break;
            case 4:
                getArg(inCom, arg);
                del(++arg);
                break;
            case 5:
                getArg(inCom, arg);
                copy(++arg);
                break;
            case 6:
                getArg(inCom, arg);
                create(++arg);
            default:
                print("ERROR! Command not found: ");
                print(input);
                break;
            }

        print(CRLF);
    }
}

void print(char* str) {
    syscall(0, str, 0, 0);
}

void printLn(char* str) {
    syscall(0, str, 0, 0);
    syscall(0, "\r\n", 0, 0);
}

void getArg(char* input, char* arg) {

    for (; *input != '\r' && *input != '\0'; arg++, input++)
    {
        *arg = *input;
    }

    *arg = '\0';
}

void readLn(char* buf) {
    syscall(1, buf, 0, 0);
}

int mod(int a, int b) {
    return a - (a / b * b);
}

void type(char* file) {
    char buf[13312];
    int sectors;

    syscall(3, file, buf, &sectors);

    if (sectors > 0) {
        print(buf);
    } else {
        print("Cannot type '");
        print(file);
        print("': No such file");
    }
}

void exec(char* program) {
    char buf[13312];
    int sectors;

    syscall(3, program, buf, &sectors);

    if (sectors > 0) {
        syscall(4, program, 0, 0);
    } else {
        print("ERROR! Program not found: ");
        printLn(program);
    }
}

void dir() {
    char dir[512];
    char filename[7];
    char sizeStr[3];
    int entry;
    int offset;
    int s;

    syscall(2, dir, 2, 0);
    for (entry = 0; entry < 512; entry += 32) {
        if (dir[entry] == '\0') {
            continue;
        }

        for (offset = 0; offset < 6 && dir[entry + offset] != '\0'; offset++) {
            filename[offset] = dir[entry + offset];
        }

        filename[offset] = '\0';

        for (offset = 6, s = 0; offset < 26 && dir[entry + offset] != '\0'; offset++, s++);

        print(filename);
        print(" (");
        if (s >= 10) {
            sizeStr[0] = (s / 10) + '0';
        }
        sizeStr[0 + (s >= 10)] = mod(s, 10) + '0';
        sizeStr[1 + (s >= 10)] = '\0';
        print(sizeStr);
        if (s > 1) {
            printLn(" sectors)");
        } else {
            printLn(" sector)");
        }
        
    }

}

void del(char *filename) {
    syscall(7, filename, 0, 0);
}

void copy(char *args) {
    char from[7];
    char to[7];
    int n;
    char buf[13312];
    int sectors = 0;

    for (n = 0, args; *args != ' '; n++, args++)
    {
        from[n] = *args;
    }

    for (; n < 7; n++)
    {
        from[n] = '\0';
    }

    for (n = 0, args++; *args != '\0'; n++, args++)
    {
        to[n] = *args;
    }

    for (; n < 7; n++)
    {
        to[n] = '\0';
    }

    syscall(3, from, buf, &sectors);

    if (sectors > 0) {
        syscall(8, buf, to, sectors);
    } else {
        print("Cannot copy '");
        print(from);
        print("': No such file");
    }
}

void create(char *filename) {
    char file[13312];
    char input[13312];
    char *filePtr = file;
    int i, bSize, sectors = 0;

    while (bSize < 13311) {
        print("... ");
        readLn(input);

        if (input[0] == '\r') { break; }

        for (i = 0; input[i] != TERM && bSize < 13311; i++, filePtr++, bSize++) {
            *filePtr = input[i];
        }
    }

    *filePtr = TERM;

    /* 
     * If file is empty, sectors = 1.
     * 
     * Otherwise:
     * sectors = byteSize / 512
     *      + 0 if there is no remainder
     *      + 1 if there is a remainder
     */
    sectors = bSize == 0 ? 1 : bSize / 512 + (mod(bSize, 512) != 0);

    syscall(8, file, filename, sectors);
}