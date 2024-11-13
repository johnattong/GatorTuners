cmake_minimum_required(VERSION 3.29)

#set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
set(GCC_COVERAGE_LINK_FLAGS "-lcurl")

#set(CMAKE_CXX_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS "${GCC_COVERAGE_LINK_FLAGS}")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)


project(GatorTuners)

include_directories(src)
include_directories(include)

add_executable(Main src/main.cpp
        src/Spotify.cpp
        src/Spotify.h
        src/Models.cpp
        src/Models.h)

target_link_libraries(Main PRIVATE nlohmann_json::nlohmann_json)