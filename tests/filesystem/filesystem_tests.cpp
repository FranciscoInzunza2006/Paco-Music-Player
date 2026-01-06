//
// Created by Francisco Javier on 29/12/2025.
//

#include "filesystem.h"
#include <gtest/gtest.h>

constexpr const wchar_t *SAMPLE_PATH = L"C:\\Projects\\Paco's Music Player\\tests\\filesystem\\samples";

TEST(FilesystemTest, existsFile) {
    const wchar_t *filePath = L"D:\\Imágenes\\Religión\\Aprobado.png";
    const bool exists = filesystemExists(filePath);

    ASSERT_TRUE(exists);
}

TEST(FilesystemTest, existsFolder) {
    ASSERT_TRUE(true);
}

TEST(FilesystemTest, existsWildcard) {
    ASSERT_TRUE(true);
}

TEST(FilesystemTest, getEntries) {
    ASSERT_TRUE(true);
}
