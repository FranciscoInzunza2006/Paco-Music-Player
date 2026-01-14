#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "tag_c.h"

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

TrackList getTracksFromPaths() {
    const char* MUSIC_DIRECTORIES_PATH[] = {
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\Terraria Official Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot Solstice OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\UNDERTALE Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\DELTARUNESoundtrack",
        //"D:\\Musica\\OFF",
        "D:\\Musica\\Ribbit",
    };
    constexpr size_t MUSIC_DIRECTORIES_COUNT = _countof(MUSIC_DIRECTORIES_PATH);

    TrackList tracks = {
        .items = nullptr,
        .count = 0,
        .capacity = 0,
    };

    for (size_t directory_index = 0; directory_index < MUSIC_DIRECTORIES_COUNT; directory_index++) {
        const char* music_directory_path = MUSIC_DIRECTORIES_PATH[directory_index];

        constexpr char VALID_FORMATS[] = ".mp3;.wav;.flac";
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

int main(void) {
    TrackList all_tracks = getTracksFromPaths();

    // for (size_t i = 0; i < all_tracks.count; i++) {
    //     const Track* track = all_tracks.items[i];
    //     printf("Path: \"%s\"\n"
    //            "Title: %s\n"
    //            "Artist: %s\n"
    //            "Track number: %u\n"
    //            "Album: %s\n\n",
    //            track->file_path, track->title, track->artist, track->track_number, track->album);
    // }

    // Separate all the tracks into different albums
    struct AlbumList {
        Album* items;
        size_t count;
        size_t capacity;
    };
    struct AlbumList albums = {nullptr, 0, 0};
    for (size_t track_index = 0; track_index < all_tracks.count; track_index++) {
        Track* track = all_tracks.items[track_index];

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
    next_track:


    }

    for (size_t album_index = 0; album_index < albums.count; album_index++) {
        const Album* album = &albums.items[album_index];
        const TrackList* tracks = &album->tracks;
        printf("Album: \"%s\"\n"
               "Number of tracks: %llu\n",
               album->name, tracks->count);

        for (size_t track_index = 0; track_index < tracks->count; track_index++) {
            const Track* track = tracks->items[track_index];
            printf("\t%u - \"%s\"\n", track->track_number, track->title);
        }
        printf("\n");
    }

    freeTrackList(&all_tracks);

    return 0;
}
