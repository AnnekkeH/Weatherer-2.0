#include "CropWeatherData.hpp"

CropWeatherData::CropWeatherData(const double temperature,
                                 const double soil_temperature_18)
    : temperature_(temperature), soil_temperature_18_(soil_temperature_18) {}

CropWeatherData::CropWeatherData(const CropWeatherData& other) = default;

CropWeatherData::CropWeatherData(CropWeatherData&& other) noexcept
    : temperature_(other.temperature_),
      soil_temperature_18_(other.soil_temperature_18_) {}

CropWeatherData& CropWeatherData::operator=(const CropWeatherData& other) {
  if (this == &other)
    return *this;
  temperature_ = other.temperature_;
  soil_temperature_18_ = other.soil_temperature_18_;
  return *this;
}

CropWeatherData& CropWeatherData::operator=(CropWeatherData&& other) noexcept {
  if (this == &other)
    return *this;
  temperature_ = other.temperature_;
  soil_temperature_18_ = other.soil_temperature_18_;
  return *this;
}

double CropWeatherData::GetTemperature() const {
  return temperature_;
}

double CropWeatherData::GetSoilTemperature18() const {
  return soil_temperature_18_;
}

void CropWeatherData::SetTemperature(double temperature) {
  temperature_ = temperature;
}

void CropWeatherData::SetSoilTemperature18(double soil_temperature_18) {
  soil_temperature_18_ = soil_temperature_18;
}
