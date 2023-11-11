#include "WeatherProvider.hpp"
#include "../util/network/NetworkUtils.hpp"
#include <nlohmann/json.hpp>

weather::WeatherProvider::WeatherProvider(const std::string& location) : location(location) {
	this->base_url = "https://api.weather.gov/points/" + location;
	this->json_data = json::parse(weather::NetworkUtils::get_http_response(this->base_url));
}

std::string weather::WeatherProvider::get_base_url() const {
	return this->base_url;
}

weather::WeatherProvider::json weather::WeatherProvider::get_json_data() const {
	return this->json_data;
}

std::string weather::WeatherProvider::get_location() const {
	return this->location;
}

nlohmann::json weather::WeatherProvider::get_two_week_forecast() const {
	return weather::NetworkUtils::get_http_response(
		weather::NetworkUtils::get_http_data(this->get_json_data(), "properties", "forecast"));
}

nlohmann::json weather::WeatherProvider::forecast_hourly() const {
	return weather::NetworkUtils::get_http_response(
		weather::NetworkUtils::get_http_data(this->get_json_data(), "properties", "forecastHourly"));
}
