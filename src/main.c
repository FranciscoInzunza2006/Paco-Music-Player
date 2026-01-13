#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "tag_c.h"

struct PMP_Album_;

typedef struct {
    char *file_path;

    char *title;
    char *artist;
    unsigned int track_number;

    struct PMP_Album_ *album;
} PMP_Music;

struct PMP_Album_ {
    char *name;

    PMP_Music *tracks;
    size_t count;
    size_t capacity;
};

typedef struct PMP_Album_ PMP_Album;

bool getTrackMetadata(PMP_Music *track, const char *path) {
    bool success = true;

    TagLib_File *file = taglib_file_new(path);
    if (file == nullptr) {
        printf("Could not open file %s\n", path);
        success = false;
        goto cleanup;
    }

    TagLib_Tag *tag = taglib_file_tag(file);
    if (tag == nullptr) {
        printf("Could not open tag %s\n", path);
        success = false;
        goto cleanup;
    }

    // if (album.name == nullptr) {
    //     char *album_name = taglib_tag_album(tag);
    //     album.name = malloc((strlen(album_name) + 1) * sizeof(char));
    //     strcpy(album.name, album_name);
    // }

    track->file_path = malloc((strlen(path) + 1) * sizeof(char));
    strcpy(track->file_path, path);

    char *title = taglib_tag_title(tag);
    track->title = malloc((strlen(title) + 1) * sizeof(char));
    strcpy(track->title, title);

    char *artist = taglib_tag_artist(tag);
    track->artist = malloc((strlen(artist) + 1) * sizeof(char));
    strcpy(track->artist, artist);

    track->track_number = taglib_tag_track(tag);
    //track->album = &album;

cleanup:
    taglib_file_free(file);
    taglib_tag_free_strings();

    return success;
}

int main(void) {
    PMP_Album album = {
        .name = nullptr,
        .tracks = nullptr,
        .count = 0,
        .capacity = 0,
    };

    constexpr char music_directory_path[] = "D:\\Musica\\OFF";
    FilePathList music_files = LoadDirectoryFiles(music_directory_path);

    for (size_t i = 0; i < music_files.count; i++) {
        const char *file_path = music_files.paths[i];

        //printf("%llu = %s\n", i, file_path);

        if (album.count >= album.capacity) {
            if (album.capacity == 0) album.capacity = 4;
            else album.capacity *= 2;

            album.tracks = realloc(album.tracks, album.capacity * sizeof(PMP_Music));
        }

        PMP_Music *track = &album.tracks[album.count];
        bool result = getTrackMetadata(track, file_path);
        if (result) album.count++;
    }

    UnloadDirectoryFiles(music_files);

    for (size_t i = 0; i < album.count; i++) {
        printf("Path: \"%s\"\n"
               "Title: %s\n"
               "Artist: %s\n"
               "Track number: %u\n\n",
               album.tracks[i].file_path, album.tracks[i].title, album.tracks[i].artist, album.tracks[i].track_number);
    }

    return 0;
}

// TagLib_File *file = taglib_file_new(file_path);
// if (file == nullptr) {
//     printf("Could not open file %s\n", file_path);
//     continue;
// }
// TagLib_Tag *tag = taglib_file_tag(file);
//
// if (tag != NULL) {
//     if (album.name == nullptr) {
//         char *album_name = taglib_tag_album(tag);
//         album.name = malloc((strlen(album_name) + 1) * sizeof(char));
//         strcpy(album.name, album_name);
//     }
//
//     if (album.count >= album.capacity) {
//         if (album.capacity == 0) album.capacity = 4;
//         else album.capacity *= 2;
//
//         album.tracks = realloc(album.tracks, album.capacity * sizeof(PMP_Music));
//     }
//
//     PMP_Music *track = &album.tracks[i];
//
//     track->file_path = malloc((strlen(file_path) + 1) * sizeof(char));
//     strcpy(track->file_path, file_path);
//
//     char *title = taglib_tag_title(tag);
//     track->title = malloc((strlen(title) + 1) * sizeof(char));
//     strcpy(track->title, title);
//
//     char *artist = taglib_tag_artist(tag);
//     track->artist = malloc((strlen(artist) + 1) * sizeof(char));
//     strcpy(track->artist, artist);
//
//     track->track_number = taglib_tag_track(tag);
//     track->album = &album;
//
//
//     // printf("title   - \"%s\"\n", taglib_tag_title(tag));
//     // printf("artist  - \"%s\"\n", taglib_tag_artist(tag));
//     // printf("album   - \"%s\"\n", taglib_tag_album(tag));
//     // printf("comment - \"%s\"\n", taglib_tag_comment(tag));
//     // printf("track   - \"%u\"\n", taglib_tag_track(tag));
//     // printf("genre   - \"%s\"\n", taglib_tag_genre(tag));
// }

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
