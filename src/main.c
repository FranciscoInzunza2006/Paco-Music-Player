#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(void); // Update and Draw one frame

int main() {
    InitWindow(screenWidth, screenHeight, "Paco's Music Player");
    SetTargetFPS(24);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void) {
    // Update

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Your mom", 190, 200, 20, RAYWHITE);

    EndDrawing();
}
