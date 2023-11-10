cmake_minimum_required(VERSION 3.28)
project(Foxtrot)

find_package(cpr CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(Foxtrot main.cpp
        api/WeatherProvider.cpp
        api/WeatherProvider.hpp)

target_link_libraries(Foxtrot PRIVATE cpr::cpr)