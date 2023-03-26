/* delete.c
 *    Test program that delete requested file.
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
    char name[StrMaxLength];
    Write("Input file: ", -1, ConsoleOutput);
    if(Read(name, StrMaxLength, ConsoleInput) <= 0){
        Write("Invalid file name.\n", -1, ConsoleOutput);
        return -1;
    }
    if(Delete(name) < 0){
        Write("Unable to delete file ", -1, ConsoleOutput);
        Write(name, -1, ConsoleOutput);
        Write("\n", -1, ConsoleOutput);
        return -1;
    }
    Write("File deleted!\n", -1, ConsoleOutput);
    return 0;
}
