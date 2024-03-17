#pragma once
#include <ostream>
#include <string>

#include "util/NumericRange.hpp"

namespace weatherer {
class CropData {
 private:
  std::string name_;
  std::string species_name_;
  int pref_soil_temp_;
  int pref_air_temp_;
  util::NumericRange<double> pref_soil_ph_;
  std::string pref_light_level_;
  std::string fun_facts_;
  bool plantable_ = false;

 public:
  explicit CropData();

  CropData(const CropData& other);

  CropData(CropData&& other) noexcept;

  CropData& operator=(const CropData& other);
  CropData& operator=(CropData&& other) noexcept;

  [[nodiscard]] std::string GetName() const;
  void SetName(const std::string& name);
  [[nodiscard]] std::string GetSpeciesName() const;
  void SetSpeciesName(const std::string& species_name);
  [[nodiscard]] int GetPrefSoilTemp() const;
  void SetPrefSoilTemp(int pref_soil_temp);
  [[nodiscard]] int GetPrefAirTemp() const;
  void SetPrefAirTemp(int pref_air_temp);
  [[nodiscard]] util::NumericRange<double> GetPrefSoilPh() const;
  [[nodiscard]] std::string GetPrefLightLevel() const;
  void SetPrefLightLevel(const std::string& pref_light_level);
  void SetPrefSoilPh(const util::NumericRange<double>& pref_soil_ph);
  [[nodiscard]] std::string GetFunFacts() const;
  void SetFunFacts(const std::string& fun_facts);
  [[nodiscard]] bool IsPlantable() const;
  void SetPlantable(bool plantable);

  friend std::ostream& operator<<(std::ostream& os, const CropData& obj);
};

inline std::ostream& operator<<(std::ostream& os, const CropData& obj) {
  if (obj.GetPrefAirTemp() != INT_MIN && obj.GetPrefSoilTemp() != INT_MIN) {
    os << "Name: " << obj.name_ << "\n";

    if (obj.GetSpeciesName() != "Unknown") {
      os << "Species Name: " << obj.species_name_ << "\n";
    }
    os << "Pref Soil Temp: " << obj.pref_soil_temp_ << "\n"
       << "Pref Air Temp: " << obj.pref_air_temp_ << "\n"
       << "Pref Soil Ph: " << obj.pref_soil_ph_ << "\n"
       << "Pref Light Level: " << obj.pref_light_level_ << "\n";
    if (obj.GetFunFacts() != "N/A") {
      os << "Fun Facts: " << obj.fun_facts_ << "\n";
    }
    os << "Plantable: " << std::boolalpha << obj.plantable_ << "\n\n";
  }
  return os;
}

}  // namespace weatherer
