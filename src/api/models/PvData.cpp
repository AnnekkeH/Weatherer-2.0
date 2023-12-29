#pragma once
#include "PvData.hpp"

weatherer::PvData::PvData()
    : sunrise_time_({}),
      sunset_time_({}),
      diffuse_radiation_({}),
      direct_radiation_({}),
      temperature_({}),
      cloud_cover_total_({}),
      wind_speed_({}) {}

weatherer::PvData::~PvData() = default;

weatherer::PvData::PvData(const PvData& other) = default;

weatherer::PvData::PvData(PvData&& other) noexcept
    : sunrise_time_(std::move(other.sunrise_time_)),
      sunset_time_(std::move(other.sunset_time_)),
      diffuse_radiation_(other.diffuse_radiation_),
      direct_radiation_(other.direct_radiation_),
      temperature_(other.temperature_),
      cloud_cover_total_(other.cloud_cover_total_),
      wind_speed_(other.wind_speed_) {}

weatherer::PvData& weatherer::PvData::operator=(const PvData& other) {
  if (this == &other) {
    return *this;
  }
  sunrise_time_ = other.sunrise_time_;
  sunset_time_ = other.sunset_time_;
  diffuse_radiation_ = other.diffuse_radiation_;
  direct_radiation_ = other.direct_radiation_;
  temperature_ = other.temperature_;
  cloud_cover_total_ = other.cloud_cover_total_;
  wind_speed_ = other.wind_speed_;
  return *this;
}

weatherer::PvData& weatherer::PvData::operator=(PvData&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  sunrise_time_ = std::move(other.sunrise_time_);
  sunset_time_ = std::move(other.sunset_time_);
  diffuse_radiation_ = other.diffuse_radiation_;
  direct_radiation_ = other.direct_radiation_;
  temperature_ = other.temperature_;
  cloud_cover_total_ = other.cloud_cover_total_;
  wind_speed_ = other.wind_speed_;
  return *this;
}

std::string weatherer::PvData::GetSunriseTime() const {
  return sunrise_time_;
}

void weatherer::PvData::SetSunriseTime(const std::string& sunrise_time) {
  sunrise_time_ = sunrise_time;
}

std::string weatherer::PvData::GetSunsetTime() const {
  return sunset_time_;
}

void weatherer::PvData::SetSunsetTime(const std::string& sunset_time) {
  sunset_time_ = sunset_time;
}

std::array<int, 24> weatherer::PvData::GetDiffuseRadiation() const {
  return diffuse_radiation_;
}

void weatherer::PvData::SetDiffuseRadiation(
    const std::array<int, 24>& diffuse_radiation) {
  diffuse_radiation_ = diffuse_radiation;
}

std::array<int, 24> weatherer::PvData::GetDirectRadiation() const {
  return direct_radiation_;
}

void weatherer::PvData::SetDirectRadiation(
    const std::array<int, 24>& direct_radiation) {
  direct_radiation_ = direct_radiation;
}

std::array<double, 24> weatherer::PvData::GetTemperature() const {
  return temperature_;
}

void weatherer::PvData::SetTemperature(
    const std::array<double, 24>& temperature) {
  temperature_ = temperature;
}

std::array<int, 24> weatherer::PvData::GetCloudCoverTotal() const {
  return cloud_cover_total_;
}

void weatherer::PvData::SetCloudCoverTotal(
    const std::array<int, 24>& cloud_cover_total) {
  cloud_cover_total_ = cloud_cover_total;
}

std::array<double, 24> weatherer::PvData::GetWindSpeed() const {
  return wind_speed_;
}

void weatherer::PvData::SetWindSpeed(const std::array<double, 24>& wind_speed) {
  wind_speed_ = wind_speed;
}