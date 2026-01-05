
#include "filesystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

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

void hello() {
    printf("hello");
}

bool filesystemExists(const wchar_t* path) {
    WIN32_FIND_DATAW _;
    HANDLE h = FindFirstFileW(path, &_);
    if (h != INVALID_HANDLE_VALUE) return true;

    const DWORD error = GetLastError();
    if (error != ERROR_FILE_NOT_FOUND) {
        printf("Filesystem exists error!");
    }
    return false;
}

const wchar_t* toWideChar(const char* str) {
    const size_t string_length = mbrtowc(nullptr, str, 0, nullptr) + 1;
    wchar_t* wstr = malloc(sizeof(wchar_t) * string_length);
    mbrtowc(wstr, str, string_length, nullptr);

    return wstr;
}