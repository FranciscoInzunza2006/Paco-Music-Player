//
// Created by Francisco Javier on 29/12/2025.
//

#include "filesystem.h"
#include <gtest/gtest.h>

constexpr const wchar_t* SAMPLE_PATH = L"C:\\Projects\\Paco's Music Player\\tests\\filesystem\\samples";

TEST(FilesystemTest, existsFile) {
    hello();
    //const wchar_t *filePath = toWideChar("D:\\Imágenes\\Religión\\Aprobado.png");
    ASSERT_TRUE(filesystemExists(L"D:\\Imágenes\\Religión\\Aprobado.png"));
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