#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "tag_c.h"

struct Album_;

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

struct Album_ {
    char* name;

    Track** tracks;
    size_t count;
    size_t capacity;
};

typedef struct Album_ Album;

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

cleanup:
    taglib_file_free(file);
    taglib_tag_free_strings();
    if (success == false) {
        free(track);
        return nullptr;
    }

    return track;
}

int main(void) {
    struct TrackList {
        Track** tracks;
        size_t count;
        size_t capacity;
    };

    struct TrackList all_tracks = {
        .tracks = nullptr,
        .count = 0,
        .capacity = 0,
    };

    const char* MUSIC_DIRECTORIES_PATH[] = {
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\Terraria Official Soundtrack",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot OST",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot Solstice OST",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\UNDERTALE Soundtrack",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\DELTARUNESoundtrack",
        "D:\\Musica\\OFF",
    };
    constexpr size_t MUSIC_DIRECTORIES_COUNT = _countof(MUSIC_DIRECTORIES_PATH);

    for (size_t directory_index = 0; directory_index < MUSIC_DIRECTORIES_COUNT; directory_index++) {
        const char* music_directory_path = MUSIC_DIRECTORIES_PATH[directory_index];

        FilePathList music_files = LoadDirectoryFiles(music_directory_path);
        for (size_t file_path_index = 0; file_path_index < music_files.count; file_path_index++) {
            const char* file_path = music_files.paths[file_path_index];

            Track* track = getTrackWithMetadataFromFile(file_path);
            if (track == nullptr) continue;

            if (all_tracks.count >= all_tracks.capacity) {
                if (all_tracks.capacity == 0) all_tracks.capacity = 4;
                else all_tracks.capacity *= 2;

                Track** new_buffer = realloc(all_tracks.tracks, all_tracks.capacity * sizeof(Track*));
                if (new_buffer == nullptr) {
                    printf("Could not reallocate memory for new tracks\n");
                    return -1;
                }
                all_tracks.tracks = new_buffer;
            }
            all_tracks.tracks[all_tracks.count++] = track;
        }
        UnloadDirectoryFiles(music_files);
    }

    for (size_t i = 0; i < all_tracks.count; i++) {
        printf("Path: \"%s\"\n"
               "Title: %s\n"
               "Artist: %s\n"
               "Track number: %u\n"
               "Album: %s\n\n",
               all_tracks.tracks[i]->file_path, all_tracks.tracks[i]->title, all_tracks.tracks[i]->artist,
               all_tracks.tracks[i]->track_number, all_tracks.tracks[i]->album);
    }

    return 0;
}

// UnloadDirectoryFiles(music_files);
// taglib_tag_free_strings();
//
// InitWindow(500, 500, "Paco's Music Player");
// SetTargetFPS(24);
// while (!WindowShouldClose()) {
//     BeginDrawing();
//     ClearBackground(RAYWHITE);
//
//     for (size_t i = 0; i < album.count; i++) {
//         PMP_Music *track = &album.tracks[i];
//
//         DrawText(TextFormat("%u - %s", track->track_number, track->title),
//                  20, 20 + 20 * (int) i, 16, BLACK);
//     }
//
//     EndDrawing();
// }
