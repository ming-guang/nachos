/* scheduler.c
 *     Run the "ping" and "pong" program.
 *     Assume that called from
 *         the parent of "test" folder.
 */

#include "syscall.h"

int main() {
    int pingProc, pongProc;
    pingProc = Exec("./test/ping");
    pongProc = Exec("./test/pong");
    if(pingProc == -1 || pongProc == -1){
        Write("Unable to fork new thread", -1, ConsoleOutput);
        Halt();
        return 0;
    }
    while(Running(pingProc) == 0 || Running(pongProc) == 0){ }
    return 0;
}
