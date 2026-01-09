#include "filesystem.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

// bool getDirectoryList(const char* path, DirectoryList** list) {
//     *list = malloc(sizeof(DirectoryList) * 256);
//
//     WIN32_FIND_DATAA find_data;
//     FindFirstFileA(path, &find_data);
//
//     return true;
// }
//
// void freeDirectoryList(DirectoryList* list) {
//     free(list);
// }


bool filesystemExists(const wchar_t *path) {
    WIN32_FIND_DATAW _;
    HANDLE h = FindFirstFileW(path, &_);
    if (h != INVALID_HANDLE_VALUE) return true;

    const DWORD error = GetLastError();
    if (error != ERROR_FILE_NOT_FOUND) {
        printf("Filesystem exists error!");
    }
    return false;
}

FilesystemDirectoryContents *filesystemDirectoryGetContents(const wchar_t *directory_path) {
    const size_t directory_path_length = wcsnlen(directory_path, FILESYSTEM_MAX_PATH_LENGTH);
    if (directory_path_length + 3 > FILESYSTEM_MAX_PATH_LENGTH) {
        printf("Directory path length exceeds %llu characters!\n", FILESYSTEM_MAX_PATH_LENGTH);
        return nullptr;
    }

    wchar_t *search_path = alloca(sizeof(wchar_t) * (directory_path_length + 3));
    wcsncpy(search_path, directory_path, directory_path_length + 1);
    wcsncat(search_path, L"\\*", 2);

    WIN32_FIND_DATAW find_file_data;
    HANDLE h = FindFirstFileW(search_path, &find_file_data);

    //setlocale(LC_ALL, "");
    if (h == INVALID_HANDLE_VALUE) {
        //printf("FindFirstFile failed (%ld)\n", GetLastError());
        return nullptr;
    }

    FilesystemDirectoryContents *contents = malloc(sizeof(FilesystemDirectoryContents));
    *contents = (FilesystemDirectoryContents){
        .path = malloc(sizeof(wchar_t) * (directory_path_length + 1)),
        .entries = nullptr,
        .count = 0,
        .capacity = 0
    };
    wcsncpy(contents->path, directory_path, directory_path_length + 1);

    do {
        const bool is_link_directory = wcsncmp(L".", find_file_data.cFileName, FILESYSTEM_MAX_PATH_LENGTH) == 0
                                 || wcsncmp(L"..", find_file_data.cFileName, FILESYSTEM_MAX_PATH_LENGTH) == 0;
        if (is_link_directory) continue;

        if (contents->count >= contents->capacity) {
            if (contents->capacity == 0) contents->capacity = 4;
            else contents->capacity *= 2;

            contents->entries = realloc(contents->entries, contents->capacity * sizeof(FilesystemEntry));
        }

        FilesystemEntry *entry = &contents->entries[contents->count++];

        const size_t filename_length = wcsnlen(find_file_data.cFileName, FILESYSTEM_MAX_PATH_LENGTH);
        entry->name = malloc(sizeof(wchar_t) * (filename_length + 1));
        wcsncpy(entry->name, find_file_data.cFileName, filename_length + 1);

        entry->is_directory = (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != false;

        //printf("%ls\n", find_file_data.cFileName);
    } while (FindNextFileW(h, &find_file_data) != 0);

    //printf("The first file found is %ls\n", find_file_data.cFileName);
    FindClose(h);

    return contents;
}

void filesystemFreeDirectoryContents(FilesystemDirectoryContents *contents) {
    if (contents == nullptr) return;

    if (contents->entries != nullptr) {
        for (size_t i = 0; i < contents->count; i++) free(contents->entries[i].name);
        free(contents->entries);
    }

    free(contents->path);
    free(contents);
}

wchar_t *toWideChar(const char *str) {
    const size_t string_length = mbstowcs(nullptr, str, 0) + 1;
    wchar_t *wstr = malloc(sizeof(wchar_t) * string_length);
    mbstowcs(wstr, str, string_length);

    return wstr;
}
