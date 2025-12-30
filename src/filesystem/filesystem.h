
// So I need to get all the entries in the given directory
// I won't bother with recursive searches (although I should consider them)

// Sounds simple, give path, get entry. Repeat to get all entries.

#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    const char* name;
    bool is_file;
} DirectoryEntry;

typedef struct {
    DirectoryEntry* entries;
    size_t count;
} DirectoryList;

bool getDirectoryList(const char* path, DirectoryList** list);
void freeDirectoryList(DirectoryList* list);
