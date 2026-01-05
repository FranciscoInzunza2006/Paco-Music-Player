
// So I need to get all the entries in the given directory
// I won't bother with recursive searches (although I should consider them)

// Sounds simple, give path, get entry. Repeat to get all entries.

#pragma once
#include <stddef.h>
#include <windows.h>

// TODO: Convert library into a dynamic one

// typedef struct {
//     const char* name;
//     bool is_file;
// } DirectoryEntry;
//
// typedef struct {
//     DirectoryEntry* entries;
//     size_t count;
// } DirectoryList;

void hello();

bool filesystemExists(const wchar_t* path);

// bool getDirectoryList(const char* path, DirectoryList** list);
// void freeDirectoryList(DirectoryList* list);

// Returns pointer, your problem now.
const wchar_t* toWideChar(const char* str);