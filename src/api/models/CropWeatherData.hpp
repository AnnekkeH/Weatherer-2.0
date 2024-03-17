#pragma once

class CropWeatherData {
 private:
  double temperature_;
  double soil_temperature_18_;

 public:
  CropWeatherData(double temperature, double soil_temperature_18);
  [[nodiscard]] double GetTemperature() const;
  [[nodiscard]] double GetSoilTemperature18() const;
  void SetTemperature(double temperature);
  void SetSoilTemperature18(double soil_temperature_18);
  CropWeatherData(const CropWeatherData& other);
  CropWeatherData(CropWeatherData&& other) noexcept;
  CropWeatherData& operator=(const CropWeatherData& other);
  CropWeatherData& operator=(CropWeatherData&& other) noexcept;
};
