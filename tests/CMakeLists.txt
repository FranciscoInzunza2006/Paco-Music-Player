
include(GoogleTest)

add_executable(example_test test_example/example_tests.cpp
        test_example/example_tests.cpp)
target_link_libraries(example_test
        GTest::gtest_main
)
gtest_discover_tests(example_test)