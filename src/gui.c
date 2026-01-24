#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "music_player.h"
#include "raygui.h"
#include "styles/jungle/style_jungle.h"

static GuiPacosState initState();
static void styleGui();

static void portableWindow(GuiPacosState* state);

static void ButtonShuffle();
static void ButtonPrevious();
static void ButtonPlay();
static void ButtonNext();

static const char* formatToTime(float time_in_seconds);

GuiPacosState guiInit() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_ALWAYS_RUN);

    InitWindow(INTERFACE_WINDOW_WIDTH, INTERFACE_WINDOW_HEIGHT, INTERFACE_WINDOW_DEFAULT_TITLE);
    InitAudioDevice();

    SetTargetFPS(INTERFACE_TARGET_FPS);

    styleGui();

    return initState();
}

bool guiShouldUpdate(const GuiPacosState* state) {
    return !WindowShouldClose() && state->windowbox_mainActive;
}

void guiUpdate(GuiPacosState* state) {
    portableWindow(state);

    if (!state->windowbox_mainActive) return;
    BeginDrawing();
    const Album* album = musicPlayer_getCurrentAlbum();
    const Track* track = musicPlayer_getCurrentTrack();

    // Windowbox
    state->windowbox_mainActive = !GuiWindowBox(state->layoutRecs[0], "#124#Paco's Music Player");

    GuiDummyRec(state->layoutRecs[1], "(Track Picture)");
    GuiLabel(state->layoutRecs[2], album->name);

    // Album listview
    GuiListView(state->layoutRecs[3], "ONE;TWO;THREE", &state->listview_albumsScrollIndex, &state->listview_albumsActive);

    // Tracks listview
    GuiListView(state->layoutRecs[4], "ONE;TWO;THREE;", &state->listview_album_tracksScrollIndex,
                &state->listview_album_tracksActive);

    // Controls buttons
    if (GuiButton(state->layoutRecs[5], "#077#")) ButtonShuffle();
    if (GuiButton(state->layoutRecs[6], "#129#")) ButtonPrevious();
    if (GuiButton(state->layoutRecs[7], "#131#")) ButtonPlay();
    if (GuiButton(state->layoutRecs[8], "#134#")) ButtonNext();

    // Volume sliderbar
    GuiSliderBar(state->layoutRecs[9], "#122#", nullptr, &state->sliderbar_volumeValue, 0, 1);
    if (state->sliderbar_volumeValue != musicPlayer_getVolume()) musicPlayer_setVolume(state->sliderbar_volumeValue);

    // Progress sliderbar
    GuiSliderBar(state->layoutRecs[10],
                 formatToTime(musicPlayer_getTimePlayed()), formatToTime(musicPlayer_getTimeLength()),
                 &state->sliderbar_progressValue, 0, 1);

    // Trackname label
    GuiLabel(state->layoutRecs[12], GuiIconText(ICON_FILETYPE_AUDIO, track->title));

    EndDrawing();
}

void guiCleanUp() {
}

GuiPacosState initState() {
    GuiPacosState state = {0};

    state.anchor_window_contents = (Vector2){0, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};

    state.windowbox_mainActive = true;
    state.windowPosition = GetWindowPosition();

    // state.listview_albumsScrollIndex = 0;
    // state.listview_albumsActive = 0;
    // state.listview_album_tracksScrollIndex = 0;
    // state.listview_album_tracksActive = 0;
    // state.sliderbar_volumeValue = 0.0f;
    // state.sliderbar_progressValue = 0.0f;

    state.layoutRecs[0] = (Rectangle){0, 0, INTERFACE_WINDOW_WIDTH, INTERFACE_WINDOW_HEIGHT};
    state.layoutRecs[1] = (Rectangle){state.anchor_window_contents.x + 24, state.anchor_window_contents.y + 24, 256, 256};
    state.layoutRecs[2] = (Rectangle){state.anchor_window_contents.x + 24, state.anchor_window_contents.y + 280, 256, 32};
    state.layoutRecs[3] = (Rectangle){state.anchor_window_contents.x + 24, state.anchor_window_contents.y + 312, 256, 120};
    state.layoutRecs[4] = (Rectangle){state.anchor_window_contents.x + 296, state.anchor_window_contents.y + 24, 480, 280};
    state.layoutRecs[5] = (Rectangle){state.anchor_window_contents.x + 320, state.anchor_window_contents.y + 320, 32, 32};
    state.layoutRecs[6] = (Rectangle){state.anchor_window_contents.x + 376, state.anchor_window_contents.y + 320, 32, 32};
    state.layoutRecs[7] = (Rectangle){state.anchor_window_contents.x + 424, state.anchor_window_contents.y + 320, 32, 32};
    state.layoutRecs[8] = (Rectangle){state.anchor_window_contents.x + 472, state.anchor_window_contents.y + 320, 32, 32};
    state.layoutRecs[9] = (Rectangle){state.anchor_window_contents.x + 552, state.anchor_window_contents.y + 328, 224, 16};
    state.layoutRecs[10] = (Rectangle){state.anchor_window_contents.x + 352, state.anchor_window_contents.y + 376, 384, 16};
    state.layoutRecs[11] = (Rectangle){10, 10, 10, 10};
    state.layoutRecs[12] = (Rectangle){state.anchor_window_contents.x + 392, state.anchor_window_contents.y + 400, 384, 32};

    return state;
}

void styleGui() {
    GuiLoadStyleJungle();
    GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_SPACING, 0);
    //GuiSetStyle(LISTVIEW, TEXT_PADDING, 4);
}

void portableWindow(GuiPacosState* state) {
    const Vector2 mousePosition = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !state->dragWindow) {
        if (CheckCollisionPointRec(mousePosition, (Rectangle){0, 0, INTERFACE_WINDOW_WIDTH, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT})) {
            state->windowPosition = GetWindowPosition();
            state->dragWindow = true;
            state->panOffset = mousePosition;
        }
    }

    if (state->dragWindow) {
        state->windowPosition.x += (mousePosition.x - state->panOffset.x);
        state->windowPosition.y += (mousePosition.y - state->panOffset.y);

        SetWindowPosition((int) state->windowPosition.x, (int) state->windowPosition.y);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) state->dragWindow = false;
    }
}

static void ButtonShuffle() {
    // TODO: Implement control logic
}

static void ButtonPrevious() {
    musicPlayer_previousTrack();
}

static void ButtonPlay() {
    if (musicPlayer_isPlaying()) musicPlayer_pause();
    else musicPlayer_play();
}

static void ButtonNext() {
    musicPlayer_nextTrack();
}

static const char* formatToTime(const float time_in_seconds) {
    return TextFormat("%02d:%02d", (int) (time_in_seconds / 60.0f), (int) time_in_seconds % 60);
}
