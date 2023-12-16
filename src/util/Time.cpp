#include "Time.hpp"

#include <array>
#include <cmath>

weatherer::util::DateTimeInfo weatherer::util::Time::get_current_date() {
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::chrono::time_point date = std::chrono::floor<std::chrono::days>(now);
	std::chrono::time_point midnight = std::chrono::time_point_cast<std::chrono::seconds>(date);

	std::time_t time = std::chrono::system_clock::to_time_t(midnight);
	std::tm tm;

	localtime_s(&tm, &time);

	std::array<char, 80> buffer;
	if (strftime(buffer.data(), 80, "%Y-%m-%d", &tm) <= 0) {
		throw std::runtime_error("An error has occurred whilst formatting the date.");
	}
	return DateTimeInfo{{buffer.data()}, time};
}

weatherer::util::DateTimeInfo weatherer::util::Time::get_past_date(std::uint32_t days) {
	if (days == 0) {
		return get_current_date();
	}
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::chrono::time_point date = std::chrono::floor<std::chrono::days>(now);
	std::chrono::time_point midnight = std::chrono::time_point_cast<std::chrono::seconds>(
		date - std::chrono::days(days));

	std::time_t time = std::chrono::system_clock::to_time_t(midnight);
	std::tm tm;

	localtime_s(&tm, &time);

	std::array<char, 80> buffer;
	if (!strftime(buffer.data(), 80, "%Y-%m-%d", &tm) > 0) {
		throw std::runtime_error("An error has occurred whilst formatting the date.");
	}
	return DateTimeInfo{{buffer.data()}, time};
}

weatherer::util::DateTimeInfo weatherer::util::Time::get_future_date(std::uint32_t days) {
	if (days == 0) {
		return get_current_date();
	}
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::chrono::time_point date = std::chrono::floor<std::chrono::days>(now);
	std::chrono::time_point midnight = std::chrono::time_point_cast<std::chrono::seconds>(
		date + std::chrono::days(days));

	std::time_t time = std::chrono::system_clock::to_time_t(midnight);
	std::tm tm;

	localtime_s(&tm, &time);

	std::array<char, 80> buffer;
	if (!strftime(buffer.data(), 80, "%Y-%m-%d", &tm) > 0) {
		throw std::runtime_error("An error has occurred whilst formatting the date.");
	}
	return DateTimeInfo{{buffer.data()}, time};
}

std::chrono::system_clock::time_point weatherer::util::Time::parse_date(const std::string& date) {
	std::tm tm{};
	std::istringstream ss{date};
	ss >> std::get_time(&tm, "%Y-%m-%d");
	return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

int weatherer::util::Time::difference_in_days(const std::string& first, const std::string& second) {
	const std::chrono::system_clock::time_point start_date = parse_date(first);
	const std::chrono::system_clock::time_point end_date = parse_date(second);
	const auto duration = start_date - end_date;
	return std::abs(std::chrono::duration_cast<std::chrono::days>(duration).count());
}
