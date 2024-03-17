#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include <cpr/cpr.h>

#include "models/Coordinates.hpp"
#include "models/CropData.hpp"
#include "util/Geolocation.hpp"

namespace weatherer {
using CropDataPtr = std::shared_ptr<CropData>;
using CropCollectionPtr =
    std::shared_ptr<std::unordered_map<std::string, CropDataPtr>>;

class CropDataProcessor {
 private:
  std::unique_ptr<nlohmann::json> crop_data_;
  std::unique_ptr<nlohmann::json> zipcode_databse_;
  std::unique_ptr<nlohmann::json> plant_zones_;
  CropCollectionPtr data_;
  std::unique_ptr<std::unordered_map<std::string, bool>> plantability_;

  static constexpr std::string_view kApiUrl_{
      "https://api.open-meteo.com/v1/forecast"};

 private:
  [[nodiscard]] static cpr::Response GetWeatherData(Coordinates const& coords);

  [[nodiscard]] static std::unordered_map<std::string, bool> IsWeatherSuitable(
      cpr::Response const& res, CropCollectionPtr const& data);

 public:
  explicit CropDataProcessor(util::LocationData const& location);

  [[nodiscard]] int GetHardnessZone(std::string const& zipcode) const;
  [[nodiscard]] std::vector<std::string> GetPlantsByZone(const int zone) const;
  [[nodiscard]] CropCollectionPtr CollectData(
      std::span<std::string> data) const;


  [[nodiscard]] CropCollectionPtr GetData() const { return data_; }
};
}  // namespace weatherer