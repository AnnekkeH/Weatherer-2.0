#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace weather {
    class WeatherProvider {
    using json = nlohmann::json;
    private:
        std::string base_url{};
        json json_data{};
        std::string location{};

    public:
        WeatherProvider(const std::string& location);

        [[nodiscard]]
        std::string get_base_url() const;
        [[nodiscard]]
        json get_json_data() const;
        [[nodiscard]]
        std::string get_location() const;
        [[nodiscard]]
        json get_forecast() const;
        [[nodiscard]]
        json forecast_hourly() const;
    };
}
