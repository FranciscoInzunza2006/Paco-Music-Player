//#include "raylib.h"
#include <stdio.h>

#include "filesystem/filesystem.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(void); // Update and Draw one frame

int main() {
    DirectoryList* dl;
    getDirectoryList("C:\\Users\\Francisco Javier\\foo.txt", &dl);

    for (size_t i = 0; i < dl->count; i++) {
        printf("%s\n", dl->entries[i].name);
    }

    freeDirectoryList(dl);

    return 0;

    // InitWindow(screenWidth, screenHeight, "Paco's Music Player");
    // SetTargetFPS(24);
    // while (!WindowShouldClose())
    // {
    //     UpdateDrawFrame();
    // }
    //
    // CloseWindow();
    // return 0;
}

void UpdateDrawFrame(void) {
    // // Update
    //
    // // Draw
    // BeginDrawing();
    // ClearBackground(BLACK);
    //
    // DrawText("Your mom", 190, 200, 20, RAYWHITE);
    //
    // EndDrawing();
}
