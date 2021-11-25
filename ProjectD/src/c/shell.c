/* 
    COMP 350 OS Shell
    Project D
    Author: Nick Manos
    shell.c
*/

#define COM_NUM 5

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

void main() {
    char *input;
    char *arg;
    char *commands[COM_NUM];
    char TERM = '\0';
    char *CRLF = "\r\n";
    char *typeStr = "type";
    char *execStr = "exec";
    char *dirStr = "dir";
    char *delStr = "del";
    char *testCom, *inCom;
    int com;
    int match;
    char *prMatch;
    int c;
    commands[0] = CRLF;
    commands[1] = typeStr;
    commands[2] = execStr;
    commands[3] = dirStr;
    commands[4] = delStr;

    printLn("COMP 350 OS vD.3\r\n");

    while (1) {
        input = "";
        print("/$ ");
        readLn(input);

        for (com = 0; com < COM_NUM; com++)
        {
            match = 0; 
            for (inCom = input, testCom = commands[com]; *inCom != ' ' && *testCom != '\0'; inCom++, testCom++)
            {
                if (*inCom != *testCom)
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
    int c;

    for (c = 0; *input != '\r' && *input != '\0'; arg++, input++, c++)
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