#pragma once
#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>
#include <ranges>
#include <vector>
#include <range/v3/view/chunk.hpp>

namespace weatherer::util {
	class Streams {
	public:
		Streams() = delete;
		~Streams() = delete;

		template <typename T, size_t N>
		static void os_array(std::ostream& os, const std::array<T, N>& arr, const std::string_view label) {
			os << label << ": ";
			std::copy(arr.begin(), arr.end(), std::ostream_iterator<T>(os, " "));
			os << "\n";
		}

		template <typename Ty_, std::size_t N>
		static std::vector<std::array<Ty_, N>> split_array(const std::vector<Ty_>& data, std::size_t number) {
			if (data.size() % number != 0) {
				throw std::out_of_range("Input data size is not a multiple of number");
			}

			std::vector<std::array<Ty_, N>> result{};
			result.reserve(data.size() / number);
			for (decltype(auto) chunk : data | ranges::views::chunk(N)) {
				std::array<Ty_, N> chunked{};

				std::copy(chunk.begin(), chunk.end(), chunked.begin());
				result.push_back(chunked);
			}

			return result;
		}
	};
}
