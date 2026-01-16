
#pragma once

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