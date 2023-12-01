#include "PVMetrics.hpp"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

weatherer::PVMetrics::PVMetrics(double longitude, double latitude) :
	longitude_(longitude), latitude_(latitude) {
	this->pv_data_ = generate_data(longitude, latitude);
}

cpr::Response weatherer::PVMetrics::get_http_data(const double longitude, const double latitude) const {
	cpr::Parameters prams{};
	prams.Add(cpr::Parameter{"longitude", std::to_string(longitude)});
	prams.Add(cpr::Parameter{"latitude", std::to_string(latitude)});
	prams.Add(cpr::Parameter{"daily", "sunrise"});
	prams.Add(cpr::Parameter{"daily", "sunset"});
	prams.Add(cpr::Parameter{"hourly", "temperature_2m"});
	prams.Add(cpr::Parameter{"hourly", "cloud_cover"});
	prams.Add(cpr::Parameter{"hourly", "wind_speed_10m"});
	prams.Add(cpr::Parameter{"hourly", "direct_radiation"});
	prams.Add(cpr::Parameter{"hourly", "diffuse_radiation"});
	prams.Add(cpr::Parameter{"forecast_days", "1"});

	const cpr::Response response = cpr::Get(cpr::Url{this->api_base_url_}, prams);

	if (response.status_code != 200) {
		throw std::runtime_error(
			"An internal error occurred whilst fetching the HTTP data: " +
			std::to_string(response.status_code)
		);
	}

	return response;
}

std::shared_ptr<weatherer::PvData> weatherer::PVMetrics::generate_data(const double longitude,
                                                                       const double latitude) const {
	nlohmann::json data = nlohmann::json::parse(get_http_data(longitude, latitude).text);
	std::shared_ptr<PvData> pv_data = std::make_shared<PvData>();
	pv_data->sunrise_time = data.at("daily").at("sunrise").at(0);
	pv_data->sunset_time = data.at("daily").at("sunset").at(0);
	pv_data->diffuse_radiation = data.at("hourly").at("diffuse_radiation");
	pv_data->direct_radiation = data.at("hourly").at("direct_radiation");
	pv_data->temperature = data.at("hourly").at("temperature_2m");
	pv_data->cloud_cover_total = data.at("hourly").at("cloud_cover");
	pv_data->wind_speed = data.at("hourly").at("wind_speed_10m");
	return pv_data;
}

std::shared_ptr<weatherer::PvData> weatherer::PVMetrics::get_pv_data() const {
	return this->pv_data_;
}
