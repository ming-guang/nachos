#include "system.h"
#include "syscall.h"

int SyscallFS::Handle(int type) {
    switch(type){
        case SC_Create:
            return SyscallFS::Create();

        case SC_Open:
            return SyscallFS::Open();

        case SC_Read:
            return SyscallFS::Read();

        case SC_Write:
            return SyscallFS::Write();
    }
    return -1;
}
