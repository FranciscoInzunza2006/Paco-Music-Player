#include "music_player.h"

#include <math.h>

// Goto config.h and uncomment search for SUPPORT_FILEFORMAT_FLAC
#include "raylib.h"
#include "tracks.h"

//Album* current_album = nullptr;
//Track* playing_track = nullptr;
//size_t playing_track_index = 0;

// Music stream handling
Music music = {0};
//bool is_music_playing = false;
float volume = 0.3f;
bool music_has_changed = false;

static void playMusic();
static void stopMusic();

void changeMusic(const char* new_music_path);

// Music Player
Playlists albums = {0};
size_t current_album_index = 0;
size_t current_track_index = 0;

void musicPlayer_init(Playlists album_list) {
    albums = album_list;
    //current_album_index = 0;
    //current_track_index = 0;
    //stopMusic();
}

void musicPlayer_update() {
    //if (IsMusicValid(music))
    UpdateMusicStream(music);
    music_has_changed = false;
}

void musicPlayer_cleanup() {
    stopMusic();
    freeAlbumList(&albums, true);
}

void musicPlayer_toggleMusicPlaying() {
    if (!IsMusicValid(music)) {
        musicPlayer_changeTrack(current_track_index); // Force start
        return;
    }

    if (IsMusicStreamPlaying(music)) PauseMusicStream(music);
    else playMusic();
}

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

    // Force start if not playing anything, may cause problems.
    if (!IsMusicValid(music)) {
        musicPlayer_changeTrack(current_track_index);
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

const Playlists* musicPlayer_getAlbumList() {
    return isValidAlbumList() ? &albums : nullptr;
}

const Playlist* musicPlayer_getCurrentAlbum() {
    if (!isValidAlbumList()) return nullptr;

    return &albums.items[current_album_index];
}

const Track* musicPlayer_getCurrentTrack() {
    const Playlist* current_album = musicPlayer_getCurrentAlbum();
    if (current_album == nullptr) return nullptr;

    return &current_album->tracks.items[current_track_index];
}

bool musicPlayer_isPlaying() {
    return IsMusicStreamPlaying(music);
}

// bool musicPlayer_isPaused() {
//     return !IsMusicStreamPlaying(music);
// }

float musicPlayer_getTimePlayed() {
    return GetMusicTimePlayed(music);
}

float musicPlayer_getTimeLength() {
    const float time_length = GetMusicTimeLength(music);
    return !isnan(time_length) ? GetMusicTimeLength(music) : 0.0f;
}

float musicPlayer_getVolume() { return volume; }

// Music stream stuff
static void playMusic() {
    if (IsMusicValid(music)) ResumeMusicStream(music);
    else musicPlayer_changeTrack(current_track_index); // Start current selected song, probably the index 0.
}
static void stopMusic() {
    UnloadMusicStream(music);
    music = (Music){0};
}

void changeMusic(const char* new_music_path) {
    UnloadMusicStream(music);

    music = LoadMusicStream(new_music_path);
    if (!IsMusicValid(music)) return;

    PlayMusicStream(music);
    SetMusicVolume(music, volume);
    music_has_changed = true;
}
