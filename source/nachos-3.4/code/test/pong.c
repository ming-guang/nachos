/* pong.c
 *     Print the character 'B' 100 times.
 */

#include "syscall.h"

#define PrintTime 100

int main() {
    int i = 0;
    for(i = 0; i < PrintTime; ++i)
        Write("B", -1, ConsoleOutput);
    Exit(0);
    return 0;
}
