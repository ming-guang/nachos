// openfile.cc 
//	Routines to manage an open Nachos file.  As in UNIX, a
//	file must be open before we can read or write to it.
//	Once we're all done, we can close it (in Nachos, by deleting
//	the OpenFile data structure).
//
//	Also as in UNIX, for convenience, we keep the file header in
//	memory while the file is open.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "filehdr.h"
#include "openfile.h"
#include "system.h"
#ifdef HOST_SPARC
#include <strings.h>
#endif

//----------------------------------------------------------------------
// OpenFile::OpenFile
//  Using int as a 32-bit binary sequence
//      name:
//          Reserve 2 file "//stdin" and "//stdout"
//      mode:
//          First bit is to check if the file is allowed to read
//          Second bit is to check if the file is allowed to write
//          Third bit is to check if the file is using synchConsole I/O
//          Reserved file will be enforced to use 0x5 and 0x6
//----------------------------------------------------------------------

void
OpenFile::Init(char *name, int mode)
{
    this -> name = NULL;
    this -> fd = 0;
    if(strcmp(name, STDIN) == 0){
        mode = STDINMode;
    } else if(strcmp(name, STDOUT) == 0){
        mode = STDOUTMode;
    } else {
        this -> fd = OpenForReadWrite(name, false);
    }
    this -> mode = mode;
    if(this -> fd > 0)
        this -> name = strcpy(this -> name, name);
}

OpenFile::OpenFile(char *name){ 
    this -> Init(name, FileCanRead | FileCanWrite);
}

OpenFile::OpenFile(char *name, int mode){
    this -> Init(name, mode);
}

//----------------------------------------------------------------------
// OpenFile::~OpenFile
// 	Close a Nachos file, de-allocating any in-memory data structures.
//----------------------------------------------------------------------

OpenFile::~OpenFile(){
    if(this -> name != NULL)
        delete [] this -> name;
    if(this -> fd > 0)
        Close(this -> fd);
}

//----------------------------------------------------------------------
// OpenFile::Opened
// Check whether the file is opened sucessfully or not
//----------------------------------------------------------------------
bool
OpenFile::Opened()
{
    return this -> fd >= 0;
}

//----------------------------------------------------------------------
// OpenFile::Seek
// 	Change the current location within the open file -- the point at
//	which the next Read or Write will start from.
//
//	"position" -- the location within the file for the next Read/Write
//----------------------------------------------------------------------

void
OpenFile::Seek(int position)
{
    if(this -> IsConsole())
        return;
    Lseek(this -> fd, position, 0);
}

//----------------------------------------------------------------------
// OpenFile::Read/Write
// 	Read/write a portion of a file, starting from seekPosition.
//	Return the number of bytes actually written or read, and as a
//	side effect, increment the current position within the file.
//
//	Implemented using the more primitive ReadAt/WriteAt.
//
//	"into" -- the buffer to contain the data to be read from disk 
//	"from" -- the buffer containing the data to be written to disk 
//	"numBytes" -- the number of bytes to transfer
//----------------------------------------------------------------------

int
OpenFile::Read(char *into, int numBytes)
{
    if(!this -> CanRead())
        return -1;
    if(this -> IsConsole())
        return synchConsole -> Read(into, numBytes);
    return ReadPartial(this -> fd, into, numBytes);
}

int
OpenFile::Write(char *from, int numBytes)
{
    if(!this -> CanWrite())
        return -1;
    if(this -> IsConsole())
        return synchConsole -> Write(from, numBytes);
    WriteFile(this -> fd, from, numBytes);
    return numBytes;
}

int
OpenFile::ReadAt(char *into, int numBytes, int position)
{
    this -> Seek(position);
    return this -> Read(into, numBytes);
}

int
OpenFile::WriteAt(char *from, int numBytes, int position)
{
    this -> Seek(position);
    return this -> Write(from, numBytes);
}

//----------------------------------------------------------------------
// OpenFile::Unlink
//   Unlink the file from the filesystem, do not close the file
//----------------------------------------------------------------------
bool
OpenFile::Unlink()
{
    if(this -> IsConsole())
        return false;
    return ::Unlink(this -> name);
}

bool 
OpenFile::CanRead()
{
    return this -> mode & FileCanRead;
}

bool 
OpenFile::CanWrite()
{
    return this -> mode & FileCanWrite;
}

bool 
OpenFile::IsConsole()
{
    return this -> mode & FileIsConsole;
}
