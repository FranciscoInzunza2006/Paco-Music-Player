
include(GoogleTest)

add_executable(track_test track_test.cpp)
target_link_libraries(track_test
        PRIVATE Tracks
        PRIVATE GTest::gtest_main
)
gtest_discover_tests(track_test)