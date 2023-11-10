#include "WeatherProvider.hpp"
#include <cpr/cpr.h>

std::string weather::WeatherProvider::get_data(const std::string& location) noexcept {
    const cpr::Response response = cpr::Get(
        cpr::Url{ "https://api.weather.gov/points/" + location });
    if (response.status_code != 200) {
        return std::string{};
    }

    return response.text;
}
