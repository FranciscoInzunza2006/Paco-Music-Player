//
// Created by Francisco Javier on 21/01/2026.
//

#include "tracks.h"
#include <gtest/gtest.h>

#ifndef SAMPLES_DIRECTORY
#error "SAMPLES_DIRECTORY wasn't defined"
#endif

TEST(Track, getTrackWithProperMetadata) {
    Track track;
    const char* file_path = SAMPLES_DIRECTORY "/music_metadata.mp3";
    const bool result = getTrackWithMetadataFromFile(file_path, &track);

    ASSERT_TRUE(result);
    EXPECT_STREQ(track.title, "Peper Steak");
    EXPECT_STREQ(track.artist, "Alias Conrad Coldwood");
    EXPECT_STREQ(track.album, "OFF Original Soundtrack");
    EXPECT_STREQ(track.comment, "https://archive.org/details/offostflac");
    EXPECT_STREQ(track.genre, "");
    EXPECT_EQ(track.track_number, 4);
}

TEST(Track, getTrackWithNoMetadata) {
    Track track;
    const char* file_path = SAMPLES_DIRECTORY "/music_no_metadata.mp3";
    const bool result = getTrackWithMetadataFromFile(file_path, &track);

    ASSERT_TRUE(result);
    EXPECT_STREQ(track.title, "music_no_metadata");
    EXPECT_STREQ(track.artist, "");
    EXPECT_STREQ(track.album, "");
    EXPECT_STREQ(track.comment, "");
    EXPECT_STREQ(track.genre, "");
    EXPECT_EQ(track.track_number, 0);
}