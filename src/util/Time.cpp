#include "Time.hpp"

#include <array>
#include <cmath>

[[nodiscard]] weatherer::util::DateTimeInfo
weatherer::util::Time::GetCurrentDate() {
  const std::chrono::time_point now = std::chrono::system_clock::now();
  const std::chrono::time_point date =
      std::chrono::floor<std::chrono::days>(now);
  const std::chrono::time_point midnight =
      std::chrono::time_point_cast<std::chrono::seconds>(date);

  const std::time_t time = std::chrono::system_clock::to_time_t(midnight);
  std::tm tm{};

  localtime_s(&tm, &time);

  std::array<char, 80> buffer{};
  if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
    throw std::runtime_error(
        "An error has occurred whilst formatting the date.");
  }
  return DateTimeInfo{{buffer.data()}, time};
}

[[nodiscard]] weatherer::util::DateTimeInfo weatherer::util::Time::GetPastDate(
    std::uint32_t days) {
  if (days == 0) {
    return GetCurrentDate();
  }
  const std::chrono::time_point now = std::chrono::system_clock::now();
  const std::chrono::time_point date =
      std::chrono::floor<std::chrono::days>(now);
  const std::chrono::time_point midnight =
      std::chrono::time_point_cast<std::chrono::seconds>(
          date - std::chrono::days(days));

  std::time_t time = std::chrono::system_clock::to_time_t(midnight);
  std::tm tm{};

  localtime_s(&tm, &time);

  std::array<char, 80> buffer{};
  if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
    throw std::runtime_error(
        "An error has occurred whilst formatting the date.");
  }
  return DateTimeInfo{{buffer.data()}, time};
}

weatherer::util::DateTimeInfo weatherer::util::Time::GetFutureDate(
    const std::string& start_date, std::uint32_t days) {
  const std::chrono::time_point init_date = ParseDate(start_date);
  const std::chrono::time_point date =
      std::chrono::floor<std::chrono::days>(init_date);
  const std::chrono::time_point midnight =
      std::chrono::time_point_cast<std::chrono::seconds>(
          date + std::chrono::days(days));

  const std::time_t time = std::chrono::system_clock::to_time_t(midnight);
  std::tm tm;

  localtime_s(&tm, &time);

  std::array<char, 80> buffer{};
  if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
    throw std::runtime_error(
        "An error has occurred whilst formatting the date.");
  }
  return DateTimeInfo{{buffer.data()}, time};
}

[[nodiscard]] std::chrono::system_clock::time_point
weatherer::util::Time::ParseDate(const std::string& date) {
  std::tm tm{};
  std::istringstream ss{date};
  ss >> std::get_time(&tm, "%Y-%m-%d");
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

[[nodiscard]] int weatherer::util::Time::DifferenceInDays(
    const std::string& first, const std::string& second) {
  const std::chrono::system_clock::time_point start_date = ParseDate(first);
  const std::chrono::system_clock::time_point end_date = ParseDate(second);
  const auto duration = start_date - end_date;
  return std::abs(
      std::chrono::duration_cast<std::chrono::days>(duration).count());
}

std::chrono::system_clock::time_point weatherer::util::Time::ParseTime(
    const std::string& time) {
  std::tm time_tm = {};
  std::istringstream ss(time);
  ss >> std::get_time(&time_tm, "%Y-%m-%dT%H:%M");

  if (ss.fail()) {
    throw std::runtime_error("Failed to parse the first time string.");
  }

  const std::chrono::system_clock::time_point time_point =
      std::chrono::system_clock::from_time_t(std::mktime(&time_tm));

  return time_point;
}

std::tm weatherer::util::Time::ConvertTimePointToTm(
    const std::chrono::system_clock::time_point& time_point) {
  const std::time_t time = std::chrono::system_clock::to_time_t(time_point);
  std::tm time_info;
  localtime_s(
      &time_info,
      &time);  // Use localtime_s for safer conversion, especially on Windows

  return time_info;
}
