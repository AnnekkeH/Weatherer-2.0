#pragma once
#include <string>
#include <memory>
#include <map>
#include <ostream>
#include <utility>
#include "util/Streams.hpp"

#include <cpr/response.h>

namespace weatherer {

	struct PvData {
		std::string sunrise_time;
		std::string sunset_time;
		std::array<int, 24> diffuse_radiation;
		std::array<int, 24> direct_radiation;
		std::array<double, 24> temperature;
		std::array<int, 24> cloud_cover_total;
		std::array<double, 24> wind_speed;

		friend std::ostream& operator<<(std::ostream& os, const PvData& obj);
	};

	using PvDataPtr = std::shared_ptr<PvData>;
	using BulkPvDataPtr = std::shared_ptr<std::map<std::string, PvDataPtr>>;
	using Coordinates = std::pair<double, double>;

	class PvHandler {
		static constexpr std::string_view api_url_{"https://api.open-meteo.com/v1/forecast"};
		static constexpr std::string_view historical_api_url_{"https://archive-api.open-meteo.com/v1/archive"};

		[[nodiscard]]
		static cpr::Response get_http_data(const Coordinates& coords, const std::string& date);

		[[nodiscard]]
		static cpr::Response get_http_data(const Coordinates& coords, const std::string& start_date,
		                                   const std::string& end_date);

		[[nodiscard]]
		static cpr::Response get_historical_http_data(const Coordinates& coords, const std::string& date);

		[[nodiscard]]
		static cpr::Response get_historical_http_data(const Coordinates& coords, const std::string& start_date,
		                                              const std::string& end_date);

		static BulkPvDataPtr generate(const std::string& response, const std::string& start_date,
		                              const std::string& end_date);


	public:
		[[nodiscard]]
		static PvDataPtr get_data(const Coordinates& coords, const std::string& date);

		[[nodiscard]]
		static std::shared_ptr<std::map<std::string, PvDataPtr>> aggregate(
			const Coordinates& coords, const std::string& start_date, const std::string& end_date);
	};

	inline std::ostream& operator<<(std::ostream& os, const PvData& obj) {
		os << "sunrise_time: " << obj.sunrise_time << "\n"
			<< "sunset_time: " << obj.sunset_time << "\n";

		util::Streams::os_array(os, obj.diffuse_radiation, "diffuse_radiation");
		util::Streams::os_array(os, obj.direct_radiation, "direct_radiation");
		util::Streams::os_array(os, obj.temperature, "temperature");
		util::Streams::os_array(os, obj.cloud_cover_total, "cloud_cover_total");
		util::Streams::os_array(os, obj.wind_speed, "wind_speed");
		return os;
	}
}
