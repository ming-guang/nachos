#include "system.h"
#include "syscall.h"
#include "syscall_handler.h"

int SyscallProcess::Handle(int type) {
    switch(type){
        case SC_Exec:
            return SyscallProcess::Exec();

        case SC_Running:
            return SyscallProcess::Running();
    }
    return -1;
}

int SyscallProcess::Exec() {
    int nameAddr = machine -> ReadRegister(4);
    char *name = machine -> BorrowString(nameAddr);
    if(name == NULL){
        DEBUG('a', "Unable to read file name");
        return -1;
    }
    delete [] name;
    return -1;
}

int SyscallProcess::Running() {
    int procId = machine -> ReadRegister(4);
    return -1;
}
