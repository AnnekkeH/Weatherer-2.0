#include "PvData.hpp"

#include <algorithm>

weatherer::PvData::PvData()
    : date_({}),
      sunrise_time_(0),
      sunset_time_(0),
      shortwave_radiation_({}),
      temperature_({}),
      cloud_cover_total_({}),
      wind_speed_({}) {}

weatherer::PvData::~PvData() = default;

weatherer::PvData::PvData(const PvData& other) = default;

weatherer::PvData::PvData(PvData&& other) noexcept
      : date_(std::move(other.date_)),
        sunrise_time_(other.sunrise_time_),
        sunset_time_(other.sunset_time_),
        shortwave_radiation_(other.shortwave_radiation_),
        temperature_(other.temperature_),
        cloud_cover_total_(other.cloud_cover_total_),
        wind_speed_(other.wind_speed_) {}

weatherer::PvData& weatherer::PvData::operator=(const PvData& other) {
  if (this == &other)
    return *this;
  date_ = other.date_;
  sunrise_time_ = other.sunrise_time_;
  sunset_time_ = other.sunset_time_;
  shortwave_radiation_ = other.shortwave_radiation_;
  temperature_ = other.temperature_;
  cloud_cover_total_ = other.cloud_cover_total_;
  wind_speed_ = other.wind_speed_;
  return *this;
}
weatherer::PvData& weatherer::PvData::operator=(PvData&& other) noexcept {
  if (this == &other)
    return *this;
  date_ = std::move(other.date_);
  sunrise_time_ = other.sunrise_time_;
  sunset_time_ = other.sunset_time_;
  shortwave_radiation_ = other.shortwave_radiation_;
  temperature_ = other.temperature_;
  cloud_cover_total_ = other.cloud_cover_total_;
  wind_speed_ = other.wind_speed_;
  return *this;
}

std::string weatherer::PvData::GetDate() const {
  return date_;
}

void weatherer::PvData::SetDate(const std::string& date) {
  date_ = date;
}

std::time_t weatherer::PvData::GetSunriseTime() const {
  return sunrise_time_;
}

void weatherer::PvData::SetSunriseTime(const std::time_t sunrise_time) {
  sunrise_time_ = sunrise_time;
}

std::time_t weatherer::PvData::GetSunsetTime() const {
  return sunset_time_;
}

void weatherer::PvData::SetSunsetTime(const std::time_t sunset_time) {
  sunset_time_ = sunset_time;
}

std::array<double, 24> weatherer::PvData::GetShortwaveRadiation() const {
  return shortwave_radiation_;
}

void weatherer::PvData::SetShortwaveRadiation(
    const std::array<int, 24>& shortwave_radiation) {
  shortwave_radiation_ = std::array<double, 24>{};
  // Divide by 1000 to convert from W/m^2 to kWh/m^2.
  std::ranges::transform(shortwave_radiation.begin(), shortwave_radiation.end(),
                         shortwave_radiation_.begin(),
                         [](const int rad) {
                           return rad / 1000.0;
                         });
}

std::array<double, 24> weatherer::PvData::GetTemperature() const {
  return temperature_;
}

void weatherer::PvData::SetTemperature(
    const std::array<double, 24>& temperature) {
  temperature_ = temperature;
}

std::array<double, 24> weatherer::PvData::GetCloudCoverTotal() const {
  return cloud_cover_total_;
}

void weatherer::PvData::SetCloudCoverTotal(
    const std::array<int, 24>& cloud_cover_total) {
  cloud_cover_total_ = std::array<double, 24>{};
  // Divide by 100 to convert from percentage to decimal.
  std::ranges::transform(cloud_cover_total.begin(), cloud_cover_total.end(),
                         cloud_cover_total_.begin(),
                         [](const int cover) {
                           return cover / 100.0;
                         });
}

std::array<double, 24> weatherer::PvData::GetWindSpeed() const {
  return wind_speed_;
}

void weatherer::PvData::SetWindSpeed(const std::array<double, 24>& wind_speed) {
  wind_speed_ = wind_speed;
}