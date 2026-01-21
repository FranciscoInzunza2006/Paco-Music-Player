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
bool getTrackWithMetadataFromFile(const char* path, Track* output) {
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

    output->file_path = copyString(path);

    taglib_set_string_management_enabled(false);
    output->title = taglib_tag_title(tag);
    if (strcmp(output->title, "") == 0) output->title = copyString(GetFileNameWithoutExt(path));

    output->artist = taglib_tag_artist(tag);
    output->album = taglib_tag_album(tag);
    output->comment = taglib_tag_comment(tag);
    output->genre = taglib_tag_genre(tag);
    output->track_number = taglib_tag_track(tag);

cleanup:
    taglib_file_free(file);
    return success;
}

void freeTrack(const Track* track) {
    free(track->file_path);
    free(track->title);
    free(track->artist);
    free(track->album);
    free(track->comment);
    free(track->genre);
}

Tracks getTracksFromDirectory(const char* path) {
    Tracks tracks = {0};

    // Raylib compatible formats: WAV, OGG, MP3, FLAC, MOD, XM
    // Taglib compatible formats: ID3v1 and ID3v2 for MP3 files, Ogg Vorbis comments and ID3 tags in
    //                            FLAC, MPC, Speex, WavPack, TrueAudio, WAV, AIFF, MP4, APE, ASF, DSF, DFF and AAC
    constexpr char VALID_FORMATS[] = ".mp3;.wav;.ogg;.flac";
    const FilePathList music_files = LoadDirectoryFilesEx(path, VALID_FORMATS, false);
    for (size_t file_path_index = 0; file_path_index < music_files.count; file_path_index++) {
        const char* file_path = music_files.paths[file_path_index];

        Track track;
        const bool success = getTrackWithMetadataFromFile(file_path, &track);
        if (!success) continue;

        DYNAMIC_ARRAY_APPEND(tracks, track);
    }
    UnloadDirectoryFiles(music_files);

    return tracks;
}

// Playlist
Playlists organizeTracksIntoPlaylists(const Tracks* tracks) {
    Playlists albums = {0};

    for (size_t track_index = 0; track_index < tracks->count; track_index++) {
        Track* track = &tracks->items[track_index];

        for (size_t i = 0; i < albums.count; i++) {
            Playlist* album = &albums.items[i];
            if (strcmp(track->album, album->name) == 0) {
                DYNAMIC_ARRAY_APPEND(album->tracks, *track);
                goto next_track;
            }
        }

        Playlist new_album = {
            .name = copyString(track->album),
            .tracks = (Tracks){0}
        };
        DYNAMIC_ARRAY_APPEND(new_album.tracks, *track);
        DYNAMIC_ARRAY_APPEND(albums, new_album);
    next_track:;
    }
    return albums;
}

void freePlaylist(const Playlist* album) {
    free(album->name);
}

// Utils
static char* copyString(const char* source) {
    char* str = malloc((strlen(source) + 1) * sizeof(char));
    strcpy(str, source);
    return str;
}
