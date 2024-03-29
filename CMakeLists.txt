cmake_minimum_required(VERSION 3.27)
project(
    Weatherer
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)

include_directories("src")

set (SOURCES 
        src/main.cpp
        src/api/PvHandler.cpp
        src/api/PvHandler.hpp
        src/util/ChunkOperator.hpp
        src/util/Date.cpp
        src/util/Date.hpp
        src/util/Statistics.hpp
        src/api/models/Coordinates.cpp
        src/api/models/Coordinates.hpp
        src/api/models/PvData.cpp
        src/api/models/PvData.hpp
        src/api/PvMetrics.cpp
        src/api/PvMetrics.hpp
        src/api/PvDataProcessor.hpp
        src/api/PvDataProcessor.cpp
)

add_executable(Weatherer ${SOURCES})
target_link_libraries(Weatherer PRIVATE cpr::cpr)
target_link_libraries(Weatherer PRIVATE nlohmann_json::nlohmann_json)