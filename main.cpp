#include <iostream>
#include "api/WeatherProvider.hpp"


int main() {
    std::cout << weather::WeatherProvider::get_data("37.11713922134418,-113.61091493319164")
              << "\n";
    return 0;
}