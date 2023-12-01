#pragma once
#include <string>
#include <memory>
#include <array>
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
	};

	class PVMetrics {
	private:
		const std::string_view api_base_url_{ "https://api.open-meteo.com/v1/forecast" };
		std::shared_ptr<PvData> pv_data_;

		double longitude_;
		double latitude_;
	
		cpr::Response get_http_data(const double longitude, const double latitude) const;
		std::shared_ptr<PvData> generate_data(const double longitude, const double latitude) const;
	public:
		PVMetrics(double longitude, double latitude);

		std::shared_ptr<PvData> get_pv_data() const;

	};
}
