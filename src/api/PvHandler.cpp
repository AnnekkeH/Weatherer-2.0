#include "PvHandler.hpp"
#include "util/Time.hpp"
#include <cpr/cpr.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <ranges>

[[nodiscard]]
cpr::Response weatherer::PvHandler::get_http_data(const Coordinates& coords, const std::string& date) {
	cpr::Parameters prams{};
	prams.Add(cpr::Parameter{"longitude", std::to_string(coords.first)});
	prams.Add(cpr::Parameter{"latitude", std::to_string(coords.second)});
	prams.Add(cpr::Parameter{"daily", "sunrise"});
	prams.Add(cpr::Parameter{"daily", "sunset"});
	prams.Add(cpr::Parameter{"hourly", "temperature_2m"});
	prams.Add(cpr::Parameter{"hourly", "cloud_cover"});
	prams.Add(cpr::Parameter{"hourly", "wind_speed_10m"});
	prams.Add(cpr::Parameter{"hourly", "direct_radiation"});
	prams.Add(cpr::Parameter{"hourly", "diffuse_radiation"});
	prams.Add(cpr::Parameter{"start_date", date});
	prams.Add(cpr::Parameter{"end_date", date});

	const cpr::Response res = cpr::Get(cpr::Url{api_base_url_}, prams);
	if (res.status_code != 200) {
		throw std::runtime_error("An error has occurred whilst fetching HTTP data. Status code: "
			+ std::to_string(res.status_code));
	}

	return res;
}

[[nodiscard]]
weatherer::PvDataPtr weatherer::PvHandler::get_pv_data(const Coordinates& coords, const std::string& date) {
	using Json = nlohmann::json;

	const cpr::Response res = get_http_data(coords, date);
	const Json json = Json::parse(res.text);

	const auto pv_data = std::make_shared<PvData>();

	pv_data->sunrise_time = json.at("daily").at("sunrise").at(0);
	pv_data->sunset_time = json.at("daily").at("sunset").at(0);
	pv_data->diffuse_radiation = json.at("hourly").at("diffuse_radiation");
	pv_data->direct_radiation = json.at("hourly").at("direct_radiation");
	pv_data->temperature = json.at("hourly").at("temperature_2m");
	pv_data->cloud_cover_total = json.at("hourly").at("cloud_cover");
	pv_data->wind_speed = json.at("hourly").at("wind_speed_10m");
	return pv_data;
}

//TODO(OneCheetah): The max start date is 10 days in the past, implement a different API for longer periods.
[[nodiscard]]
std::shared_ptr<std::map<std::string, weatherer::PvDataPtr>> weatherer::PvHandler::generate_pv_data(
	const Coordinates& coords, const std::string& start_date, const std::string& end_date) {

	auto map = std::make_shared<std::map<std::string, PvDataPtr>>();
	const int total_days = util::Time::difference_in_days(start_date, end_date);
	for (const int i : std::ranges::views::iota(0, total_days)) {
		const auto [date, time] = weatherer::util::Time::get_future_date(i);
		map->insert({date, get_pv_data(coords, date)});
	}

	return map;
}
