//
// Created by Francisco Javier on 16/01/2026.
//

#pragma once

constexpr int INTERFACE_WINDOW_WIDTH = 800;
constexpr int INTERFACE_WINDOW_HEIGHT = 800;
constexpr char INTERFACE_WINDOW_DEFAULT_TITLE[] = "Paco's Music Player";
constexpr int INTERFACE_TARGET_FPS = 60;

typedef struct {
    int listview_albumsScrollIndex;
    int listview_albumsActive;
    int listview_album_tracksScrollIndex;
    int listview_album_tracksActive;

    bool button_shuffle_pressed;
    bool button_previous_pressed;
    bool button_next_pressed;
    bool button_play_pressed;
    bool button_stop_pressed;

    float sliderbar_volumeValue;
    float sliderbar_progressValue;
} GuiLayoutState;

GuiLayoutState interfaceInit();

bool interfaceShouldUpdate();

void interfaceUpdate(GuiLayoutState* state);

void interfaceCleanUp();
