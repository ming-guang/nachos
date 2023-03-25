/* cat.c
 *    Test program print the entire file content.
 */

#include "syscall.h"

#define StrMaxLength 256
#define BufferSize 256

int ReadLine(char *buffer, int maxLength, OpenFileId oid) {
    int count = 0;
    for(int i = 0; i < maxLength; ++i){
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
    char name[StrMaxLength], buffer[BufferSize];
    OpenFileId file = -1;
    Write("Input file: ", 32, ConsoleOutput);
    if(Read(name, StrMaxLength, ConsoleInput) <= 0){
        Write("Invalid file name.\n", 32, ConsoleOutput);
        return -1;
    }
    file = Open(name);
    if(file < 0){
        Write("Unable to open file ", 32, ConsoleOutput);
        Write(name, StrMaxLength, ConsoleOutput);
        Write("\n", 1, ConsoleOutput);
        return -1;
    }
    while(true){
        count = Read(buffer, BufferSize, file);
        Write(buffer, count, ConsoleOutput);
        if(count < BufferSize)
            break;
    }
    return 0;
}
