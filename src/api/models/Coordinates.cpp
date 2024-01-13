#include "Coordinates.hpp"

weatherer::Coordinates::Coordinates(const double latitude, const double longitude)
    : latitude_(latitude), longitude_(longitude) {}

weatherer::Coordinates::~Coordinates() = default;

weatherer::Coordinates::Coordinates(const Coordinates& other) = default;

weatherer::Coordinates::Coordinates(Coordinates&& other) noexcept
    : latitude_(other.latitude_), longitude_(other.longitude_) {}

weatherer::Coordinates& weatherer::Coordinates::operator=(const Coordinates& other) {
  if (this == &other) {
    return *this;
  }
  latitude_ = other.latitude_;
  longitude_ = other.longitude_;
  return *this;
}

weatherer::Coordinates& weatherer::Coordinates::operator=(Coordinates&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  latitude_ = other.latitude_;
  longitude_ = other.longitude_;
  return *this;
}

double weatherer::Coordinates::GetLatitude() const {
  return latitude_;
}

double weatherer::Coordinates::GetLongitude() const {
  return longitude_;
}