// openfile.h 
//	Data structures for opening, closing, reading and writing to 
//	individual files.  The operations supported are similar to
//	the UNIX ones -- type 'man open' to the UNIX prompt.
//
//	There are two implementations.  One is a "STUB" that directly
//	turns the file operations into the underlying UNIX operations.
//	(cf. comment in filesys.h).
//
//	The other is the "real" implementation, that turns these
//	operations into read and write disk sector requests. 
//	In this baseline implementation of the file system, we don't 
//	worry about concurrent accesses to the file system
//	by different threads -- this is part of the assignment.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef OPENFILE_H
#define OPENFILE_H

#define STDIN "//stdin"
#define STDOUT "//stdout"
#define FileCanReadBit 0
#define FileCanWriteBit 1
#define FileIsConsoleBit 2
#define STDINMode 0x5
#define STDOUTMode 0x6

#include "copyright.h"
#include "utility.h"

class FileHeader;

class OpenFile {
  public:
    OpenFile(char *name, int mode);
    ~OpenFile();			// Close the file

    void Seek(int position); 		// Set the position from which to 
    int Read(char *into, int numBytes); // Read/write bytes from the file
    int Write(char *from, int numBytes);
    
  private:
    int fd;                     // File descriptor of the file
    int mode;                   // File open mode
    bool CanRead();
    bool CanWrite();
    bool IsConsole();
};

#endif // OPENFILE_H
