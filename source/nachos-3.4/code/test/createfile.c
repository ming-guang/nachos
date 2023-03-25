/* createfile.c
 *    Test program that create a new blank file.
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
    char name[StrMaxLength];
    OpenFileId file = -1;
    Write("File name: ", -1, ConsoleOutput);
    if(Read(name, StrMaxLength, ConsoleInput) <= 0){
        Write("Invalid file name.\n", -1, ConsoleOutput);
        return -1;
    }
    if(Create(name) < 0){
        Write("File creation failed.\n", 1, ConsoleOutput);
        return -1;
    }
    Write("File created!\n", -1, ConsoleOutput);
    return 0;
}
