cmake_minimum_required(VERSION 3.24)

project(
    castleNBT
    VERSION 0.1.1
    DESCRIPTION "Minecraft NBT library"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(castleNBT STATIC
    src/createNbt.cpp
    src/endian.cpp
    src/reader.cpp
    src/readNbt.cpp
    src/readTag.cpp
    src/variable.cpp
    src/writeNbt.cpp
    src/writer.cpp
    src/writeTag.cpp
    src/nbt.cpp
)

target_include_directories(castleNBT
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(castleNBT PUBLIC cxx_std_23)

if(PROJECT_IS_TOP_LEVEL)
    add_executable(castleNBTTool
        tools/main.cpp
    )

    target_link_libraries(castleNBTTool
        PRIVATE castleNBT
    )

    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
