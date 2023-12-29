#pragma once
#include <chrono>
#include <string>

namespace weatherer::util {

struct DateTimeInfo {
  std::string time_;
  std::time_t val_;
};

struct TimeFrame {
  std::string start_date_;
  std::string end_date_;
};

class Time {
 public:
  Time() = delete;
  ~Time() = delete;

  [[nodiscard]] static DateTimeInfo GetCurrentDate();
  [[nodiscard]] static DateTimeInfo GetPastDate(std::uint32_t days);
  [[nodiscard]] static DateTimeInfo GetFutureDate(const std::string& start_date,
                                                  std::uint32_t days);
  [[nodiscard]] static int DifferenceInDays(const std::string& first,
                                            const std::string& second);
  [[nodiscard]] static std::chrono::system_clock::time_point ParseDate(
      const std::string& date);
  [[nodiscard]] static std::chrono::system_clock::time_point ParseTime(
      const std::string& time);
  [[nodiscard]] static std::tm ConvertTimePointToTm(
      const std::chrono::system_clock::time_point& time_point);
};
}  // namespace weatherer::util
