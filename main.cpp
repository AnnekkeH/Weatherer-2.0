#include <iostream>
#include <memory>
#include "api/WeatherProvider.hpp"
#include "util/network/NetworkUtils.hpp"
#include <nlohmann/json.hpp>

int main() {
    
    const auto weather_provider = std::make_unique<weather::WeatherProvider>("37.11713922134418,-113.61091493319164");
    std::cout << weather_provider->get_forecast().dump(4) << "\n";
    return 0;
}