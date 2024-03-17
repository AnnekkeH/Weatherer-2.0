#include "CropData.hpp"

weatherer::CropData::CropData()
    : name_("Unknown"),
      species_name_("Unknown"),
      pref_soil_temp_{INT_MIN},
      pref_air_temp_{INT_MIN},
      pref_soil_ph_{util::NumericRange<double>{0, 0}},
      pref_light_level_{"Unknown"},
      fun_facts_{"N/A"} {}

weatherer::CropData::CropData(const CropData& other) = default;

weatherer::CropData::CropData(CropData&& other) noexcept
    : name_(std::move(other.name_)),
      species_name_(std::move(other.species_name_)),
      pref_soil_temp_(other.pref_soil_temp_),
      pref_air_temp_(other.pref_air_temp_),
      pref_soil_ph_(other.pref_soil_ph_),
      pref_light_level_(std::move(other.pref_light_level_)),
      fun_facts_(std::move(other.fun_facts_)),
      plantable_(other.plantable_) {}

weatherer::CropData& weatherer::CropData::operator=(const CropData& other) {
  if (this == &other)
    return *this;
  name_ = other.name_;
  species_name_ = other.species_name_;
  pref_soil_temp_ = other.pref_soil_temp_;
  pref_air_temp_ = other.pref_air_temp_;
  pref_soil_ph_ = other.pref_soil_ph_;
  pref_light_level_ = other.pref_light_level_;
  fun_facts_ = other.fun_facts_;
  plantable_ = other.plantable_;
  return *this;
}

weatherer::CropData& weatherer::CropData::operator=(CropData&& other) noexcept {
  if (this == &other)
    return *this;
  name_ = std::move(other.name_);
  species_name_ = std::move(other.species_name_);
  pref_soil_temp_ = other.pref_soil_temp_;
  pref_air_temp_ = other.pref_air_temp_;
  pref_soil_ph_ = other.pref_soil_ph_;
  pref_light_level_ = std::move(other.pref_light_level_);
  fun_facts_ = std::move(other.fun_facts_);
  plantable_ = other.plantable_;
  return *this;
}

std::string weatherer::CropData::GetName() const {
  return name_;
}

void weatherer::CropData::SetName(const std::string& name) {
  name_ = name;
}

std::string weatherer::CropData::GetSpeciesName() const {
  return species_name_;
}

void weatherer::CropData::SetSpeciesName(const std::string& species_name) {
  species_name_ = species_name;
}

int weatherer::CropData::GetPrefSoilTemp() const {
  return pref_soil_temp_;
}

void weatherer::CropData::SetPrefSoilTemp(int pref_soil_temp) {
  pref_soil_temp_ = pref_soil_temp;
}

int weatherer::CropData::GetPrefAirTemp() const {
  return pref_air_temp_;
}

void weatherer::CropData::SetPrefAirTemp(int pref_air_temp) {
  pref_air_temp_ = pref_air_temp;
}

weatherer::util::NumericRange<double> weatherer::CropData::GetPrefSoilPh()
    const {
  return pref_soil_ph_;
}

std::string weatherer::CropData::GetPrefLightLevel() const {
  return pref_light_level_;
}

void weatherer::CropData::SetPrefLightLevel(
    const std::string& pref_light_level) {
  pref_light_level_ = pref_light_level;
}

void weatherer::CropData::SetPrefSoilPh(
    const util::NumericRange<double>& pref_soil_ph) {
  pref_soil_ph_ = pref_soil_ph;
}

std::string weatherer::CropData::GetFunFacts() const {
  return fun_facts_;
}

void weatherer::CropData::SetFunFacts(const std::string& fun_facts) {
  fun_facts_ = fun_facts;
}

bool weatherer::CropData::IsPlantable() const {
  return plantable_;
}

void weatherer::CropData::SetPlantable(bool plantable) {
  plantable_ = plantable;
}
