#include "Date.hpp"

#include <utility>

weatherer::util::Date::Date() {
  const std::chrono::time_point<std::chrono::system_clock> current_time =
      std::chrono::system_clock::now();

  // Convert the current time point to a time_t object
  std::time_t time_t = std::chrono::system_clock::to_time_t(current_time);

  // Get the local time struct
  std::tm local_time{};
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  localtime_s(&local_time, &time_t);
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
  localtime_r(&time_t, &local_time);
#endif

  // Convert the modified time struct back to time_t
  this->time_ = std::mktime(&local_time);
};

weatherer::util::Date::Date(std::time_t const unix_time) : time_(unix_time) {}

weatherer::util::Date::Date(std::string const& date)
    : time_(ConvertToUnixTime(date)) {}

weatherer::util::Date::Date(Date&& other) noexcept : time_(other.time_) {}

weatherer::util::Date& weatherer::util::Date::operator=(const Date& other) {
  if (this == &other)
    return *this;
  time_ = other.time_;
  return *this;
}

weatherer::util::Date& weatherer::util::Date::operator=(Date&& other) noexcept {
    if (this == &other)
        return *this;
    time_ = other.time_;
    return *this;
}

std::tm weatherer::util::Date::GetCurrentLocalTime() const {
  std::tm local_time{};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  localtime_s(&local_time, &this->time_);
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
  localtime_r(&date.time_, &local_time);
#endif
  return local_time;
}

void weatherer::util::Date::ResetToMidnight() {
  std::tm tm = this->GetCurrentLocalTime();
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  this->time_ = std::mktime(&tm);
}

std::time_t weatherer::util::Date::ConvertToUnixTime(std::string const& date) {
  std::tm time{};
  std::istringstream stringstream(date);
  stringstream >> std::get_time(&time, "%Y-%m-%dT%H:%M");
  return std::mktime(&time);
}

std::time_t weatherer::util::Date::GetTime() const {
  return this->time_;
}

std::string weatherer::util::Date::ToString() const {
  std::ostringstream oss;
  oss << *this;
  return oss.str();
}

std::string weatherer::util::Date::StripTime() const {
  std::ostringstream oss{};
  std::tm const tm = this->GetCurrentLocalTime();
  oss << std::put_time(&tm, "%Y-%m-%d");
  return oss.str();
}

weatherer::util::TimeFrame::TimeFrame(std::string const& start_date,
                                      std::string const& end_date)
    : start_date_(Date{start_date}), end_date_(Date{end_date}) {}

weatherer::util::TimeFrame::TimeFrame(Date start_date,
                                      Date end_date)
    : start_date_(std::move(start_date)), end_date_(std::move(end_date)) {}

weatherer::util::TimeFrame::TimeFrame(std::string const& date)
    : start_date_(Date{date}), end_date_(Date{date}) {}

weatherer::util::TimeFrame::TimeFrame(Date const& date)
    : start_date_(date), end_date_(date) {}

weatherer::util::TimeFrame::TimeFrame(const TimeFrame& other) {}

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

weatherer::util::Date weatherer::util::TimeFrame::GetStartDate() const {
  return start_date_;
}

weatherer::util::Date weatherer::util::TimeFrame::GetEndDate() const {
    return end_date_;
}
