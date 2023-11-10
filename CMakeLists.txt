cmake_minimum_required(VERSION 3.28)
project(Weatherer)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(Weatherer "main.cpp"
        "api/WeatherProvider.cpp"
        "api/WeatherProvider.hpp"  "util/network/NetworkUtils.hpp" "util/network/NetworkUtils.cpp")

target_link_libraries(Weatherer PRIVATE cpr::cpr)
target_link_libraries(Weatherer PRIVATE nlohmann_json::nlohmann_json)