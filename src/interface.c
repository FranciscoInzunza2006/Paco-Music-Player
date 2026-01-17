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

    state.sliderbar_volume_value = musicPlayer_getVolume();

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
    if (state->button_play_pressed) {
        musicPlayer_toggleMusicPlaying();
    }

    if (state->button_previous_pressed) {
        musicPlayer_previousTrack();
    }
    if (state->button_next_pressed) {
        musicPlayer_nextTrack();
    }

    if (state->sliderbar_volume_value != musicPlayer_getVolume()) {
        musicPlayer_setVolume(state->sliderbar_volume_value);
    }

    state->time_played = musicPlayer_getTimePlayed();
    state->time_length = musicPlayer_getTimeLength();
    // If not clicking bar: Update bar to correctly reflect the position in the music
    // If clicking bar: Wait until mouse is released to update position
    if (!state->sliderbar_progress_value_changed) {
        state->sliderbar_progress_value = state->time_played / state->time_length;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        musicPlayer_setProgress(state->sliderbar_progress_value);
    }


    state->album_name = musicPlayer_getCurrentAlbum()->name;
    state->track_name = musicPlayer_getCurrentTrack()->title;
}

static void drawAndUpdateState(GuiLayoutState* state) {
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // Album
    GuiDummyRec((Rectangle){24, 24, 256, 256}, nullptr);
    GuiLabel((Rectangle){24, 280, 256, 32}, state->album_name);

    // Album selection
    GuiListView((Rectangle){24, 312, 256, 120},
                "ONE;TWO;THREE",
                &state->listview_albums_scroll_index, &state->listview_albums_active);

    // Album track selection
    GuiListView((Rectangle){296, 24, 480, 280},
                "ONE;TWO;THREE;",
                &state->listview_tracks_scroll_index, &state->listview_tracks_active);

    // Controls
    state->button_shuffle_pressed = GuiButton((Rectangle){320, 320, 32, 32}, "#077#");

    const char* toggle_button_icon = GuiIconText(
        musicPlayer_isPlaying() ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY, nullptr);
    state->button_play_pressed = GuiButton((Rectangle){424, 320, 32, 32}, toggle_button_icon);

    state->button_previous_pressed = GuiButton((Rectangle){376, 320, 32, 32}, "#129#");
    state->button_next_pressed = GuiButton((Rectangle){472, 320, 32, 32}, "#134#");

    // Volume bar
    GuiSliderBar((Rectangle){536, 328, 224, 16},
                 GuiIconText(ICON_AUDIO, nullptr),
                 TextFormat("%d", (int) (state->sliderbar_volume_value * 100)),
                 &state->sliderbar_volume_value, 0, 1);

    // Time played bar
    const float time_played_bar = state->sliderbar_progress_value * state->time_length;
    const char* time_played_string =
            TextFormat("%02d:%02d", (int) (time_played_bar / 60.0f), (int) time_played_bar % 60);
    const char* time_length_string =
            TextFormat("%02d:%02d", (int) (state->time_length / 60.0f), (int) state->time_length % 60);

    GuiSliderBar((Rectangle){376, 376, 384, 16},
                 time_played_string, time_length_string,
                 &state->sliderbar_progress_value, 0, 1);

    const float true_progress = state->time_played / state->time_length;
    state->sliderbar_progress_value_changed = true_progress != state->sliderbar_progress_value;
    printf("%f = %f\n", true_progress, state->sliderbar_progress_value);

    // Track name
    GuiLabel((Rectangle){392, 400, 384, 32}, GuiIconText(ICON_FILETYPE_AUDIO, state->track_name));

    EndDrawing();
}
