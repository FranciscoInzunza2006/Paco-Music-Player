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
    Album album = {
        .name = nullptr,
        .tracks = nullptr,
        .count = 0,
        .capacity = 0,
    };

    constexpr char music_directory_path[] = "D:\\Musica\\OFF";
    FilePathList music_files = LoadDirectoryFiles(music_directory_path);

    for (size_t i = 0; i < music_files.count; i++) {
        const char* file_path = music_files.paths[i];

        //printf("%llu = %s\n", i, file_path);

        if (album.count >= album.capacity) {
            if (album.capacity == 0) album.capacity = 4;
            else album.capacity *= 2;

            album.tracks = realloc(album.tracks, album.capacity * sizeof(Track*));
        }

        Track* track = getTrackWithMetadataFromFile(file_path);
        if (track != nullptr) {
            album.tracks[album.count] = track;
            album.count++;
        }
    }

    UnloadDirectoryFiles(music_files);

    for (size_t i = 0; i < album.count; i++) {
        printf("Path: \"%s\"\n"
               "Title: %s\n"
               "Artist: %s\n"
               "Track number: %u\n\n",
               album.tracks[i]->file_path, album.tracks[i]->title, album.tracks[i]->artist, album.tracks[i]->track_number);
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
