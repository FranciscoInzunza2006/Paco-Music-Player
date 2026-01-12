
#include <stdio.h>
#include <locale.h>
#include <raylib.h>
#include "filesystem.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int main() {
    printf("Hello, World!\n");

    const wchar_t* music_folder_path = L"D:\\Música\\OFF";
    FilesystemDirectoryContents* music_folder_contents = filesystemDirectoryGetContents(music_folder_path);

    setlocale(LC_ALL, "");

    printf("%ls contents: \n", music_folder_path);
    for (size_t i = 0; i < music_folder_contents->count; i++) {
        printf("\t %ls\n", music_folder_contents->entries[i].name);
    }

    InitWindow(500, 500, "Paco's Music Player");
    InitAudioDevice();
    SetTargetFPS(60);

    //size_t full_path_length = wcslen(music_folder_path) + wcslen(music_folder_contents->entries[0].name) + 1;
    wchar_t full_path[FILESYSTEM_MAX_PATH_LENGTH];
    wcscpy(full_path, music_folder_path);
    wcscat(full_path, L"\\");
    wcscat(full_path, music_folder_contents->entries[0].name);

    Music music = LoadMusicStream("C:/mini1111.xm");

    PlayMusicStream(music);
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        if (IsKeyPressed(KEY_SPACE)) {
            PlayMusicStream(music);
            printf("Playing!");
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Now playing: ", 50, 50, 20, BLACK);
        //DrawText(music_folder_contents->entries[0].name, 70, 80, 20, BLACK);
        EndDrawing();
    }


    filesystemFreeDirectoryContents(music_folder_contents);
    music_folder_contents = nullptr;

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
