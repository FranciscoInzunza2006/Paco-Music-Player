
#include <raylib.h>
#include <stddef.h>

void loadMusicIndex(Music* music, FilePathList* files, size_t index);

int main() {
    constexpr char music_directory_path[] = "D:\\Musica\\OFF";
    FilePathList music_files = LoadDirectoryFiles(music_directory_path);
    Music music = {0};
    size_t current_music_file_index = 0;

    InitWindow(500, 500, "Paco's Music Player");
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    InitAudioDevice();

    loadMusicIndex(&music, &music_files, current_music_file_index);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_LEFT)) {
            if (current_music_file_index > 0)
                current_music_file_index--;
            loadMusicIndex(&music, &music_files, current_music_file_index);
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (current_music_file_index < music_files.count-1)
                current_music_file_index++;
            loadMusicIndex(&music, &music_files, current_music_file_index);
        }

        UpdateMusicStream(music);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Currently playing: ", 50, 0, 20, BLACK);
        DrawText(GetFileNameWithoutExt(music_files.paths[current_music_file_index]), 80, 80, 20, BLACK);

        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}

void loadMusicIndex(Music* music, FilePathList* files, size_t index) {
    UnloadMusicStream(*music);
    *music = LoadMusicStream(files->paths[index]);
    PlayMusicStream(*music);
}