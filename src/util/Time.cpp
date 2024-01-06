#include "Time.hpp"

#include <array>
#include <cmath>
#include <utility>

weatherer::util::DateTimeInfo::DateTimeInfo(std::string time,
                                            const std::time_t val)
    : time_(std::move(time)), val_(val) {}

weatherer::util::DateTimeInfo::DateTimeInfo(const DateTimeInfo& other) =
    default;

weatherer::util::DateTimeInfo::DateTimeInfo(DateTimeInfo&& other) noexcept
    : time_(std::move(other.time_)), val_(other.val_) {}

weatherer::util::DateTimeInfo& weatherer::util::DateTimeInfo::operator=(
    const DateTimeInfo& other) {
  if (this == &other)
    return *this;
  time_ = other.time_;
  val_ = other.val_;
  return *this;
}

weatherer::util::DateTimeInfo& weatherer::util::DateTimeInfo::operator=(
    DateTimeInfo&& other) noexcept {
  if (this == &other)
    return *this;
  time_ = std::move(other.time_);
  val_ = other.val_;
  return *this;
}

weatherer::util::TimeFrame::TimeFrame(std::string start_date,
                                      std::string end_date)
    : start_date_(std::move(start_date)), end_date_(std::move(end_date)) {}

weatherer::util::TimeFrame::TimeFrame(std::string const& date)
    : start_date_(date), end_date_(date) {}

weatherer::util::TimeFrame::TimeFrame(const TimeFrame& other) = default;

weatherer::util::TimeFrame::TimeFrame(TimeFrame&& other) noexcept
    : start_date_(std::move(other.start_date_)),
      end_date_(std::move(other.end_date_)) {}

weatherer::util::TimeFrame& weatherer::util::TimeFrame::operator=(
    const TimeFrame& other) {
  if (this == &other)
    return *this;
  start_date_ = other.start_date_;
  end_date_ = other.end_date_;
  return *this;
}

weatherer::util::TimeFrame& weatherer::util::TimeFrame::operator=(
    TimeFrame&& other) noexcept {
  if (this == &other)
    return *this;
  start_date_ = std::move(other.start_date_);
  end_date_ = std::move(other.end_date_);
  return *this;
}

[[nodiscard]] weatherer::util::DateTimeInfo
weatherer::util::Time::GetCurrentDate() {
  // Retrieve the current time
  const std::chrono::time_point now = std::chrono::system_clock::now();
// Truncate the time to get the current date
  const std::chrono::time_point date =
      std::chrono::floor<std::chrono::days>(now);
  // Cast the date to seconds and adjust for local time
  const std::chrono::time_point midnight =
      std::chrono::time_point_cast<std::chrono::seconds>(date);

  const std::time_t time = std::chrono::system_clock::to_time_t(midnight);
  std::tm tm{};

  // Convert the time to a local tm structure
  localtime_s(&tm, &time);

  std::array<char, 80> buffer{};
  // Format the date using strftime
  if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
    // Throw an error if formatting fails
    throw std::runtime_error(
        "An error has occurred whilst formatting the date.");
  }
  // Return the formatted date and time information
  return DateTimeInfo{{buffer.data()}, time};
}

[[nodiscard]] weatherer::util::DateTimeInfo weatherer::util::Time::GetPastDate(
    const std::uint32_t days) {
  // If the input days is 0, return the current date
  if (days == 0) {
    return GetCurrentDate();
  }
  // Retrieve the current time
  const std::chrono::time_point now = std::chrono::system_clock::now();
  // Truncate the time to get the current date
  const std::chrono::time_point date =
      std::chrono::floor<std::chrono::days>(now);
  // Calculate the date and time for the specified number of days ago
  const std::chrono::time_point midnight =
      std::chrono::time_point_cast<std::chrono::seconds>(
          date - std::chrono::days(days));

  const std::time_t time = std::chrono::system_clock::to_time_t(midnight);
  std::tm tm{};

  // Convert the time to a local tm structure
  localtime_s(&tm, &time);

  std::array<char, 80> buffer{};
  // Format the date using strftime
  if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
    // Throw an error if formatting fails
    throw std::runtime_error(
        "An error has occurred whilst formatting the date.");
  }
  // Return the formatted date and time information
  return DateTimeInfo{{buffer.data()}, time};
}

weatherer::util::DateTimeInfo weatherer::util::Time::GetFutureDate(
    const std::string& start_date, const std::uint32_t days) {
  // Parse the input start date into a time_point
  const std::chrono::time_point init_date = ParseDate(start_date);
  // Truncate the time to get the date
  const std::chrono::time_point date =
      std::chrono::floor<std::chrono::days>(init_date);
  // Calculate the date and time for the specified number of days in the future
  const std::chrono::time_point midnight =
      std::chrono::time_point_cast<std::chrono::seconds>(
          date + std::chrono::days(days));

  const std::time_t time = std::chrono::system_clock::to_time_t(midnight);
  std::tm tm{};

  // Convert the time to a local tm structure
  localtime_s(&tm, &time);

  std::array<char, 80> buffer{};
  // Format the date using strftime
  if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
    // Throw an error if formatting fails
    throw std::runtime_error(
        "An error has occurred whilst formatting the date.");
  }
  // Return the formatted date and time information
  return DateTimeInfo{{buffer.data()}, time};
}

[[nodiscard]] std::chrono::system_clock::time_point
weatherer::util::Time::ParseDate(const std::string& date) {
  std::tm tm{};
  std::istringstream ss{date};
  // Use get_time to parse the date string
  ss >> std::get_time(&tm, "%Y-%m-%d");
  // Convert the tm structure to a time_point
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

[[nodiscard]] int weatherer::util::Time::CalculateTimeSpan(
    const std::string& first, const std::string& second) {
  // Parse the date strings into time_points
  const std::chrono::system_clock::time_point start_date = ParseDate(first);
  const std::chrono::system_clock::time_point end_date = ParseDate(second);
  // Calculate the duration between the two time_points
  const auto duration = start_date - end_date;
  // Return the absolute time span in days
  return std::abs(
      std::chrono::duration_cast<std::chrono::days>(duration).count());
}

std::chrono::system_clock::time_point weatherer::util::Time::ParseTime(
    const std::string& time) {
  // Initialize a tm structure to store the parsed time components
  std::tm time_tm{};
  // Parse the time string using get_time
  std::istringstream ss(time);
  ss >> std::get_time(&time_tm, "%Y-%m-%dT%H:%M");

  // Check if parsing was successful
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse the first time string.");
  }

  // Convert the tm structure to a time_point
  const std::chrono::system_clock::time_point time_point =
      std::chrono::system_clock::from_time_t(std::mktime(&time_tm));

  return time_point;
}

std::tm weatherer::util::Time::ConvertTimePointToTm(
  const std::chrono::system_clock::time_point& time_point) {
  // Convert the time_point to time_t
  const std::time_t time = std::chrono::system_clock::to_time_t(time_point);
  // Initialize a tm structure to store the converted time components
  std::tm time_info{};
  // Convert the time to a local tm structure
  localtime_s(
      &time_info,
      &time);

  return time_info;
}
