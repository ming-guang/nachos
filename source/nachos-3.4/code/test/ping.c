/* ping.c
 *     Print the character 'A' 100 times.
 */

#include "syscall.h"

#define PrintTime 100

int main() {
    int i = 0;
    for(i = 0; i < PrintTime; ++i)
        Write("A", -1, ConsoleOutput);
    Exit(0);
    return 0;
}
