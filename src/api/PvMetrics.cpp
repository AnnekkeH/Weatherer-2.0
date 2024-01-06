#include "PvMetrics.hpp"

#include <cmath>
#include <numbers>
#include <span>
#include <vector>

#include "api/models/Coordinates.hpp"
#include "util/Statistics.hpp"
#include "util/Time.hpp"

double weatherer::PvMetrics::CalculateSolarIrradiance(const PvData& pv_data) {
  std::vector<double> hourly_sum(24);

  // Fill the hourly_sum vector with values from 0 to 23.
  std::iota(hourly_sum.begin(), hourly_sum.end(), 0);

  // Calculate the sum of direct and diffuse radiation for each hour.
  std::ranges::transform(hourly_sum.begin(), hourly_sum.end(),
                         hourly_sum.begin(), [pv_data](const int hour) {
                           return pv_data.GetDirectRadiation().at(hour) +
                                  pv_data.GetDiffuseRadiation().at(hour);
                         });

  // Calculate the mean of the hourly sum, representing the solar irradiance.
  return util::Statistics::Mean(std::span{hourly_sum});
}

int weatherer::PvMetrics::CalculateSolarNoonTime(const PvData& pv_data) {
  // Parse sunrise and sunset times from the photovoltaic data.
  const auto sunrise_time = util::Time::ParseTime(pv_data.GetSunriseTime());
  const auto sunset_time = util::Time::ParseTime(pv_data.GetSunsetTime());

  // Calculate the total duration between sunrise and sunset.
  const auto duration = sunset_time - sunrise_time;

  // Calculate the midpoint duration between sunrise and sunset.
  const auto midpoint_duration = duration / 2;

  // Calculate the solar noon time by adding the midpoint duration to the sunrise time.
  const auto solar_noon = sunrise_time + midpoint_duration;
  // Convert the solar noon time to a time structure to extract the hour.
  const std::time_t solar_noon_time =
      std::chrono::system_clock::to_time_t(solar_noon);
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

  // Lambada function to convert degrees to radians
  const auto convert_to_radians = [](const double degrees) {
    return degrees * (std::numbers::pi / 180);
  };

  // Calculate the average temperature from hourly temperature data
  const double average_temperature = Statistics::Mean(std::span{
      pv_data.GetTemperature().data(), pv_data.GetTemperature().size()});
  // Calculate the average cloud cover from hourly data
  const double average_cloud_cover =
      Statistics::Mean(std::span{pv_data.GetCloudCoverTotal().data(),
                                 pv_data.GetCloudCoverTotal().size()}) /
      100;

  // Convert latitude to radians
  const double radians_latitude = convert_to_radians(coordinates.GetLatitude());
  // Calculate solar irradiance using a helper function
  const double solar_irradiance = CalculateSolarIrradiance(pv_data);

  // Calculate solar time and day of the year
  const int solar_time = CalculateSolarNoonTime(pv_data);
  const int day_of_year =
      Time::ConvertTimePointToTm(Time::ParseDate(date)).tm_yday;

  // Calculate solar declination angle (δ)
  const double sollar_declination =
      23.45 * sin((365.0 / 360.0) * (day_of_year - 81));
  const double radians_sollar_declination =
      convert_to_radians(sollar_declination);

  // Calculate latitude factor
  const double latitude_factor =
      cos(radians_latitude) * cos(radians_sollar_declination);

  // Calculate temperature factor
  const double temperature_factor = 1 - 0.005 * (average_temperature - 25);

  // Calculate hour angle for solar zenith angle calculation
  const int hour_angle = 15 * (solar_time - 12);

  // Calculate incident angle factor
  const double solar_zentith_angle =
      asin(sin(radians_latitude) * sin(radians_sollar_declination) +
           cos(radians_latitude) * cos(radians_sollar_declination) *
               cos(convert_to_radians(hour_angle)));
  const double incident_angle_factor = cos(solar_zentith_angle);

  // Adjust irradiance with factors
  const double adjusted_irradiance =
      solar_irradiance * latitude_factor * temperature_factor *
      incident_angle_factor * (1 - average_cloud_cover);

  // Calculate and return the daily energy yield
  return adjusted_irradiance * panel_eff * panel_area;
};