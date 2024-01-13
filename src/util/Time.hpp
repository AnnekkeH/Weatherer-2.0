#pragma once
#include <chrono>
#include <string>

namespace weatherer::util {

/**
 * @brief Represents date and time information.
 *
 * The DateTimeInfo struct encapsulates information about a specific date and time,
 * represented by a string and a std::time_t value.
 */
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

/**
 * @brief Represents a time frame defined by start and end dates.
 *
 * The TimeFrame struct encapsulates information about a specific time period,
 * specified by start and end dates.
 */
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

/**
 * @brief Utility class for handling date and time operations.
 *
 * The Time class provides static methods for working with date and time,
 * including parsing date and time strings, calculating time spans, and converting
 * between different representations.
 */
class Time {
 public:
  // Prevent instantiation of the Time class.
  Time() = delete;
  ~Time() = delete;

/**
 * @brief Get the current date.
 * @return DateTimeInfo struct containing the formatted date and time in seconds.
 * @throws std::runtime_error if there is an error formatting the date.
 *
 * Retrieve the current date and time from the system clock, and format it as
 * "%Y-%m-%d". The result is encapsulated in a DateTimeInfo struct, which includes
 * the formatted date and the corresponding time in seconds since the epoch.
 */
  [[nodiscard]] static DateTimeInfo GetCurrentDate();

/**
 * @brief Calculate the date from a specified number of days ago.
 * @param days The number of days in the past.
 * @return DateTimeInfo struct containing the formatted date and time in seconds.
 * @throws std::runtime_error if there is an error formatting the date.
 *
 * Calculate the date from a specified number of days ago, and format it as
 * "%Y-%m-%d". The result is encapsulated in a DateTimeInfo struct, which includes
 * the formatted date and the corresponding time in seconds since the epoch.
 */
  [[nodiscard]] static DateTimeInfo GetPastDate(const std::uint32_t days);

/**
 * @brief Get the date from a specified number of days in the future.
 * @param start_date The starting date for calculation in the format "%Y-%m-%d".
 * @param days The number of days in the future from the start date.
 * @return DateTimeInfo struct containing the formatted date and time in seconds.
 * @throws std::runtime_error if there is an error formatting the date.
 *
 * Calculates the date from a specified number of days in the future
 * relative to a given start date. The result is encapsulated in a DateTimeInfo struct,
 * which includes the formatted date and the corresponding time in seconds since the epoch.
 */
  [[nodiscard]] static DateTimeInfo GetFutureDate(const std::string& start_date,
                                                  std::uint32_t days);

/**
 * @brief Calculate the time span between two dates.
 * @param first The first date string (format: "%Y-%m-%d").
 * @param second The second date string (format: "%Y-%m-%d").
 * @return The absolute time span in days between the two dates.
 *
 * Calculates the time span (difference) in days between two date strings.
 * The dates are expected to be in the format "%Y-%m-%d".
 */
  [[nodiscard]] static int CalculateTimeSpan(const std::string& first,
                                            const std::string& second);

/**
 * @brief Parse a date string into a time_point.
 * @param date The date string to be parsed (format: "%Y-%m-%d").
 * @return std::chrono::system_clock::time_point representing the parsed date.
 *
 * Parses a date string in the format "%Y-%m-%d" and returns
 * the corresponding std::chrono::system_clock::time_point.
 * Use this function when only the date information is present.
 */
  [[nodiscard]] static std::chrono::system_clock::time_point ParseDate(
      const std::string& date);

/**
 * @brief Parse a time string into a time_point.
 * @param time The time string to be parsed (format: "%Y-%m-%dT%H:%M").
 * @return std::chrono::system_clock::time_point representing the parsed time.
 * @throws std::runtime_error if there is an error parsing the time string.
 *
 * Parses a time string in the format "%Y-%m-%dT%H:%M" and returns
 * the corresponding std::chrono::system_clock::time_point.
 * Use this function when both date and time information are present.
 */
  [[nodiscard]] static std::chrono::system_clock::time_point ParseTime(
      const std::string& time);

/**
 * @brief Convert a time_point to std::tm structure.
 * @param time_point The time_point to be converted.
 * @return std::tm structure representing the date and time.
 *
 * Convert a std::chrono::system_clock::time_point to a std::tm
 * structure for further manipulation of date and time components.
 * Use this function when detailed access to individual components
 * (such as year, month, day, hour, minute) is required.
 */
  [[nodiscard]] static std::tm ConvertTimePointToTm(
      const std::chrono::system_clock::time_point& time_point);
};
}  // namespace weatherer::util
