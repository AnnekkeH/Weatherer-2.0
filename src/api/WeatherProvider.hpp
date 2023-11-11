#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace weather {
    class WeatherProvider {
        using json = nlohmann::json;
    private:
        std::string base_url{}; // The base URL for the weather API
        json json_data{}; // The JSON data returned by the weather API
        std::string location{}; // The location for which the weather data is being fetched

    public:
        // Constructor that takes a location parameter and initializes the location member variable
        WeatherProvider(const std::string& location);

        // Returns the base URL for the weather API
        [[nodiscard]]
        std::string get_base_url() const;

        // Returns the JSON data returned by the weather API
        [[nodiscard]]
        json get_json_data() const;

        // Returns the location for which the weather data is being fetched
        [[nodiscard]]
        std::string get_location() const;

        // Returns a JSON object representing the forecast data for the current location
        [[nodiscard]]
        json get_two_week_forecast() const;

        // Returns a JSON object representing the hourly forecast data for the current location
        [[nodiscard]]
        json forecast_hourly() const;
    };
}
