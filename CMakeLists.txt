add_library(nbt STATIC
    src/endian.cpp
    src/nbt.cpp
)

target_include_directories(nbt
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(nbt PUBLIC cxx_std_23)
