//
// Created by Francisco Javier on 29/12/2025.
//

#include "filesystem.h"
#include <gtest/gtest.h>

#define SAMPLES_DIRECTORY_PATH L"C:\\Projects\\Paco's Music Player\\tests\\filesystem\\samples"

TEST(FilesystemTest, existsFile) {
    wchar_t *file_path = toWideChar(__FILE__);
    const bool exists = filesystemExists(file_path);

    EXPECT_TRUE(exists);

    wchar_t *non_existent_file_path = toWideChar(__FILE__ "---");
    const bool exists2 = filesystemExists(non_existent_file_path);
    EXPECT_FALSE(exists2);

    free(file_path);
    free(non_existent_file_path);
}

TEST(FilesystemTest, existsFolder) {
    const wchar_t* folder_path = SAMPLES_DIRECTORY_PATH;
    const bool exists = filesystemExists(folder_path);
    EXPECT_TRUE(exists);

    const wchar_t* folder_path2 = SAMPLES_DIRECTORY_PATH "---";
    const bool exists2 = filesystemExists(folder_path2);
    EXPECT_FALSE(exists2);
}

TEST(FilesystemTest, existsWildcard) {
    const wchar_t* wildcard_path = SAMPLES_DIRECTORY_PATH "\\*";
    const bool exists = filesystemExists(wildcard_path);
    EXPECT_TRUE(exists);
}

TEST(FilesystemTest, getContents) {
    const wchar_t* search_path = SAMPLES_DIRECTORY_PATH;
    ASSERT_TRUE(filesystemExists(search_path));

    const FilesystemDirectoryContents* contents = filesystemDirectoryGetContents(search_path);

    //ASSERT_TRUE(entries.count == 3);
    ASSERT_TRUE(contents != nullptr);

    for (size_t i = 0; i < contents->count; i++) {
        const FilesystemEntry entry = contents->entries[i];
        printf("\t - %ls\n", entry.name);
    }
}
