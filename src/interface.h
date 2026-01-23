//
// Created by Francisco Javier on 16/01/2026.
//

#pragma once

constexpr int INTERFACE_WINDOW_WIDTH = 800;
constexpr int INTERFACE_WINDOW_HEIGHT = 450;
constexpr char INTERFACE_WINDOW_DEFAULT_TITLE[] = "Paco's Music Player";
constexpr int INTERFACE_TARGET_FPS = 60;

typedef struct {
    const char** names;
    int count;
} ListviewValues;

typedef struct {
    // Controls
    int listview_albums_scroll_index;
    int listview_albums_active;
    ListviewValues listview_albums_values;

    int listview_tracks_scroll_index;
    int listview_tracks_active;
    bool listview_tracks_selected;
    ListviewValues* listview_tracks_values; // Each album has a corresponding set of listview values

    bool button_shuffle_pressed;
    bool button_previous_pressed;
    bool button_next_pressed;
    bool button_play_pressed;
    bool button_stop_pressed;

    float sliderbar_volume_value;
    float sliderbar_progress_value;
    bool sliderbar_progress_selected;

    // Other stuff
    const char* album_name;
    const char* track_name;

    float time_played;
    float time_length;
} GuiLayoutState;

GuiLayoutState interfaceInit();

bool interfaceShouldUpdate();

void interfaceUpdate(GuiLayoutState* state);

void interfaceCleanUp();
