#include "Geolocation.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <stdexcept>

weatherer::util::LocationData
weatherer::util::Geolocation::GetLocationData(const std::string& address) {
  using Json = nlohmann::json;
  cpr::Parameters prams{};
  prams.Add(cpr::Parameter{"returntype", "location"});
  prams.Add(cpr::Parameter{"searchtype", "onelineaddress"});
  prams.Add(cpr::Parameter{"benchmark", "Public_AR_Census2020"});
  prams.Add(cpr::Parameter{"address", address});
  prams.Add(cpr::Parameter{"format", "json"});
  cpr::Response response = cpr::Get(
      cpr::Url{
          "https://geocoding.geo.census.gov/geocoder/locations/onelineaddress"},
      prams);

  if (response.status_code != 200) {
    throw std::runtime_error("Failed to get coordinates");
  }

  Json json = Json::parse(response.text);
  const std::string result = json.at("result")
                                 .at("addressMatches")
                                 .at(0)
                                 .at("matchedAddress")
                                 .get<std::string>();

  if (result.empty()) {
    throw std::runtime_error("Failed to get coordinates");
  }

  const double x = json.at("result")
                       .at("addressMatches")
                       .at(0)
                       .at("coordinates")
                       .at("x")
                       .get<double>();
  const double y = json.at("result")
                       .at("addressMatches")
                       .at(0)
                       .at("coordinates")
                       .at("y")
                       .get<double>();
  const std::string zipcode = json.at("result")
                                  .at("addressMatches")
                                  .at(0)
                                  .at("addressComponents")
                                  .at("zip")
                                  .get<std::string>();
  return std::pair{weatherer::Coordinates{y, x}, zipcode};
}
