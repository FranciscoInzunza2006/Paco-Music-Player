#include "music_player.h"

#include <stddef.h>

#include "raylib.h"
#include "tracks.h"

//Album* current_album = nullptr;
//Track* playing_track = nullptr;
//size_t playing_track_index = 0;

// Music Stuff
Music music = {0};
bool is_music_playing = false;
float volume = 1.0f;
bool musicPlayer_loadMusic(const char* path) {
    UnloadMusicStream(music);

    music = LoadMusicStream(path);
    PlayMusicStream(music);
    SetMusicVolume(music, volume);

    is_music_playing = true;
    return true;
}

void musicPlayer_updateMusic() {
    UpdateMusicStream(music);
}

void musicPlayer_toggleMusicPlaying() {
    is_music_playing = !is_music_playing;
    if (is_music_playing) ResumeMusicStream(music);
    else PauseMusicStream(music);
}

bool musicPlayer_isMusicPlaying() {
    return is_music_playing;
}

bool musicPlayer_isMusicPaused() {
    return !is_music_playing;
}

void musicPlayer_setMusicPosition(float position) {
    if (position < 0.0f) position = 0.0f;
    if (position > 1.0f) position = 1.0f;

    SeekMusicStream(music, position * musicPlayer_getTimeLength());
}

float musicPlayer_getTimePlayed() {
    return GetMusicTimePlayed(music);
}
float musicPlayer_getTimeLength() {
    return GetMusicTimeLength(music);
}

void musicPlayer_setVolume(const float new_volume) {
    volume = new_volume;
    SetMusicVolume(music, volume);
}

float musicPlayer_getVolume() { return volume; }

// Album and tracks stuff
AlbumList albums = {0};
size_t current_album_index = 0;
size_t current_track_index = 0;
void loadMusic(const size_t index) {
    const Album* current_album = &albums.items[current_album_index];
    Track* current_track = &current_album->tracks.items[current_track_index];
    musicPlayer_loadMusic(current_track->file_path);
}