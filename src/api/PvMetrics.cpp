#include "PvMetrics.hpp"

#include <cmath>
#include <numbers>
#include <span>

#include "api/models/Coordinates.hpp"
#include "util/Date.hpp"
#include "util/Statistics.hpp"

int weatherer::PvMetrics::CalculateSolarNoonTime(const PvData& pv_data) {
  // Parse sunrise and sunset times from the photovoltaic data.
  const auto sunrise_time = util::Date{pv_data.GetSunriseTime()};
  const auto sunset_time = util::Date{pv_data.GetSunsetTime()};

  // Calculate the total duration between sunrise and sunset.
  const auto duration = sunset_time - sunrise_time;

  // Calculate the midpoint duration between sunrise and sunset.
  const auto midpoint_duration = duration / 2;

  // Calculate the solar noon time by adding the midpoint duration to the sunrise time.
  const auto solar_noon = sunrise_time + midpoint_duration;
  // Convert the solar noon time to a time structure to extract the hour.
  const std::time_t solar_noon_time = solar_noon.GetTime();
  std::tm solar_noon_tm{};

  localtime_s(&solar_noon_tm, &solar_noon_time);

  // Return the hour of the day when solar noon occurs. Adding one because the
  // tm_hour field in the time structure represents hours in the range [0, 23],
  // and we want to present the hour in the range [1, 24].
  return solar_noon_tm.tm_hour + 1;
}

double weatherer::PvMetrics::CalculateDailyEnergyYeild(
    PvData const& pv_data, Coordinates const& coordinates,
    std::string const& date, const double panel_eff, const double panel_area) {
  using namespace util;

  const auto convert_to_radians = [](const double degrees) {
    return degrees * (std::numbers::pi / 180.0);
  };

  const double radians_latitude = convert_to_radians(coordinates.GetLatitude());

  // Calcuate the laitude factor based on the solar declination angle.
  const int day_of_year =
      Date{date}.GetCurrentLocalTime().tm_yday;
  const double solar_declination_angle =
      23.45 * std::sin((365.0 / 360.0) * (day_of_year - 81));
  const double radian_solar_declination_angle =
      convert_to_radians(solar_declination_angle);
  const double latitude_factor =
      std::cos(radians_latitude) * std::cos(radian_solar_declination_angle);

  // Calculate the incident angle factor based on the solar noon time.
  const int solar_time = CalculateSolarNoonTime(pv_data);
  const double hour_angle = 15 * (solar_time - 12);
  const double solar_zenith_angle = std::asin(
      std::sin(radians_latitude) * std::sin(radian_solar_declination_angle) +
      std::cos(radians_latitude) * std::cos(radian_solar_declination_angle) *
          std::cos(convert_to_radians(hour_angle)));
  const double incident_angle_factor =
      std::cos(convert_to_radians(solar_zenith_angle));

  std::vector<double> hourly_pv{};
  hourly_pv.reserve(24);

  for (int i = 0; i < 24; i++) {
    const double solar_irradiance = pv_data.GetShortwaveRadiation().at(i);
    const double base_daily_energy_yield = solar_irradiance * panel_eff * panel_area;
    const double hourly_temperature = pv_data.GetTemperature().at(i);
    const double temperature_factor = hourly_temperature > 25 ? 1.0 - 0.004 * (hourly_temperature - 25) : 1.0;
    const double hourly_cloud_cover = pv_data.GetCloudCoverTotal().at(i);
    const double cloud_coverage_factor = (1 - hourly_cloud_cover);
    hourly_pv.push_back(base_daily_energy_yield * temperature_factor * latitude_factor *
         cloud_coverage_factor * incident_angle_factor);
  }

  return std::accumulate(hourly_pv.begin(), hourly_pv.end(), 0.0);
};