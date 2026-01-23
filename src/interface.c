//
// Created by Francisco Javier on 16/01/2026.
//

#include "interface.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "styles/jungle/style_jungle.h"

#include "raylib.h"
#include "music_player.h"

static void processState(GuiLayoutState* state);

static void drawAndUpdateState(GuiLayoutState* state);

static const char* formatToTime(float time_in_seconds) {
    return TextFormat("%02d:%02d", (int) (time_in_seconds / 60.0f), (int) time_in_seconds % 60);
}

static void createTrackListviewValues(ListviewValues* out, const Album* album) {
    out->names = malloc(sizeof(char*) * album->tracks.count);
    out->count = (int) album->tracks.count;
    for (size_t track_index = 0; track_index < album->tracks.count; track_index++) {
        const Track* track = &album->tracks.items[track_index];

        size_t needed_size = snprintf(nullptr, 0, "%3u. %s", track->track_number, track->title);
        char* a = malloc((needed_size + 1) * sizeof(char)); // You are allowed to do this?
        snprintf(a, needed_size + 1, "%3u. %s", track->track_number, track->title);
        out->names[track_index] = a;
    }
}

static void createAlbumListviewValues(GuiLayoutState* state) {
    const Albums* album_list = musicPlayer_getAlbumList();
    if (album_list != nullptr) {
        state->listview_albums_values.names = malloc(sizeof(char*) * album_list->count);
        state->listview_albums_values.count = (int) album_list->count;

        state->listview_tracks_values = malloc(album_list->count * sizeof(ListviewValues));
        for (size_t album_index = 0; album_index < album_list->count; album_index++) {
            const Album* album = &album_list->items[album_index];
            state->listview_albums_values.names[album_index] = album->name;

            createTrackListviewValues(&state->listview_tracks_values[album_index], album);
        }
    }
}

GuiLayoutState interfaceInit() {
    GuiLayoutState state = {0};

    state.sliderbar_volume_value = musicPlayer_getVolume();
    state.time_played = 0.0f;
    state.time_length = 0.0f;

    createAlbumListviewValues(&state);

    InitWindow(INTERFACE_WINDOW_WIDTH, INTERFACE_WINDOW_HEIGHT, INTERFACE_WINDOW_DEFAULT_TITLE);
    InitAudioDevice();

    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(INTERFACE_TARGET_FPS);

    GuiLoadStyleJungle();
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
        if (musicPlayer_isPlaying()) musicPlayer_pause();
        else musicPlayer_play();
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

    // Change track
    if (state->listview_tracks_selected) {
        musicPlayer_changeAlbum(state->listview_albums_active);
        musicPlayer_changeTrack(state->listview_tracks_active);
        state->sliderbar_progress_value = 0.0f;
    }

    // Update track info
    const Track* track = musicPlayer_getCurrentTrack();
    if (track != nullptr && (state->track_name != track->title)) {
        state->track_name = track->title;
        state->album_name = track->album;
        state->time_length = musicPlayer_getTimeLength();
    }

    if (state->sliderbar_progress_selected && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        // WARNING: This will cause issues if more... playback? (shuffle, loop album) added
        if (state->sliderbar_progress_value == 1.0f) state->sliderbar_progress_value = 0.0f;

        musicPlayer_setProgress(state->sliderbar_progress_value);
        state->sliderbar_progress_selected = false;
    }

    state->time_played = musicPlayer_getTimePlayed();

    // If not clicking bar: Update bar to correctly reflect the position in the music
    // If clicking bar: Wait until mouse is released to update position
    if (!state->sliderbar_progress_selected) {
        if (state->time_length == 0.0f) state->sliderbar_progress_value = 0.0f;
        else state->sliderbar_progress_value = state->time_played / state->time_length;
    }
}

static void drawAndUpdateState(GuiLayoutState* state) {
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // Album
    GuiDummyRec((Rectangle){24, 24, 256, 256}, nullptr);
    GuiLabel((Rectangle){24, 280, 256, 32}, state->album_name);

    // Album selection
    int album_active = state->listview_albums_active;
    GuiListViewEx((Rectangle){24, 312, 256, 120},
                  state->listview_albums_values.names, state->listview_albums_values.count,
                  &state->listview_albums_scroll_index, &album_active, nullptr);
    if (album_active != -1) state->listview_albums_active = album_active;

    // Tracklist
    const ListviewValues tracklist_titles = state->listview_tracks_values[state->listview_albums_active];

    // DIRTY HACK
    static int hacky_index = 0;

    const bool looking_same_album = strcmp(musicPlayer_getCurrentAlbum()->name,
                                           state->listview_albums_values.names[state->listview_albums_active]) == 0;
    int active = looking_same_album ? state->listview_tracks_active : -1;
    int* scroll = looking_same_album ? &state->listview_tracks_scroll_index : &hacky_index;
    const int x = active;
    GuiListViewEx((Rectangle){296, 24, 480, 280},
                  tracklist_titles.names, tracklist_titles.count,
                  scroll, &active, nullptr);
    if (active != -1) {
        state->listview_tracks_selected = active != x;
        if (state->listview_tracks_selected) {
            state->listview_tracks_active = active;
            state->listview_tracks_scroll_index = *scroll;
            hacky_index = 0;
        }
    }

    // Controls
    state->button_shuffle_pressed = GuiButton((Rectangle){320, 320, 32, 32}, "#077#");

    const char* toggle_button_icon = GuiIconText(musicPlayer_isPlaying() ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY, nullptr);
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
    GuiSliderBar((Rectangle){376, 376, 384, 16},
                 formatToTime(time_played_bar), formatToTime(state->time_length),
                 &state->sliderbar_progress_value, 0, 1);
    const float true_progress = state->time_length != 0.0f ? state->time_played / state->time_length : 0.0f;
    state->sliderbar_progress_selected = true_progress != state->sliderbar_progress_value;

    // Track name
    GuiLabel((Rectangle){392, 400, 384, 32}, GuiIconText(ICON_FILETYPE_AUDIO, state->track_name));

    EndDrawing();
}
