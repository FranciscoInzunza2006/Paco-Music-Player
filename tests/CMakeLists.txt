
include(GoogleTest)

add_executable(filesystem_tests filesystem/filesystem_tests.cpp
        filesystem/filesystem_tests.cpp)
target_link_libraries(filesystem_tests
        filesystem
        GTest::gtest_main
)
gtest_discover_tests(filesystem_tests)