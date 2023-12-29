#pragma once
#include <algorithm>
#include <array>
#include <ranges>
#include <vector>

namespace weatherer::util {
template <typename Ty_, std::size_t Amt_>
static std::vector<std::array<Ty_, Amt_>> ChunkVector(const std::vector<Ty_>& data,
                                                   std::size_t number) {
  if (data.size() % number != 0) {
    throw std::out_of_range("Input data size is not a multiple of number");
  }

  if (data.size() == 0) {
    return {};
  }

  std::vector<std::array<Ty_, Amt_>> result{};
  result.reserve(data.size() / number);
  for (auto const& chunk : data | std::ranges::views::chunk(Amt_)) {
    std::array<Ty_, Amt_> chunked{};

    std::copy(chunk.begin(), chunk.end(), chunked.begin());
    result.push_back(chunked);
  }

  return result;
}
};  // namespace weatherer::util
