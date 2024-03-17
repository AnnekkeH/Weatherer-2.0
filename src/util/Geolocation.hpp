#pragma once

#include <string>
#include <utility>
#include "api/models/Coordinates.hpp"

namespace weatherer::util {
using LocationData = std::pair<Coordinates, std::string>;

class Geolocation {
 public:
  Geolocation() = delete;
  ~Geolocation() = delete;

  [[nodiscard]] static LocationData GetLocationData(const std::string& address);
};
}  // namespace weatherer::util