#pragma once
#include <algorithm>
#include <numeric>

namespace weatherer::util {
class Statistics {
 public:
  Statistics() = delete;
  ~Statistics() = delete;

  template <typename Ty_, typename = std::enable_if_t<
                              std::is_arithmetic_v<typename Ty_::value_type>>>
  [[nodiscard]] static constexpr double Mean(Ty_ const& container) {
    return std::accumulate(container.begin(), container.end(), 0.0) /
           container.size();
  }

  template <typename Ty_, typename Vt_ = typename Ty_::value_type,
            typename = std::enable_if_t<std::is_arithmetic_v<Vt_>>>
  [[nodiscard]] static constexpr Vt_ Median(Ty_ const& container) {
    Ty_ sorted = container;
    std::sort(sorted.begin(), sorted.end());

    if (sorted.size() % 2 == 0) {
      auto const middle = sorted.size() / 2;
      return (sorted.at(middle - 1) + sorted.at(middle)) / 2;
    }
    return sorted.at(sorted.size() / 2);
  }
};

}  // namespace weatherer::util
