
include(GoogleTest)

add_executable(filesystem_tests filesystem/filesystem_tests.cpp)
target_link_libraries(filesystem_tests
        PRIVATE Filesystem
        PRIVATE GTest::gtest_main
)
gtest_discover_tests(filesystem_tests)