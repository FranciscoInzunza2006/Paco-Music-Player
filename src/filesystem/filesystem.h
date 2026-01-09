// So I need to get all the entries in the given directory
// I won't bother with recursive searches (although I should consider them)

// Sounds simple, give path, get entry. Repeat to get all entries.

#pragma once
#include <stddef.h>
#include <windows.h>

#ifdef _WIN32
  #ifdef FILESYSTEM_EXPORTS
    #define FS_API __declspec(dllexport)
  #else
    #define FS_API __declspec(dllimport)
  #endif
#else
  #define FS_API
#endif

#ifdef __cplusplus
extern "C" {

#endif

constexpr size_t FILESYSTEM_MAX_PATH_LENGTH = 260;

typedef struct {
    wchar_t *name;
    bool is_directory;
} FilesystemEntry;

typedef struct {
    wchar_t *path;
    FilesystemEntry *entries;
    size_t count;
    size_t capacity;
} FilesystemDirectoryContents;

    /// Takes a path to a file or directory and returns if it was found/exists.
FS_API bool filesystemExists(const wchar_t *path);

/// @param directory_path Valid directory path
/// @return A pointer to a data structure with the contents of the directory. Free using the respective function.
FS_API FilesystemDirectoryContents *filesystemDirectoryGetContents(const wchar_t *directory_path);

    /// Releases all the memory of the FilesystemDirectoryContents structure pointed to. This functions takes also releases all the entries.
FS_API void filesystemFreeDirectoryContents(FilesystemDirectoryContents *contents);

/// Converts a ASCII array to a wide character array. Caller gets the ownership.
FS_API wchar_t *toWideChar(const char *str);

#ifdef __cplusplus
}
#endif
