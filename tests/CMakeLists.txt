include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    nbt_serialization_test.cpp
    nbt_class_test.cpp
    nbt_deserialization_test.cpp
    nbt_more_tests.cpp
    nbt_reader_test.cpp
)

target_include_directories(unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(unit_tests
    PRIVATE
        castleNBT
        GTest::gtest_main
        # GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
