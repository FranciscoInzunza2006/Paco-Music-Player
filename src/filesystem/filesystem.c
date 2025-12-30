
#include "filesystem.h"
#include <stdlib.h>
#include <windows.h>

bool getDirectoryList(const char* path, DirectoryList** list) {
    *list = malloc(sizeof(DirectoryList) * 256);

    WIN32_FIND_DATAA find_data;
    FindFirstFileA(path, &find_data);

    return true;
}

void freeDirectoryList(DirectoryList* list) {
    free(list);
}
