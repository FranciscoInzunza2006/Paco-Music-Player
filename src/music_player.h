
#pragma once
#include <stddef.h>

#include "raylib.h"
#include "tracks.h"

// extern AlbumList albums;
// extern size_t current_album_index;
// extern size_t current_track_index;
//
// extern Music music;
// extern bool is_music_playing;
// extern float volume;

// bool musicPlayer_loadMusicFromPath(const char* path);
// void musicPlayer_updateMusic();
//
// void musicPlayer_toggleMusicPlaying();
// bool musicPlayer_isMusicPlaying();
// bool musicPlayer_isMusicPaused();w

void musicPlayer_init(AlbumList album_list);
void musicPlayer_update();
void musicPlayer_cleanup();

void musicPlayer_changeAlbum(size_t index);
void musicPlayer_changeTrack(size_t index);

const Album* musicPlayer_getCurrentAlbum();
const Track* musicPlayer_getCurrentTrack();

void musicPlayer_toggleMusicPlaying();
bool musicPlayer_isPlaying();
bool musicPlayer_isPaused();

void musicPlayer_setProgress(float progress);
float musicPlayer_getTimePlayed();
float musicPlayer_getTimeLength();

void musicPlayer_setVolume(float new_volume);
float musicPlayer_getVolume();
