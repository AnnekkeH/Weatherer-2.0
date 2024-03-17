#pragma once
#include <numeric>
#include <span>
#include <type_traits>

namespace weatherer::util {
class Statistics {
public:
  Statistics() = delete;
  ~Statistics() = delete;

  template <typename Ty_, size_t Amt_,
            typename = std::enable_if<std::is_arithmetic_v<Ty_>>>
  [[nodiscard]] static constexpr double Mean(std::span<Ty_, Amt_> data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
  }
};
}  // namespace weatherer::util
