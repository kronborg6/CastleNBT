cmake_minimum_required(VERSION 4.2)

project(
    castleNBT
    VERSION 0.1.0
    DESCRIPTION "Minecraft NBT library"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(castleNBT STATIC
    src/endian.cpp
    src/writeNbt.cpp
    src/writeTag.cpp
    src/nbt.cpp
    src/child.cpp
)

target_include_directories(castleNBT
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(castleNBT PUBLIC cxx_std_23)

# Executable
add_executable(castleNBTTool
    tools/main.cpp
)

target_link_libraries(castleNBTTool
    PRIVATE castleNBT
)
