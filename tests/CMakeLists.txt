include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    nbt_hex_test.cpp
)

target_include_directories(unit_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(unit_tests
    PRIVATE
    castleNBT
        GTest::gtest_main
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
