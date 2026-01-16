//
// Created by Francisco Javier on 15/01/2026.
//

#include "tracks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"
#include "raylib.h"
#include "tag_c.h"

// Utils
static char* copyString(const char* source);

// Track
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

    const TagLib_Tag* tag = taglib_file_tag(file);
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

// Tracklist
TrackList getTrackListFromDirectory(const char* path) {
    constexpr char VALID_FORMATS[] = ".mp3;.wav"; // TODO: Add ALL the valid formats, between taglib and raylib.

    const FilePathList music_files = LoadDirectoryFilesEx(path, VALID_FORMATS, false);
    TrackList tracks = {
        .items = nullptr,
        .count = 0,
        .capacity = 0,
    };
    for (size_t file_path_index = 0; file_path_index < music_files.count; file_path_index++) {
        const char* file_path = music_files.paths[file_path_index];

        Track* track = getTrackWithMetadataFromFile(file_path);
        if (track == nullptr) continue;

        DYNAMIC_ARRAY_APPEND(tracks, track);
    }
    UnloadDirectoryFiles(music_files);

    return tracks;
}

void freeTrackList(const TrackList* tracks) {
    for (size_t i = 0; i < tracks->count; i++) {
        freeTrack(tracks->items[i]);
    }
    free(tracks->items);
}

// Album
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

void freeAlbum(const Album* album, const bool free_tracks) {
    free(album->name);
    if (free_tracks) freeTrackList(&album->tracks);
}

void freeAlbumList(const AlbumList* albums, const bool free_tracks) {
    for (size_t album_index = 0; album_index < albums->count; album_index++) {
        freeAlbum(&albums->items[album_index], free_tracks);
    }
    free(albums->items);
}

// Utils
static char* copyString(const char* source) {
    char* str = malloc((strlen(source) + 1) * sizeof(char));
    strcpy(str, source);
    return str;
}