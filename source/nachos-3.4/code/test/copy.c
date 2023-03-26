/* copy.c
 *    Test program that clone file A content to file B.
 */

#include "syscall.h"

#define StrMaxLength 256
#define BufferSize 256

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

OpenFileId RequestFile(char *message, int create){
    OpenFileId file = -1;
    char name[StrMaxLength];
    Write(message, -1, ConsoleOutput);
    if(Read(name, StrMaxLength, ConsoleInput) <= 0){
        Write("Invalid file name.\n", -1, ConsoleOutput);
        return -1;
    }
    if(create){
        if(Create(name) < 0){
            Write("Unable to create file \"", -1, ConsoleOutput);
            Write(name, -1, ConsoleOutput);
            Write("\"\n", -1, ConsoleOutput);
            return -1;
        }
    }
    file = Open(name, FileCanRead | FileCanWrite);
    if(file < 0){
        Write("Unable to open file \"", -1, ConsoleOutput);
        Write(name, -1, ConsoleOutput);
        Write("\"\n", -1, ConsoleOutput);
        return -1;
    }
}

int main() {
    int count = 0;
    char buffer[BufferSize];
    OpenFileId src = -1, dst = -1;
    src = RequestFile("Source file: ", 0);
    if(src < 0){ 
        return -1;
    }
    dst = RequestFile("Destination file: ", 1);
    if(dst < 0){
        Close(src);
        return -1;
    }
    while(1){
        count = Read(buffer, BufferSize, src);
        Write(buffer, count, dst);
        if(count < BufferSize)
            break;
    }
    Close(src);
    Close(dst);
    return 0;
}
