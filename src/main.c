#include <stdio.h>
#include <stdlib.h>

#include "tracks.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

TrackList getTracksFromPaths();

// Media Player
Music music;
Album* current_album = nullptr;
Track* playing_track = nullptr;
size_t playing_track_index = 0;
float volume = 1.0f;
bool is_music_paused = false;

static void ButtonShuffle() {
}

static void ButtonPlay() {
    is_music_paused = !is_music_paused;
    if (is_music_paused) PauseMusicStream(music);
    else ResumeMusicStream(music);
}

static void loadMusicIndex(const TrackList* files, const size_t index) {
    UnloadMusicStream(music);
    music = LoadMusicStream(files->items[index]->file_path);
    PlayMusicStream(music);

    SetMusicVolume(music, volume);
    is_music_paused = false;
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
    float sliderbar_volumeValue = 0.0f;
    float sliderbar_progressValue = 0.0f;

    InitWindow(800, 450, "Paco's Music Player");
    InitAudioDevice();
    loadMusicIndex(&current_album->tracks, playing_track_index);

    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(60);

    GuiSetIconScale(1);
    while (!WindowShouldClose()) {
        // Update
        // TODO: Implement required update logic
        UpdateMusicStream(music);

        // Draw
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        GuiDummyRec((Rectangle){24, 24, 256, 256}, nullptr);
        GuiLabel((Rectangle){24, 280, 256, 32}, "Album name");
        GuiListView((Rectangle){24, 312, 256, 120}, "ONE;TWO;THREE", &listview_albumsScrollIndex,
                    &listview_albumsActive);
        GuiListView((Rectangle){296, 24, 480, 280}, "ONE;TWO;THREE;", &listview_album_tracksScrollIndex,
                    &listview_album_tracksActive);

        if (GuiButton((Rectangle){320, 320, 32, 32}, "#077#")) ButtonShuffle();
        if (GuiButton((Rectangle){376, 320, 32, 32}, "#129#")) ButtonPrevious();

        if (GuiButton((Rectangle){424, 320, 32, 32},
                      GuiIconText(is_music_paused ? ICON_PLAYER_PLAY : ICON_PLAYER_PAUSE, nullptr)))
            ButtonPlay();

        if (GuiButton((Rectangle){472, 320, 32, 32}, "#134#")) ButtonNext();
        //GuiDrawIcon(ICON_FILETYPE_AUDIO,520, 320, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        GuiSliderBar((Rectangle){552, 328, 224, 16}, nullptr, nullptr, &sliderbar_volumeValue, 0, 1);

        const float time_played = GetMusicTimePlayed(music);
        const float time_length = GetMusicTimeLength(music);

        GuiLabel((Rectangle){320, 368, 64, 32},
                 TextFormat("%02d:%02d", (int) (time_played / 60.0f), (int) time_played % 60));
        sliderbar_progressValue = time_played / time_length;
        GuiSliderBar((Rectangle){392, 376, 384, 16}, nullptr, nullptr, &sliderbar_progressValue, 0, 1);

        GuiLabel((Rectangle){392, 400, 384, 32},
                 GuiIconText(ICON_FILETYPE_AUDIO, current_album->tracks.items[playing_track_index]->title)
        );

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

