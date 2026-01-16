#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "tag_c.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//region Dynamic array macro
constexpr size_t DYNAMIC_ARRAY_STARTING_CAPACITY = 4;
#define DYNAMIC_ARRAY_APPEND(array, item)\
    do {\
        if (array.count >= array.capacity) {\
            if (array.capacity == 0) array.capacity = DYNAMIC_ARRAY_STARTING_CAPACITY;\
            else array.capacity *= 2;\
        \
            array.items = realloc(array.items, array.capacity * sizeof(*array.items)); /* NOLINT(*-suspicious-realloc-usage) */ \
        }\
        array.items[array.count++] = item;\
    } while (false)
//endregion

typedef struct {
    char* file_path;

    // Metadata
    char* title;
    char* artist;
    char* album;
    char* comment;
    char* genre;
    unsigned int track_number;
} Track;

typedef struct {
    Track** items;
    size_t count;
    size_t capacity;
} TrackList;

typedef struct {
    char* name;
    TrackList tracks;
} Album;

typedef struct {
    Album* items;
    size_t count;
    size_t capacity;
} AlbumList;

static char* copyString(const char* source);

Track* getTrackWithMetadataFromFile(const char* path);

void freeTrack(const Track* track);

void freeTrackList(TrackList* tracks);

void freeAlbum(Album* album, bool free_tracks);

void freeAlbumList(AlbumList* albums, bool free_tracks);

TrackList getTracksFromPaths();

AlbumList organizeTracksIntoAlbums(const TrackList* tracks);


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
    SetTargetFPS(24);

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
    freeTrackList(&all_tracks);
    freeAlbumList(&albums, false);

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
    constexpr size_t MUSIC_DIRECTORIES_COUNT = _countof(MUSIC_DIRECTORIES_PATH);

    TrackList tracks = {
        .items = nullptr,
        .count = 0,
        .capacity = 0,
    };

    for (size_t directory_index = 0; directory_index < MUSIC_DIRECTORIES_COUNT; directory_index++) {
        const char* music_directory_path = MUSIC_DIRECTORIES_PATH[directory_index];

        constexpr char VALID_FORMATS[] = ".mp3;.wav";
        // TODO: Add ALL the valid formats, between taglib and raylib.
        const FilePathList music_files = LoadDirectoryFilesEx(music_directory_path, VALID_FORMATS, false);
        for (size_t file_path_index = 0; file_path_index < music_files.count; file_path_index++) {
            const char* file_path = music_files.paths[file_path_index];

            Track* track = getTrackWithMetadataFromFile(file_path);
            if (track == nullptr) continue;

            DYNAMIC_ARRAY_APPEND(tracks, track);
        }
        UnloadDirectoryFiles(music_files);
    }

    return tracks;
}

AlbumList organizeTracksIntoAlbums(const TrackList* tracks) {
    AlbumList albums = {nullptr, 0, 0};
    for (size_t track_index = 0; track_index < tracks->count; track_index++) {
        Track* track = tracks->items[track_index];

        for (size_t i = 0; i < albums.count; i++) {
            Album* album = &albums.items[i];
            if (strcmp(track->album, album->name) == 0) {
                DYNAMIC_ARRAY_APPEND(album->tracks, track); // NOLINT(*-suspicious-realloc-usage)
                goto next_track;
            }
        }

        Album new_album = {
            .name = copyString(track->album),
            .tracks = (TrackList){
                .items = nullptr,
                .count = 0,
                .capacity = 0,
            }
        };
        DYNAMIC_ARRAY_APPEND(new_album.tracks, track);
        DYNAMIC_ARRAY_APPEND(albums, new_album);
    next_track:;
    }
    return albums;
}

static char* copyString(const char* source) {
    char* str = malloc((strlen(source) + 1) * sizeof(char));
    strcpy(str, source);
    return str;
}

Track* getTrackWithMetadataFromFile(const char* path) {
    Track* track = malloc(sizeof(Track));
    if (track == nullptr) {
        return nullptr;
    }

    bool success = true;
    TagLib_File* file = taglib_file_new(path);
    if (file == nullptr) {
        printf("Could not open file %s\n", path);
        success = false;
        goto cleanup;
    }

    TagLib_Tag* tag = taglib_file_tag(file);
    if (tag == nullptr) {
        printf("Could not open tag %s\n", path);
        success = false;
        goto cleanup;
    }

    track->file_path = copyString(path);

    track->title = copyString(taglib_tag_title(tag));
    track->artist = copyString(taglib_tag_artist(tag));
    track->album = copyString(taglib_tag_album(tag));
    track->comment = copyString(taglib_tag_comment(tag));
    track->genre = copyString(taglib_tag_genre(tag));
    track->track_number = taglib_tag_track(tag);

    if (strcmp(track->title, "") == 0) {
        track->title = copyString(GetFileNameWithoutExt(path));
    }

cleanup:
    taglib_file_free(file);
    taglib_tag_free_strings();
    if (success == false) {
        free(track);
        return nullptr;
    }

    return track;
}

void freeTrack(const Track* track) {
    free(track->file_path);
    free(track->title);
    free(track->artist);
    free(track->album);
    free(track->comment);
    free(track->genre);
}

void freeTrackList(TrackList* tracks) {
    for (size_t i = 0; i < tracks->count; i++) {
        freeTrack(tracks->items[i]);
    }
    free(tracks->items);
}

void freeAlbum(Album* album, bool free_tracks) {
    free(album->name);
    if (free_tracks) freeTrackList(&album->tracks);
}

void freeAlbumList(AlbumList* albums, bool free_tracks) {
    for (size_t album_index = 0; album_index < albums->count; album_index++) {
        freeAlbum(&albums->items[album_index], free_tracks);
    }
    free(albums->items);
}
