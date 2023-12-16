#pragma once
#include <string>
#include <chrono>

namespace weatherer::util {

	struct DateTimeInfo {
		const std::string time_;
		const std::time_t val_;

		DateTimeInfo(std::string time, const std::time_t val) : time_{std::move(time)}, val_{val} {
		}

		~DateTimeInfo() = default;
	};

	class Time {
	public:
		Time() = delete;
		~Time() = delete;

		static DateTimeInfo get_current_date();
		static DateTimeInfo get_past_date(std::uint32_t days);
		static DateTimeInfo get_future_date(std::uint32_t days);
		static int difference_in_days(const std::string& first, const std::string& second);
		static std::chrono::system_clock::time_point parse_date(const std::string& date);
	};
}
