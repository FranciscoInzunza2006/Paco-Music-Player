#include "music_player.h"

#include <math.h>

// Goto config.h and uncomment search for SUPPORT_FILEFORMAT_FLAC
#include "raylib.h"
#include "tracks.h"

//region Music stream handling
Music music = {0};
float volume = 0.3f;

// Music stream stuff
static void stopMusic() {
    UnloadMusicStream(music);
    music = (Music){0};
}

static bool changeMusic(const char* music_file_path) {
    UnloadMusicStream(music);

    music = LoadMusicStream(music_file_path);
    if (!IsMusicValid(music)) return false;

    PlayMusicStream(music);
    SetMusicVolume(music, volume);

    return true;
}
//endregion

//region Music Player
Albums albums = {0};
size_t current_album_index = 0;
size_t current_track_index = 0;

void musicPlayer_init(Albums album_list) {
    albums = album_list;
    //current_album_index = 0;
    //current_track_index = 0;
    //stopMusic();
}

void musicPlayer_update() {
    UpdateMusicStream(music);
}

void musicPlayer_cleanup() {
    stopMusic();
}

bool musicPlayer_play() {
    if (!IsMusicValid(music)) {
        const Track* track = musicPlayer_getCurrentTrack();
        if (track == nullptr) return false;
        if (!changeMusic(track->file_path)) return false;
    }

    ResumeMusicStream(music);
    return true;
}
void musicPlayer_pause() {
    PauseMusicStream(music);
}

// void musicPlayer_toggle() {
//     if (IsMusicStreamPlaying(music)) musicPlayer_pause();
//     else musicPlayer_play();
// }

void musicPlayer_previousTrack() {
    musicPlayer_changeTrack(current_track_index - 1);
}

void musicPlayer_nextTrack() {
    musicPlayer_changeTrack(current_track_index + 1);
}

// Setters
void musicPlayer_changeAlbum(const size_t index) {
    current_album_index = index;
    current_track_index = -1;

    stopMusic();
}

void musicPlayer_changeTrack(const size_t index) {
    current_track_index = index;

    const size_t current_album_tracks_count = albums.items[current_album_index].tracks.count;
    if (current_track_index >= current_album_tracks_count) current_track_index = current_album_tracks_count - 1;

    changeMusic(musicPlayer_getCurrentTrack()->file_path);
}

void musicPlayer_setProgress(float progress) {
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    if (!musicPlayer_isPlaying()) {
        if (!musicPlayer_play()) return;
    }

    SeekMusicStream(music, progress * musicPlayer_getTimeLength());
}

void musicPlayer_setVolume(const float new_volume) {
    volume = new_volume;
    SetMusicVolume(music, volume);
}

// Getters
static bool isValidAlbumList() {
    return albums.items != nullptr && albums.count != 0 && albums.capacity != 0;
}

const Albums* musicPlayer_getAlbumList() {
    return isValidAlbumList() ? &albums : nullptr;
}

const Album* musicPlayer_getCurrentAlbum() {
    if (!isValidAlbumList()) return nullptr;

    if (current_album_index >= albums.count) return nullptr;
    return &albums.items[current_album_index];
}

const Track* musicPlayer_getCurrentTrack() {
    const Album* current_album = musicPlayer_getCurrentAlbum();
    if (current_album == nullptr) return nullptr;

    if (current_track_index >= current_album->tracks.count) return nullptr;
    return &current_album->tracks.items[current_track_index];
}

bool musicPlayer_isPlaying() {
    return IsMusicStreamPlaying(music);
}

float musicPlayer_getTimePlayed() {
    return GetMusicTimePlayed(music);
}

float musicPlayer_getTimeLength() {
    const float time_length = GetMusicTimeLength(music);
    return !isnan(time_length) ? time_length : 0.0f;
}

float musicPlayer_getVolume() { return volume; }

//endregion