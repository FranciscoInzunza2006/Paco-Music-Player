//
// Created by Francisco Javier on 15/01/2026.
//

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {

#endif


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
    Track* items;
    size_t count;
    size_t capacity;
} Tracks;

typedef struct {
    char* name;
    Tracks tracks;
} Album;

typedef struct {
    Album* items;
    size_t count;
    size_t capacity;
} Albums;

bool getTrackWithMetadataFromFile(const char* path, Track* output);

void freeTrack(const Track* track);

void freeAlbum(const Album* album);

Albums organizeTracksIntoAlbums(const Tracks* tracks);

Tracks getTracksFromDirectory(const char* path);

#ifdef __cplusplus
}
#endif
