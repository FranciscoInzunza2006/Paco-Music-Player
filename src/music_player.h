
#pragma once
#include <stddef.h>

#include "raylib.h"
#include "tracks.h"

extern AlbumList albums;
extern size_t current_album_index;
extern size_t current_track_index;

extern Music music;
extern bool is_music_playing;
extern float volume;

bool musicPlayer_loadMusic(const char* path);
void musicPlayer_updateMusic();

void musicPlayer_toggleMusicPlaying();
bool musicPlayer_isMusicPlaying();
bool musicPlayer_isMusicPaused();

void musicPlayer_setMusicPosition(float position);

float musicPlayer_getTimePlayed();
float musicPlayer_getTimeLength();

void musicPlayer_setVolume(float new_volume);
float musicPlayer_getVolume();

void loadMusic (size_t index);