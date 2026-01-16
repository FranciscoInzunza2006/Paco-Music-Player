//
// Created by Francisco Javier on 15/01/2026.
//

#pragma once

#include <stddef.h>

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

bool getTrackWithMetadataFromFile(const char* path, Track* output);

void freeTrack(const Track* track);

void freeTrackList(const TrackList* tracks);

void freeAlbum(const Album* album, bool free_tracks);

void freeAlbumList(const AlbumList* albums, bool free_tracks);

AlbumList organizeTracksIntoAlbums(const TrackList* tracks);

TrackList getTrackListFromDirectory(const char* path);