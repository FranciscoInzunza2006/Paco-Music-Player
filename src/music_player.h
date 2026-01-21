
#pragma once
#include <stddef.h>

#include "tracks.h"

void musicPlayer_init(Playlists album_list);
void musicPlayer_update();
void musicPlayer_cleanup();

bool musicPlayer_play();
void musicPlayer_pause();
//void musicPlayer_toggle();

void musicPlayer_previousTrack();
void musicPlayer_nextTrack();
void musicPlayer_changePlayback();

void musicPlayer_setVolume(float new_volume);
void musicPlayer_setProgress(float progress);

void musicPlayer_changeTrack(size_t index);
void musicPlayer_changeAlbum(size_t index);

// Getters
const Playlists* musicPlayer_getAlbumList();
const Playlist* musicPlayer_getCurrentAlbum();
const Track* musicPlayer_getCurrentTrack();

bool musicPlayer_isPlaying();

float musicPlayer_getTimePlayed();
float musicPlayer_getTimeLength();

float musicPlayer_getVolume();
