#pragma once
#include <array>
#include <ostream>
#include <print>
#include <string>

namespace weatherer {
struct PvData {
 private:
  std::string sunrise_time_;
  std::string sunset_time_;
  std::array<int, 24> diffuse_radiation_;
  std::array<int, 24> direct_radiation_;
  std::array<double, 24> temperature_;
  std::array<int, 24> cloud_cover_total_;
  std::array<double, 24> wind_speed_;

 public:
  [[nodiscard]] PvData();
  ~PvData();
  PvData(const PvData& other);
  PvData(PvData&& other) noexcept;
  PvData& operator=(const PvData& other);
  PvData& operator=(PvData&& other) noexcept;

  [[nodiscard]] std::string GetSunriseTime() const;

  void SetSunriseTime(const std::string& sunrise_time);

  [[nodiscard]] std::string GetSunsetTime() const;

  void SetSunsetTime(const std::string& sunset_time);

  [[nodiscard]] std::array<int, 24> GetDiffuseRadiation() const;
  void SetDiffuseRadiation(const std::array<int, 24>& diffuse_radiation);

  [[nodiscard]] std::array<int, 24> GetDirectRadiation() const;

  void SetDirectRadiation(const std::array<int, 24>& direct_radiation);
  [[nodiscard]] std::array<double, 24> GetTemperature() const;

  void SetTemperature(const std::array<double, 24>& temperature);

  [[nodiscard]] std::array<int, 24> GetCloudCoverTotal() const;

  void SetCloudCoverTotal(const std::array<int, 24>& cloud_cover_total);

  [[nodiscard]] std::array<double, 24> GetWindSpeed() const;

  void SetWindSpeed(const std::array<double, 24>& wind_speed);

  friend std::ostream& operator<<(std::ostream& os, const PvData& obj);
};

inline std::ostream& operator<<(std::ostream& os, const PvData& obj) {
  const auto print_arr = [&os]<typename Ty_, size_t Amt_>(
                             std::array<Ty_, Amt_> const& arr) -> void {
    std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<double>(os, " "));
  };
  os << "Sunrise Time: " << obj.sunrise_time_ << "\n"
     << "Sunset Time: " << obj.sunset_time_ << "\n"
     << "Diffuse Radiation: ";
  print_arr(obj.diffuse_radiation_);
  os << "\n"
     << "Direct Radiation: ";
  print_arr(obj.direct_radiation_);
  os << "\n"
     << "Temperature: ";
  print_arr(obj.temperature_);
  os << "\n"
     << "Cloud Cover Total: ";
  print_arr(obj.cloud_cover_total_);
  os << "\n"
     << "Wind Speed: ";
  print_arr(obj.wind_speed_);
  os << "\n";
  return os;
}
}  // namespace weatherer