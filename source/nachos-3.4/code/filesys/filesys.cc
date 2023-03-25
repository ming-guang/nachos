// filesys.cc 
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk 
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them 
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#define SlowCapacityGrow 1024

#include "copyright.h"

#include "openfile.h"
#include "filesys.h"

//----------------------------------------------------------------------
// FileSystem::FileSystem
//   Ignore the "format" args.
//   Reserve index 0 and 1 on the filesystem table for STDIN and STDOUT.
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{
    this -> table = NULL;
    this -> capacity = 0;
    this -> Open(STDIN, 0);
    this -> Open(STDOUT, 0);
}

//----------------------------------------------------------------------
// FileSystem::~FileSystem
//   Close all the opened file.
//   Table is always been allocated via opening STDIN and STDOUT
//      so no need to check if it is NULL.
//----------------------------------------------------------------------

FileSystem::~FileSystem()
{
    for(int i = 0; i < this -> capacity; ++i)
        if(this -> table[i])
            delete this -> table[i];
    delete [] this -> table;
}

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading or writing or both. 
//	To open a file:
//	  Append the newly created "OpenFile" to any free slot
//	  of the filesystem table.
//
//	"name" -- the text name of the file to be opened.
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(char *name)
{
    return this -> Open(name, FileCanRead | FileCanWrite);
}

OpenFile *
FileSystem::Open(char *name, int mode)
{
    int free_slot = -1;
    for(int i = 0; i < this -> capacity; ++i)
        if(this -> table[i] == NULL){
            free_slot = i;
            break;
        }

    if(free_slot < 0){
        if(!this -> GrowTable())
            return NULL;
        return this -> Open(name, mode);
    }
    OpenFile *file = new OpenFile(name, mode);
    if(!file)
        return NULL;
    if(!file -> Opened()){
        DEBUG('a', "Unable to open file \"%s\"", name);
        delete file;
        return NULL;
    }
    file -> oid = free_slot;
    this -> table[free_slot] = file;
    return file; 
}

//----------------------------------------------------------------------
// FileSystem::Get
// 	Get the already opened file in the filesystem table.
//----------------------------------------------------------------------
OpenFile *
FileSystem::Get(int oid)
{
    if(oid < 0)
        return NULL;
    if(oid >= this -> capacity)
        return NULL;
    return this -> table[oid];
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system or can't be deleted.
//----------------------------------------------------------------------

bool
FileSystem::Remove(int oid)
{
    OpenFile *file = this -> Get(oid);
    if(!file)
        return false;
    if(oid <= 1)
        return false;
    if(!file -> Unlink())
        return false;
    return this -> Close(oid);
}

//----------------------------------------------------------------------
// FileSystem::Close
//   Close the opened file.
//----------------------------------------------------------------------
bool
FileSystem::Close(int oid)
{
    return this -> Close(oid, true);
}

bool
FileSystem::Close(int oid, bool dealocate)
{
    if(!this -> Get(oid))
        return false;
    if(oid <= 1){
        DEBUG('a', "Reserved open file id can't be closed.");
        return false;
    }
    if(dealocate)
        delete this -> table[oid];
    this -> table[oid] = NULL;
    return true;
}

//----------------------------------------------------------------------
// FileSystem::GrowTable
//   Grow the filesystem table when it's is over the capacity.
//   Make the capacity "1" as default
//   Double the capacity when the capacity < 1024
//   x1.5 the capacity when the capacity is >= 1024.
//----------------------------------------------------------------------
bool
FileSystem::GrowTable()
{
    int new_capacity = 1;
    if((this -> capacity > 0) && (this -> capacity < SlowCapacityGrow)){
        new_capacity = this -> capacity << 1;
    } else if (this -> capacity >= SlowCapacityGrow){
        new_capacity += this -> capacity + this -> capacity >> 1;
    }
    OpenFile **new_table = new OpenFile*[new_capacity];
    if(!new_table)
        return false;
    for(int i = 0; i < this -> capacity; ++i)
        new_table[i] = this -> table[i];
    for(int i = this -> capacity; i < new_capacity; ++i)
        new_table[i] = NULL;
    if(this -> table)
        delete [] this -> table;
    this -> table = new_table;
    this -> capacity = new_capacity;
    return true;
}
