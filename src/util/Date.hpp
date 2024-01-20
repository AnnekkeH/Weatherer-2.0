#pragma once
#include <chrono>
#include <string>

namespace weatherer::util {

/**
 * @brief Utility class for handling date and time operations.
 *
 * The Date class encapsulates functionality related to date and time manipulation. It provides methods for parsing date and time strings,
 * calculating time spans, and converting between different representations. The class includes static methods for common operations and
 * supports various functionalities such as addition, subtraction, resetting to midnight, and obtaining the current local time.
 */
class Date {
 private:
  std::time_t time_{};
  /**
   * @brief Converts a date string to UNIX time.
   *
   * Takes a date string in the format "%Y-%m-%dT%H:%M" (ISO 8601) and converts it to UNIX time.
   * The conversion involves parsing the input string and creating a tm structure, followed by using std::mktime to obtain UNIX time.
   *
   * @param date A string representation of a date.
   * @return The corresponding UNIX time.
   */
  static std::time_t ConvertToUnixTime(std::string const& date);

 public:
  explicit Date();
  explicit Date(std::time_t unix_time);
  explicit Date(std::string const& date);
  Date(const Date& other) = default;
  Date(Date&& other) noexcept;
  Date& operator=(const Date& other);
  Date& operator=(Date&& other) noexcept;
  ~Date() = default;

  std::strong_ordering operator<=>(const Date& rhs) const = default;

  /**
   * @brief Constant for the number of seconds per day.
   *
   * Defines the number of seconds in a day (24 hours).
   */
  static constexpr std::time_t kSecondsPerDay = 86400;

  /**
   * @brief Constant for the number of seconds per hour.
   *
   * Defines the number of seconds in an hour.
   */
  static constexpr std::time_t kSecondsPerHour = 3600;

  /**
   * @brief Constant for the number of seconds per minute.
   *
   * Defines the number of seconds in a minute.
   */
  static constexpr std::time_t kSecondsPerMinute = 60;

  /**
   * @brief Overloaded addition operator for adding seconds to a Date object.
   *
   * Adds a specified number of seconds to a Date object.
   *
   * @param time The Date object.
   * @param seconds The number of seconds to add.
   * @return The resulting Date object.
   */
  friend Date operator+(Date const& time, std::time_t const& seconds) noexcept;
  /**
   * @brief Overloaded subtraction operator for subtracting seconds from a Date object.
   *
   * Subtracts a specified number of seconds from a Date object.
   *
   * @param time The Date object.
   * @param seconds The number of seconds to subtract.
   * @return The resulting Date object.
   */
  friend Date operator-(Date const& time, std::time_t const& seconds) noexcept;
  /**
   * @brief Overloaded addition operator for adding two Date objects.
   *
   * Adds two Date objects together, resulting in the sum of their internal UNIX times.
   *
   * @param lhs The first Date object.
   * @param rhs The second Date object.
   * @return The sum of the two Date objects in UNIX time.
   */
  friend std::time_t operator+(Date const& lhs, Date const& rhs) noexcept;
  /**
   * @brief Overloaded subtraction operator for subtracting two Date objects.
   *
   * Subtracts one Date object from another, resulting in the difference of their internal UNIX times.
   *
   * @param lhs The minuend Date object.
   * @param rhs The subtrahend Date object.
   * @return The difference between the two Date objects in UNIX time.
   */
  friend std::time_t operator-(Date const& lhs, Date const& rhs) noexcept;
  /**
   * @brief Overloaded stream insertion operator for outputting a Date object to an ostream.
   *
   * Outputs a Date object to an ostream in the format "%Y-%m-%dT%H:%M:%S" (ISO 8601).
   *
   * @param os The output stream.
   * @param obj The Date object to output.
   * @return The output stream.
   */
  friend std::ostream& operator<<(std::ostream& os, const Date& obj);

  /**
   * @brief Resets the time of the Date object to midnight.
   *
   * Set the time component of the Date object to midnight (00:00:00).
   */
  void ResetToMidnight();
  /**
   * @brief Gets the UNIX time representation of the Date object.
   *
   * Returns the internal UNIX time representation of the Date object.
   *
   * @return The UNIX time.
   */
  [[nodiscard]] std::time_t GetTime() const;
  /**
   * @brief Gets a string representation of the Date object.
   *
   * Returns a string representation of the Date object in the format "%Y-%m-%dT%H:%M:%S" (ISO 8601).
   *
   * @return The string representation of the Date object.
   */
  [[nodiscard]] std::string ToString() const;
  /**
   * @brief Gets the current local time as a tm structure.
   *
   * Retrieves the current local time and returns it as a tm structure.
   *
   * @return The tm structure representing the current local time.
   */
  [[nodiscard]] std::tm GetCurrentLocalTime() const;

  [[nodiscard]] std::string StripTime() const;
};

inline Date operator+(Date const& time, std::time_t const& seconds) noexcept {
  return Date{time.time_ + seconds};
}

inline Date operator-(Date const& time, std::time_t const& seconds) noexcept {
  return Date{time.time_ - seconds};
}

inline std::time_t operator+(Date const& lhs, Date const& rhs) noexcept {
  return lhs.time_ + rhs.time_;
}

inline std::time_t operator-(Date const& lhs, Date const& rhs) noexcept {
  return lhs.time_ - rhs.time_;
}

inline std::ostream& operator<<(std::ostream& os, const Date& obj) {
  std::tm const tm = obj.GetCurrentLocalTime();
  os << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
  return os;
}

/**
 * @brief Represents a time frame defined by start and end dates.
 *
 * The TimeFrame struct encapsulates information about a specific time period,
 * specified by start and end dates.
 */
class TimeFrame {
 private:
  Date start_date_;
  Date end_date_;

 public:
  explicit TimeFrame(std::string const& start_date,
                     std::string const& end_date);
  explicit TimeFrame(Date start_date, Date end_date);
  explicit TimeFrame(std::string const& date);
  explicit TimeFrame(Date const& date);
  ~TimeFrame() = default;
  TimeFrame(const TimeFrame& other);
  TimeFrame(TimeFrame&& other) noexcept;
  TimeFrame& operator=(const TimeFrame& other);
  TimeFrame& operator=(TimeFrame&& other) noexcept;
  /**
   * @return The start date of the TimeFrame.
   */
  [[nodiscard]] Date GetStartDate() const;
  /**
   * @return The end date of the TimeFrame.
   */
  [[nodiscard]] Date GetEndDate() const;
};
}  // namespace weatherer::util
