cmake_minimum_required(VERSION 3.28)
project(
    Weatherer
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include_directories("src")
# include_directories("include")

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE}" "${CMAKE_BINARY_DIR}/vcpkg.cmake")

# add_library(CXX_MODULES)
#[===[
target_sources(CXX_MODULES
  PUBLIC
    FILE_SET CXX_MODULES FILES
    "src/api/PVMetrics.ixx"
  
)
]===]


add_executable(Weatherer "src/main.cpp" 
    "src/api/PvHandler.cpp"
    "src/util/Time.cpp" 
)

# target_link_libraries(Weatherer PRIVATE CXX_MODULES)

target_link_libraries(Weatherer PRIVATE cpr::cpr)
target_link_libraries(Weatherer PRIVATE nlohmann_json::nlohmann_json)

