//
// Created by Francisco Javier on 16/01/2026.
//

#include "interface.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raylib.h"
#include "music_player.h"

static void processState(GuiLayoutState* state);

static void drawAndUpdateState(GuiLayoutState* state);

GuiLayoutState interfaceInit() {
    GuiLayoutState state = {0};

    InitWindow(INTERFACE_WINDOW_WIDTH, INTERFACE_WINDOW_HEIGHT, INTERFACE_WINDOW_DEFAULT_TITLE);
    InitAudioDevice();

    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(INTERFACE_TARGET_FPS);

    return state;
}

bool interfaceShouldUpdate() {
    return !WindowShouldClose();
}

void interfaceUpdate(GuiLayoutState* state) {
    processState(state);
    drawAndUpdateState(state);
}

void interfaceCleanUp() {
    CloseAudioDevice();
    CloseWindow();
}

static void processState(GuiLayoutState* state) {
    (void) (state);
}

static void drawAndUpdateState(GuiLayoutState* state) {
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // Album
    GuiDummyRec((Rectangle){24, 24, 256, 256}, nullptr);
    GuiLabel((Rectangle){24, 280, 256, 32}, "Album name");

    // Album selection
    GuiListView((Rectangle){24, 312, 256, 120},
                "ONE;TWO;THREE",
                &state->listview_albumsScrollIndex, &state->listview_albumsActive);

    // Album track selection
    GuiListView((Rectangle){296, 24, 480, 280},
                "ONE;TWO;THREE;",
                &state->listview_album_tracksScrollIndex, &state->listview_album_tracksActive);

    // Controls
    state->button_shuffle_pressed = GuiButton((Rectangle){320, 320, 32, 32}, "#077#");

    const char* toggle_button_icon = GuiIconText(
        musicPlayer_isMusicPlaying() ? ICON_PLAYER_PLAY : ICON_PLAYER_PAUSE, nullptr);
    if (GuiButton((Rectangle){424, 320, 32, 32}, toggle_button_icon))
        musicPlayer_toggleMusicPlaying();

    state->button_previous_pressed = GuiButton((Rectangle){376, 320, 32, 32}, "#129#");
    state->button_next_pressed = GuiButton((Rectangle){472, 320, 32, 32}, "#134#");

    // Volume bar
    GuiSliderBar((Rectangle){536, 328, 224, 16},
                 GuiIconText(ICON_AUDIO, nullptr),
                 TextFormat("%d", (int) (state->sliderbar_volumeValue * 100)),
                 &state->sliderbar_volumeValue, 0, 1);
    if (state->sliderbar_volumeValue != musicPlayer_getVolume()) {
        musicPlayer_setVolume(state->sliderbar_volumeValue);
    }

    // Time played bar
    const float time_played = musicPlayer_getTimePlayed();
    const float time_length = musicPlayer_getTimeLength();
    const char* time_played_string = TextFormat("%02d:%02d", (int) (time_played / 60.0f), (int) time_played % 60);
    const char* time_length_string = TextFormat("%02d:%02d", (int) (time_length / 60.0f), (int) time_length % 60);
    const float progress = time_played / time_length;
    float slider_bar_progress = progress;
    GuiSliderBar((Rectangle){376, 376, 384, 16}, time_played_string, time_length_string, &slider_bar_progress, 0, 1);
    if (fabsf(progress - slider_bar_progress) > 0.01f) {
        musicPlayer_setMusicPosition(slider_bar_progress);
    }

    // Track name
    const char* track_name = albums.items[current_album_index].tracks.items[current_track_index].title;
    GuiLabel((Rectangle){392, 400, 384, 32}, GuiIconText(ICON_FILETYPE_AUDIO, track_name));

    EndDrawing();
}
