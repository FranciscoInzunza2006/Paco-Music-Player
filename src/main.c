#include <stdlib.h>

#include "tracks.h"

#include "music_player.h"
#include "interface.h"

TrackList getTracksFromPaths();

// static void ButtonShuffle() {
// }

int main(void) {
    TrackList all_tracks = getTracksFromPaths();
    GuiLayoutState state = interfaceInit();

    musicPlayer_init(organizeTracksIntoAlbums(&all_tracks));
    while (interfaceShouldUpdate()) {
        // Update
        musicPlayer_update();
        interfaceUpdate(&state);

        // Draw

    }
    interfaceCleanUp();

    //freeTrackList(&all_tracks);
    //freeAlbumList(&albums, false);

    return 0;
}

TrackList getTracksFromPaths() {
    const char* MUSIC_DIRECTORIES_PATH[] = {
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\Terraria Official Soundtrack",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot Solstice OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\UNDERTALE Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\DELTARUNESoundtrack",
        //"D:\\Musica\\OFF",
        //"D:\\Musica\\Ribbit",
    };
    //constexpr size_t MUSIC_DIRECTORIES_COUNT = _countof(MUSIC_DIRECTORIES_PATH);


    const char* music_directory_path = MUSIC_DIRECTORIES_PATH[0];
    return getTrackListFromDirectory(music_directory_path);

    // for (size_t directory_index = 0; directory_index < MUSIC_DIRECTORIES_COUNT; directory_index++) {
    //     const char* music_directory_path = MUSIC_DIRECTORIES_PATH[directory_index];
    //    return getTrackListFromDirectory(music_directory_path);
    //
    // }

    //return tracks;
}
