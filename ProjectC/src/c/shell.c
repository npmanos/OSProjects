/* 
    COMP 350 OS Shell
    Project C
    Author: Nick Manos
    shell.c
*/

#define COM_NUM 3

void print(char *);
void printLn(char *);
void readLn(char *);
void file(char *);
void type(char *);
void exec(char *);
void getArg(char *, char *);

void main() {
    char *input;
    char *arg;
    char *commands[COM_NUM];
    char TERM = '\0';
    char *CRLF = "\r\n";
    char *typeStr = "type";
    char *execStr = "exec";
    char *testCom, *inCom;
    int com;
    int match;
    char *prMatch;
    int c;
    commands[0] = CRLF;
    commands[1] = typeStr;
    commands[2] = execStr;

    printLn("COMP 350 OS vC\r\n");

    while (1) {
        input = "";
        print("/$ ");
        readLn(input);

        for (com = 0; com < COM_NUM; com++)
        {
            match = 0; 
            for (inCom = input, testCom = commands[com]; *inCom != ' ' && *inCom != TERM; inCom++, testCom++)
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