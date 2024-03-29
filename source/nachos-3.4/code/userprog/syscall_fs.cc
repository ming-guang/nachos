#include "system.h"
#include "syscall.h"
#include "syscall_handler.h"

#include <strings.h>

int SyscallFS::Handle(int type) {
    switch(type){
        case SC_Create:
            return SyscallFS::Create();

        case SC_Open:
            return SyscallFS::Open();

        case SC_Close:
            return SyscallFS::Close();

        case SC_Read:
            return SyscallFS::Read();

        case SC_Write:
            return SyscallFS::Write();

        case SC_Delete:
            return SyscallFS::Delete();

        case SC_Seek:
            return SyscallFS::Seek();
    }
    return -1;
}

int SyscallFS::Create() {
    int nameAddr = machine -> ReadRegister(4);
    char *name = machine -> BorrowString(nameAddr);
    if(name == NULL){
        DEBUG('a', "Unable to read file name");
        return -1;
    }
    bool success = fileSystem -> Create(name);
    delete [] name;
    return -1 * !success;
}

int SyscallFS::Open() {
    int nameAddr = machine -> ReadRegister(4);
    char *name = machine -> BorrowString(nameAddr);
    if(name == NULL){
        DEBUG('a', "Unable to read file name");
        return -1;
    }
    int mode = machine -> ReadRegister(5);
    OpenFile *file = fileSystem -> Open(name, mode);
    delete [] name; 
    if(file == NULL){
        DEBUG('a', "Unknown error occur on open file");
        return -1;
    }
    return file -> oid;
}

int SyscallFS::Close() {
    OpenFileId oid = machine -> ReadRegister(4);
    return -1 * !fileSystem -> Close(oid);
}

int SyscallFS::Read() {
    int toAddr = machine -> ReadRegister(4);
    int size = machine -> ReadRegister(5);
    int oid = machine -> ReadRegister(6);
    OpenFile *file = fileSystem -> Get(oid);
    if(file == NULL){
        DEBUG('a', "Can't open file with an OpenFileId of %d", oid);
        return -1;
    }
    char *buffer = new char[size];
    if(buffer == NULL){
        DEBUG('a', "Unable to allocate read buffer");
        return -1;
    }
    int count = file -> Read(buffer, size);
    if(count <= 0){
        delete [] buffer;
        return -1;
    }
    if(!machine -> TransferMemory(buffer, count, toAddr)){
        DEBUG('a', "Unable to transfer read buffer to user space");
        count = -1;
    }
    delete [] buffer;
    return count;
}

int SyscallFS::Write() {
    int fromAddr = machine -> ReadRegister(4);
    int size = machine -> ReadRegister(5);
    int oid = machine -> ReadRegister(6);
    char *buffer = NULL;
    OpenFile *file = fileSystem -> Get(oid);
    if(file == NULL){
        DEBUG('a', "Can't open file with an OpenFileId of %d", oid);
        return -1;
    }
    if(size < 0){
        if(!(buffer = machine -> BorrowString(fromAddr))){
            DEBUG('a', "Unable to transfer string write buffer to kernel space");
            return -1;
        }
        size = strlen(buffer);
    } else if(!(buffer = machine -> BorrowMemory(fromAddr, size))){
        DEBUG('a', "Unable to transfer write buffer to kernel space");
        return -1;
    }
    int count = file -> Write(buffer, size);
    delete [] buffer;
    return count;
}

int SyscallFS::Delete() {
    int nameAddr = machine -> ReadRegister(4);
    char *name = machine -> BorrowString(nameAddr);
    if(name == NULL){
        DEBUG('a', "Unable to read file name");
        return -1;
    }
    bool success = fileSystem -> Remove(name);
    delete [] name;
    return -1 * !success;
}

int SyscallFS::Seek() {
    int position = machine -> ReadRegister(4);
    int oid = machine -> ReadRegister(5);
    OpenFile *file = fileSystem -> Get(oid);
    if(file == NULL){
        DEBUG('a', "Can't open file with an OpenFileId of %d", oid);
        return -1;
    }
    if(!file -> Seek(position)){
        DEBUG('a', "Can't seek file with an OpenFileId of %d", oid);
        return -1;
    }
    return 0;
}
