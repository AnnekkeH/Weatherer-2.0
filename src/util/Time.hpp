#pragma once
#include <chrono>
#include <string>

namespace weatherer::util {

struct DateTimeInfo {
  std::string time_;
  std::time_t val_{};

  explicit DateTimeInfo(std::string time, const std::time_t val);
  ~DateTimeInfo() = default;
  DateTimeInfo(const DateTimeInfo& other);
  DateTimeInfo(DateTimeInfo&& other) noexcept;
  DateTimeInfo& operator=(const DateTimeInfo& other);
  DateTimeInfo& operator=(DateTimeInfo&& other) noexcept;
};

struct TimeFrame {
  std::string start_date_;
  std::string end_date_;

  explicit TimeFrame(std::string start_date, std::string end_date);
  explicit TimeFrame(std::string const& date);
  ~TimeFrame() = default;
  TimeFrame(const TimeFrame& other);
  TimeFrame(TimeFrame&& other) noexcept;
  TimeFrame& operator=(const TimeFrame& other);
  TimeFrame& operator=(TimeFrame&& other) noexcept;
};

class Time {
 public:
  // Prevent instantiation of the Time class.
  Time() = delete;
  ~Time() = delete;

  [[nodiscard]] static DateTimeInfo GetCurrentDate();
  [[nodiscard]] static DateTimeInfo GetPastDate(const std::uint32_t days);
  [[nodiscard]] static DateTimeInfo GetFutureDate(const std::string& start_date,
                                                  std::uint32_t days);
  [[nodiscard]] static int CalculateTimeSpan(const std::string& first,
                                            const std::string& second);
  [[nodiscard]] static std::chrono::system_clock::time_point ParseDate(
      const std::string& date);
  [[nodiscard]] static std::chrono::system_clock::time_point ParseTime(
      const std::string& time);
  [[nodiscard]] static std::tm ConvertTimePointToTm(
      const std::chrono::system_clock::time_point& time_point);
};
}  // namespace weatherer::util
