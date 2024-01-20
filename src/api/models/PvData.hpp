#pragma once
#include <array>
#include <ostream>
#include <print>
#include <string>

#include "util/Date.hpp"

namespace weatherer {
/**
 * @brief Represents photovoltaic (PV) system data for a specific time frame.
 *
 * The PvData class encapsulates information related to weather conditions over a
 * 24-hour time frame, including sunrise and sunset times, diffuse and direct
 * radiation, temperature, cloud cover, and wind speed. It provides methods for
 * accessing and setting these data attributes and has a friend operator<< function
 * for convenient ostream output.
 */
class PvData {
 private:
  // Date format (e.g. 2021-01-01)
  std::string date_;
  // Military time format (e.g. 00:00, 01:00, 02:00, ..., 23:00)
  std::time_t sunrise_time_;
  // Military time format (e.g. 00:00, 01:00, 02:00, ..., 23:00)
  std::time_t sunset_time_;
  // Units: kWh/m^2
  std::array<double, 24> diffuse_radiation_;
  // Units: kWh/m^2
  std::array<double, 24> direct_radiation_;
  // Units: degrees Celsius
  std::array<double, 24> temperature_;
  // Value between 0 and 1
  std::array<double, 24> cloud_cover_total_;
  // Units: km/h
  std::array<double, 24> wind_speed_;

 public:
  [[nodiscard]] explicit PvData();
  ~PvData();
  PvData(const PvData& other);
  PvData(PvData&& other) noexcept;
  PvData& operator=(const PvData& other);
  PvData& operator=(PvData&& other) noexcept;

  [[nodiscard]] std::string GetDate() const;

  void SetDate(const std::string& date);

  [[nodiscard]] std::time_t GetSunriseTime() const;

  void SetSunriseTime(const std::time_t sunrise_time);

  [[nodiscard]] std::time_t GetSunsetTime() const;

  void SetSunsetTime(const std::time_t sunset_time);

  [[nodiscard]] std::array<double, 24> GetDiffuseRadiation() const;

  void SetDiffuseRadiation(const std::array<int, 24>& diffuse_radiation);

  [[nodiscard]] std::array<double, 24> GetDirectRadiation() const;

  void SetDirectRadiation(const std::array<int, 24>& direct_radiation);

  [[nodiscard]] std::array<double, 24> GetTemperature() const;

  void SetTemperature(const std::array<double, 24>& temperature);

  [[nodiscard]] std::array<double, 24> GetCloudCoverTotal() const;

  void SetCloudCoverTotal(const std::array<int, 24>& cloud_cover_total);

  [[nodiscard]] std::array<double, 24> GetWindSpeed() const;

  void SetWindSpeed(const std::array<double, 24>& wind_speed);

  /**
   * @brief Overloaded stream insertion operator to facilitate object output.
   * @param os The output stream.
   * @param obj The PvData object to be output.
   * @return The modified output stream.
   *
   * Allows PvData objects to be easily printed to an output stream.
   * It displays attributes such as sunrise and sunset times, radiation values,
   * temperature, cloud cover, and wind speed in a formatted manner. The data arrays
   * (diffuse_radiation_, direct_radiation_, temperature_, cloud_cover_total_, wind_speed_)
   * are printed using a helper lambda function (print_arr) to output each element
   * separated by spaces.
   */
  friend std::ostream& operator<<(std::ostream& os, const PvData& obj);
};

inline std::ostream& operator<<(std::ostream& os, const PvData& obj) {
  const auto print_arr = [&os]<typename Ty_, size_t Amt_>(
                             std::array<Ty_, Amt_> const& arr) -> void {
    std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<double>(os, " "));
  };
  os << "Sunrise Time: " << util::Date{obj.sunrise_time_}.ToString() << "\n"
     << "Sunset Time: " << util::Date{obj.sunset_time_}.ToString() << "\n"
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