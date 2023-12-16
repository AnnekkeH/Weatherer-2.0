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
	using Coordinates = std::pair<double, double>;

	class PvHandler {
		static constexpr std::string_view api_base_url_{"https://api.open-meteo.com/v1/forecast"};
		[[nodiscard]]
		static cpr::Response get_http_data(Coordinates const& coords, std::string const& date);


	public:
		[[nodiscard]]
		static PvDataPtr get_pv_data(Coordinates const& coords, std::string const& date);
		[[nodiscard]]
		static std::shared_ptr<std::map<std::string, PvDataPtr>> generate_pv_data(
			Coordinates const& coords, std::string const& start_date, std::string const& end_date);
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

