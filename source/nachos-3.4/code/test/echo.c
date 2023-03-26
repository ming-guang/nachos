/* echo.c
 *    Test program that will re-print user input.
 */

#include "syscall.h"

#define StrMaxLength 256

int ReadLine(char *buffer, int maxLength, OpenFileId oid) {
    int count = 0, i = 0;
    for(i = 0; i < maxLength; ++i){
        Read(&buffer[i], 1, oid);
        ++count;
        if(buffer[i] == '\n' || buffer[i] == 13 || buffer[i] == 0){
            buffer[i] = 0;
            break;
        }
    }
    buffer[maxLength - 1] = 0;
    return count;
}

int main() {
    int count = 0;
    char inp[StrMaxLength];
    while(1){
        Write("<- ", -1, ConsoleOutput);
        count = ReadLine(inp, StrMaxLength, ConsoleInput);
        if(count <= 0){
            return 0;
        }
        Write("-> ", -1, ConsoleOutput);
        Write(inp, count, ConsoleOutput);
        Write("\n", -1, ConsoleOutput);
    }
    return 0;
}
