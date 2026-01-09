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

    constexpr size_t FILESYSTEM_MAX_PATH_LENGTH = 260;

    typedef struct {
        wchar_t* name;
        bool is_directory;
    } FilesystemEntry;

    typedef struct {
        wchar_t* path;
        FilesystemEntry* entries;
        size_t count;
        size_t capacity;
    } FilesystemDirectoryContents;

bool filesystemExists(const wchar_t *path);
FilesystemDirectoryContents* filesystemDirectoryGetContents(const wchar_t *directory_path);
    void FilesystemFreeDirectoryContents(FilesystemDirectoryContents* contents);

// Returns pointer, your problem now.
wchar_t *toWideChar(const char *str);

#ifdef __cplusplus
}
#endif
