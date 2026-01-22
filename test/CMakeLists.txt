
include(GoogleTest)

set(SAMPLES_DIRECTORY "\"${CMAKE_CURRENT_SOURCE_DIR}/samples\"")

add_executable(track_test track_test.cpp)
target_link_libraries(track_test
        PRIVATE Tracks
        PRIVATE GTest::gtest_main
)
target_compile_definitions(track_test PRIVATE SAMPLES_DIRECTORY=${SAMPLES_DIRECTORY})
gtest_discover_tests(track_test)