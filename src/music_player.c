#include "music_player.h"

#include <stddef.h>

#include "raylib.h"
#include "tracks.h"

//Album* current_album = nullptr;
//Track* playing_track = nullptr;
//size_t playing_track_index = 0;

// Music stream handling
Music music = {0};
bool is_music_playing = false;
float volume = 1.0f;

static void stopMusic();

void changeMusic(const char* new_music_path);

// Music Player
AlbumList albums = {0};
size_t current_album_index = 0;
size_t current_track_index = 0;

void musicPlayer_init(AlbumList album_list) {
    albums = album_list;
    current_album_index = 0;
    current_track_index = 0;
    is_music_playing = false;
    stopMusic();
}

void musicPlayer_update() {
    UpdateMusicStream(music);
}

void musicPlayer_cleanup() {
    stopMusic();
    freeAlbumList(&albums, true);
}

// Setters
void musicPlayer_changeAlbum(const size_t index) {
    current_album_index = index;
    current_track_index = -1;

    stopMusic();
}

void musicPlayer_changeTrack(const size_t index) {
    current_track_index = index;
    changeMusic(musicPlayer_getCurrentTrack()->file_path);
}

void musicPlayer_setProgress(float progress) {
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    SeekMusicStream(music, progress * musicPlayer_getTimeLength());
}

void musicPlayer_toggleMusicPlaying() {
    is_music_playing = !is_music_playing;
    if (is_music_playing) ResumeMusicStream(music);
    else PauseMusicStream(music);
}

void musicPlayer_setVolume(const float new_volume) {
    volume = new_volume;
    SetMusicVolume(music, volume);
}

// Getters
const Album* musicPlayer_getCurrentAlbum() {
    return &albums.items[current_album_index];
}

const Track* musicPlayer_getCurrentTrack() {
    return &musicPlayer_getCurrentAlbum()->tracks.items[current_track_index];
}

bool musicPlayer_isPlaying() {
    return is_music_playing;
}

bool musicPlayer_isPaused() {
    return !is_music_playing;
}

float musicPlayer_getTimePlayed() {
    return GetMusicTimePlayed(music);
}

float musicPlayer_getTimeLength() {
    return GetMusicTimeLength(music);
}

float musicPlayer_getVolume() { return volume; }

// Music stream stuff
static void stopMusic() {
    UnloadMusicStream(music);
    music = (Music){0};
    is_music_playing = false;
}

void changeMusic(const char* new_music_path) {
    UnloadMusicStream(music);

    music = LoadMusicStream(new_music_path);
    if (!IsMusicValid(music)) {
        is_music_playing = false;
        return;
    }

    PlayMusicStream(music);
    SetMusicVolume(music, volume);

    is_music_playing = true;
}
