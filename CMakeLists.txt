cmake_minimum_required(VERSION 3.29)

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
        src/Models.h
        src/BTree.h
        src/Graph.h
        src/Stopwatch.cpp
        src/Stopwatch.h
)


if (WIN32)
    # Find and link Curl for Windows
    find_package(CURL REQUIRED)
    target_link_libraries(Main PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
else()
    # Unix-specific linking
    set(GCC_COVERAGE_LINK_FLAGS "-lcurl")
    set(CMAKE_EXE_LINKER_FLAGS "${GCC_COVERAGE_LINK_FLAGS}")
    target_link_libraries(Main PRIVATE nlohmann_json::nlohmann_json)

endif()