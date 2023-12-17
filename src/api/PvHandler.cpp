#include "PvHandler.hpp"
#include "util/Time.hpp"
#include <cpr/cpr.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <ranges>

[[nodiscard]]
cpr::Response weatherer::PvHandler::get_http_data(const Coordinates& coords, const std::string& date) {
	return get_http_data(coords, date, date);
}

cpr::Response weatherer::PvHandler::get_http_data(const Coordinates& coords, const std::string& start_date,
                                                  const std::string& end_date) {
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
	prams.Add(cpr::Parameter{"start_date", start_date});
	prams.Add(cpr::Parameter{"end_date", end_date});

	cpr::Response res = cpr::Get(cpr::Url{api_url_}, prams);
	if (res.status_code != 200) {
		throw std::runtime_error("An error has occurred whilst fetching HTTP data. Status code: "
			+ std::to_string(res.status_code));
	}
	return res;
}

cpr::Response weatherer::PvHandler::get_historical_http_data(const Coordinates& coords, const std::string& date) {
	return get_historical_http_data(coords, date, date);
}

cpr::Response weatherer::PvHandler::get_historical_http_data(const Coordinates& coords, const std::string& start_date,
                                                             const std::string& end_date) {
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
	prams.Add(cpr::Parameter{"start_date", start_date});
	prams.Add(cpr::Parameter{"end_date", end_date});

	cpr::Response res = cpr::Get(cpr::Url{historical_api_url_}, prams);
	if (res.status_code != 200) {
		throw std::runtime_error(
			"An error has occurred whilst fetching HTTP data. Status code: " + std::to_string(res.status_code));
	}
	return res;
}


[[nodiscard]]
weatherer::PvDataPtr weatherer::PvHandler::get_data(const Coordinates& coords, const std::string& date) {
	using Json = nlohmann::json;

	const cpr::Response res = get_http_data(coords, date);
	const Json json = Json::parse(res.text);

	auto pv_data = std::make_shared<PvData>();

	pv_data->sunrise_time = json.at("daily").at("sunrise").at(0);
	pv_data->sunset_time = json.at("daily").at("sunset").at(0);
	pv_data->diffuse_radiation = json.at("hourly").at("diffuse_radiation");
	pv_data->direct_radiation = json.at("hourly").at("direct_radiation");
	pv_data->temperature = json.at("hourly").at("temperature_2m");
	pv_data->cloud_cover_total = json.at("hourly").at("cloud_cover");
	pv_data->wind_speed = json.at("hourly").at("wind_speed_10m");
	return pv_data;
}


weatherer::BulkPvDataPtr weatherer::PvHandler::generate(const std::string& response,
                                                        const std::string& start_date,
                                                        const std::string& end_date) {
	using namespace util;
	using Json = nlohmann::json;
	int total_days = Time::difference_in_days(start_date, end_date);

	auto data = std::make_shared<std::map<std::string, PvDataPtr>>();
	Json json = Json::parse(response);

	const auto diffuse_radiations = Streams::split_array<int, 24>(json.at("hourly").at("diffuse_radiation"),
	                                                              total_days + 1);
	const auto direct_radiations = Streams::split_array<int, 24>(json.at("hourly").at("direct_radiation"),
	                                                             total_days + 1);
	const auto temperatures = Streams::split_array<double, 24>(json.at("hourly").at("temperature_2m"),
	                                                           total_days + 1);
	const auto cloud_cover_totals = Streams::split_array<int, 24>(json.at("hourly").at("cloud_cover"),
	                                                              total_days + 1);
	const auto wind_speeds = Streams::split_array<double, 24>(json.at("hourly").at("wind_speed_10m"),
	                                                          total_days + 1);

	for (const int i : std::ranges::views::iota(0, total_days)) {
		const auto [date, val] = Time::get_future_date(start_date, i + 1);

		auto pv_data = std::make_shared<PvData>();
		pv_data->sunrise_time = json.at("daily").at("sunrise").at(i);
		pv_data->sunset_time = json.at("daily").at("sunset").at(i);
		pv_data->diffuse_radiation = diffuse_radiations.at(i);
		pv_data->direct_radiation = direct_radiations.at(i);
		pv_data->temperature = temperatures.at(i);
		pv_data->cloud_cover_total = cloud_cover_totals.at(i);
		pv_data->wind_speed = wind_speeds.at(i);

		data->insert({date, pv_data});
	}
	return data;
}

[[nodiscard]]
weatherer::BulkPvDataPtr weatherer::PvHandler::aggregate(const Coordinates& coords,
                                                         const std::string& start_date,
                                                         const std::string& end_date) {
	using namespace util;

	// If we started in the past and ended in the past
	if (Time::difference_in_days(start_date, end_date) >= 14 && Time::difference_in_days(
		end_date, Time::get_current_date().time_) >= 5) {
		const std::string response = get_historical_http_data(coords, start_date, end_date).text;
		return generate(response, start_date, end_date);
	}
	// If we started in the past and ended in the future
	if (Time::difference_in_days(start_date, end_date) >= 14 && Time::difference_in_days(
		end_date, Time::get_current_date().time_) <= 5) {
		const std::string first_reponse = get_historical_http_data(coords, start_date, Time::get_past_date(5).time_).
			text;
		const std::string second_reponse = get_http_data(coords, Time::get_past_date(5).time_, end_date).text;

		const BulkPvDataPtr first_data = generate(first_reponse, start_date, Time::get_past_date(5).time_);
		const BulkPvDataPtr second_data = generate(second_reponse, Time::get_past_date(5).time_, end_date);

		first_data->insert(second_data->begin(), second_data->end());
		return first_data;
	}
	// If we started in the future and ended in the future
	return generate(get_http_data(coords, start_date, end_date).text, start_date, end_date);
}
