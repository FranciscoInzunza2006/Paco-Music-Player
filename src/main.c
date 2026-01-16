#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tracks.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "music_player.h"
#include "raygui.h"

TrackList getTracksFromPaths();

// Media Player
Album* current_album = nullptr;
Track* playing_track = nullptr;
size_t playing_track_index = 0;

static void ButtonShuffle() {
}

static void loadMusicIndex(const TrackList* files, const size_t index) {
    musicPlayer_loadMusic(files->items[index].file_path);
}

static void ButtonPrevious() {
    if (playing_track_index > 0)
        playing_track_index--;
    loadMusicIndex(&current_album->tracks, playing_track_index);
}

static void ButtonNext() {
    if (playing_track_index < current_album->tracks.count - 1)
        playing_track_index++;
    loadMusicIndex(&current_album->tracks, playing_track_index);
}

int main(void) {
    TrackList all_tracks = getTracksFromPaths();
    AlbumList albums = organizeTracksIntoAlbums(&all_tracks);

    current_album = &albums.items[0];

    int listview_albumsScrollIndex = 0;
    int listview_albumsActive = 0;
    int listview_album_tracksScrollIndex = 0;
    int listview_album_tracksActive = 0;
    float sliderbar_volumeValue = 0.3f;
    float sliderbar_progressValue = 0.0f;

    InitWindow(800, 450, "Paco's Music Player");
    InitAudioDevice();
    loadMusicIndex(&current_album->tracks, playing_track_index);

    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(60);

    GuiSetIconScale(1);
    while (!WindowShouldClose()) {
        // Update
        musicPlayer_updateMusic();

        // Draw
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // Album
        GuiDummyRec((Rectangle){24, 24, 256, 256}, nullptr);
        GuiLabel((Rectangle){24, 280, 256, 32}, "Album name");

        // Album selection
        GuiListView((Rectangle){24, 312, 256, 120}, "ONE;TWO;THREE", &listview_albumsScrollIndex,
                    &listview_albumsActive);

        // Album track selection
        GuiListView((Rectangle){296, 24, 480, 280}, "ONE;TWO;THREE;", &listview_album_tracksScrollIndex,
                    &listview_album_tracksActive);

        // Controls
        if (GuiButton((Rectangle){320, 320, 32, 32}, "#077#")) ButtonShuffle();

        const char* toggle_button_icon = GuiIconText(
            musicPlayer_isMusicPlaying() ? ICON_PLAYER_PLAY : ICON_PLAYER_PAUSE, nullptr);
        if (GuiButton((Rectangle){424, 320, 32, 32}, toggle_button_icon))
            musicPlayer_toggleMusicPlaying();

        if (GuiButton((Rectangle){376, 320, 32, 32}, "#129#")) ButtonPrevious();
        if (GuiButton((Rectangle){472, 320, 32, 32}, "#134#")) ButtonNext();

        // Volume bar
        //GuiDrawIcon(ICON_FILETYPE_AUDIO,520, 320, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        GuiSliderBar((Rectangle){552, 328, 224, 16}, nullptr, nullptr, &sliderbar_volumeValue, 0, 1);
        if (sliderbar_volumeValue != musicPlayer_getVolume()) {
            musicPlayer_setVolume(sliderbar_volumeValue);
        }

        // Time played bar
        const float time_played = musicPlayer_getTimePlayed();
        const float time_length = musicPlayer_getTimeLength();
        const char* time_string = TextFormat("%02d:%02d", (int) (time_played / 60.0f), (int) time_played % 60);
        GuiLabel((Rectangle){320, 368, 64, 32}, time_string);

        const float progress = time_played / time_length;
        sliderbar_progressValue = progress;
        GuiSliderBar((Rectangle){392, 376, 384, 16}, nullptr, nullptr, &sliderbar_progressValue, 0, 1);
        if (fabsf(progress - sliderbar_progressValue) > 0.01f) {
            musicPlayer_setMusicPosition(sliderbar_progressValue);
        }

        // Track name
        const char* track_name = current_album->tracks.items[playing_track_index].title;
        GuiLabel((Rectangle){392, 400, 384, 32}, GuiIconText(ICON_FILETYPE_AUDIO, track_name));

        EndDrawing();
    }

    CloseWindow();
    //freeTrackList(&all_tracks);
    //freeAlbumList(&albums, false);

    return 0;
}

TrackList getTracksFromPaths() {
    const char* MUSIC_DIRECTORIES_PATH[] = {
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\Terraria Official Soundtrack",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot Solstice OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\UNDERTALE Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\DELTARUNESoundtrack",
        //"D:\\Musica\\OFF",
        //"D:\\Musica\\Ribbit",
    };
    //constexpr size_t MUSIC_DIRECTORIES_COUNT = _countof(MUSIC_DIRECTORIES_PATH);


    const char* music_directory_path = MUSIC_DIRECTORIES_PATH[0];
    return getTrackListFromDirectory(music_directory_path);

    // for (size_t directory_index = 0; directory_index < MUSIC_DIRECTORIES_COUNT; directory_index++) {
    //     const char* music_directory_path = MUSIC_DIRECTORIES_PATH[directory_index];
    //    return getTrackListFromDirectory(music_directory_path);
    //
    // }

    //return tracks;
}
