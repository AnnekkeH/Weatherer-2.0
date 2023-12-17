cmake_minimum_required(VERSION 3.27)
project(
    Weatherer
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

include_directories("src")

set (SOURCES 
    src/main.cpp
    src/api/PvHandler.cpp
    src/api/PvHandler.hpp
    src/util/Streams.hpp
    src/util/Time.cpp
    src/util/Time.hpp
)

add_executable(Weatherer ${SOURCES})
target_link_libraries(Weatherer PRIVATE cpr::cpr)
target_link_libraries(Weatherer PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Weatherer PRIVATE range-v3::meta range-v3::concepts range-v3::range-v3)