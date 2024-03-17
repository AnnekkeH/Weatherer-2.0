#include "CropDataProcessor.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <span>
#include <unordered_map>

#include "util/ChunkOperator.hpp"

//TODO: Place with command line argument
#ifndef WEATHERER_PLANT_JSON_DATABASE_
#define WEATHERER_PLANT_JSON_DATABASE_
#define CROP_DATABASE_PATH \
  "C:\\Users\\OneCheetah\\VisualStudio\\Weatherer-2.0\\crop_database.json"
#define PLANT_HARDNESS_DATABASE_PATH \
  "C:\\Users\\OneCheetah\\VisualStudio\\Weatherer-2.0\\plant_zones.json"
#define ZIPCODE_DATABASE_PATH \
  "C:\\Users\\OneCheetah\\VisualStudio\\Weatherer-2.0\\zipcodes.json"
#endif

weatherer::CropDataProcessor::CropDataProcessor(
    util::LocationData const& location)
    : crop_data_(std::make_unique<nlohmann::json>(
          nlohmann::json::parse(std::ifstream{CROP_DATABASE_PATH}))),
      zipcode_databse_(std::make_unique<nlohmann::json>(
          nlohmann::json::parse(std::ifstream{ZIPCODE_DATABASE_PATH}))),
      plant_zones_(std::make_unique<nlohmann::json>(
          nlohmann::json::parse(std::ifstream{PLANT_HARDNESS_DATABASE_PATH}))) {

  auto plants = GetPlantsByZone(GetHardnessZone(location.second));
  const auto data = CollectData(plants);
  this->data_ = data;
  plantability_ = std::make_unique<std::unordered_map<std::string, bool>>(
      IsWeatherSuitable(GetWeatherData(location.first), data));
}

cpr::Response weatherer::CropDataProcessor::GetWeatherData(
    Coordinates const& coords) {
  cpr::Response response;
  cpr::Parameters prams{};
  prams.Add(cpr::Parameter{"longitude", std::to_string(coords.GetLongitude())});
  prams.Add(cpr::Parameter{"latitude", std::to_string(coords.GetLatitude())});
  prams.Add(cpr::Parameter{"hourly", "soil_temperature_18cm"});
  prams.Add(cpr::Parameter{"daily", "temperature_2m_min"});
  prams.Add(cpr::Parameter{"temperature_unit", "celsius"});
  prams.Add(cpr::Parameter{"timeformat", "unixtime"});
  prams.Add(cpr::Parameter{"timezone", "auto"});
  prams.Add(cpr::Parameter{"forecast_days", "1"});

  cpr::Response res = cpr::Get(cpr::Url{kApiUrl_}, prams);

  if (res.status_code != 200) {
    throw std::runtime_error("Failed to get weather data");
  }

  return res;
}

std::unordered_map<std::string, bool>
weatherer::CropDataProcessor::IsWeatherSuitable(cpr::Response const& res,
                                                CropCollectionPtr const& data) {
  using Json = nlohmann::json;

  auto json = Json::parse(res.text);
  double min_air_tmep =
      json.at("daily").at("temperature_2m_min").at(0).get<double>();
  const std::array<double, 23> soil_temps = json.at("hourly")
                                                .at("soil_temperature_18cm")
                                                .get<std::array<double, 23>>();
  auto min_soil_temp_index = std::ranges::distance(
      soil_temps.begin(),
      std::ranges::min_element(soil_temps.begin(), soil_temps.end()));

  std::unordered_map<std::string, bool> plantability = {};
  for (auto const& [crop_name, crop_data] : *data) {
    const bool is_soil_temp_suitable =
        crop_data->GetPrefSoilTemp() < soil_temps.at(min_soil_temp_index);
    const bool is_air_temp_suitable =
        crop_data->GetPrefAirTemp() < min_air_tmep;
    plantability.emplace(crop_name,
                         is_soil_temp_suitable && is_air_temp_suitable);
  }
  return plantability;
}

int weatherer::CropDataProcessor::GetHardnessZone(
    std::string const& zipcode) const {
  [[unlikely]] if (zipcode_databse_->empty()) {
    throw std::runtime_error("Zipcode database is empty");
  }
  std::cout << std::boolalpha;

  auto& zone = zipcode_databse_->at(zipcode).at("zone");
  [[unlikely]] if (zone.empty() || zone.is_null() ||
                   !zone.is_number_integer()) {
    throw std::runtime_error("Zipcode not found");
  }

  return zone.get<int>();
}

std::vector<std::string> weatherer::CropDataProcessor::GetPlantsByZone(
    const int zone) const {
  [[unlikely]] if (plant_zones_->empty()) {
    throw std::runtime_error("Plant zone database is empty");
  }

  auto const& plants = plant_zones_->at(std::to_string(zone)).at("plants");

  [[unlikely]] if (plants.empty() || plants.is_null() || !plants.is_string()) {
    throw std::runtime_error("Plants not found");
  }

  return util::SplitString(plants.get<std::string>(), std::string_view{";"});
}

weatherer::CropCollectionPtr weatherer::CropDataProcessor::CollectData(
    std::span<std::string> data) const {
  using namespace util;
  using Json = nlohmann::json;

  auto collection =
      std::make_shared<std::unordered_map<std::string, CropDataPtr>>();

  auto JsonIsNull = [](Json const& json) -> bool {
    return json.empty() || json.is_null();
  };

  for (std::string const& crop : data) {
    const CropDataPtr crop_data = std::make_shared<CropData>();
    const auto json = crop_data_->at(crop);
    auto const& species_name = json.at("Species Name");
    auto const& pref_light_level = json.at("Pref. Light Exposure");
    auto const& pref_soil_temp = json.at("Pref. Soil Temp C");
    auto const& pref_air_temp = json.at("Pref. Air Temp C");
    auto const& pref_soil_ph = json.at("Pref. Soil pH");
    auto const& fun_facts = json.at("Fun Facts");

    crop_data->SetName(crop);

    if (!JsonIsNull(species_name)) {
      crop_data->SetSpeciesName(species_name.get<std::string>());
    }
    if (!JsonIsNull(pref_light_level)) {
      crop_data->SetPrefLightLevel(pref_light_level.get<std::string>());
    }

    if (!JsonIsNull(pref_soil_temp) && pref_soil_temp.is_number_integer()) {
      crop_data->SetPrefSoilTemp(pref_soil_temp.get<int>());
    }

    if (!JsonIsNull(pref_air_temp) && pref_air_temp.is_number_integer()) {
      crop_data->SetPrefAirTemp(pref_air_temp.get<int>());
    }

    if (!JsonIsNull(pref_soil_ph) && pref_soil_ph.is_string()) {
      auto nums =
          SplitString(pref_soil_ph.get<std::string>(), std::string_view{"-"});
      crop_data->SetPrefSoilPh(
          NumericRange{std::stod(nums.at(0)), std::stod(nums.at(1))});
    }

    if (!JsonIsNull(fun_facts)) {
      crop_data->SetFunFacts(fun_facts.get<std::string>());
    }

    collection->emplace(crop, crop_data);
  }

  return collection;
}
