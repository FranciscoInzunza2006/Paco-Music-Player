
// So I need to get all the entries in the given directory
// I won't bother with recursive searches (although I should consider them)

// Sounds simple, give path, get entry. Repeat to get all entries.

#pragma once
#include <stddef.h>
#include <windows.h>

// TODO: Convert library into a dynamic one

#ifdef __cplusplus
extern "C" {
#endif

void hello();

bool filesystemExists(const wchar_t* path);

// Returns pointer, your problem now.
wchar_t* toWideChar(const char* str);

#ifdef __cplusplus
}
#endif