#include <fstream>
#include <iostream>
#include "api/CropDataProcessor.hpp"
#include "nlohmann/json.hpp"
#include "util/ChunkOperator.hpp"

int main() {
  // double latitute{}, longitude{}, panel_eff{}, panel_area{};
  // std::println("Enter latitude (Example: 34.0549)");
  // std::cin >> latitute;
  // std::println("Enter longitude (Example: -118.2426)");
  // std::cin >> longitude;
  // std::println("Enter panel efficiency [0, 1] (Example: 0.20)");
  // std::cin >> panel_eff;
  // std::println("Enter panel area in m^2 (Example 157.94)");
  // std::cin >> panel_area;
  // const auto pv_handle = std::make_unique<weatherer::PvHandler>(
  //     weatherer::Coordinates{latitute, longitude},
  //     weatherer::util::TimeFrame{"2023-07-01", "2023-08-01"});
  //
  // // pv_handle->OutputData(std::cout);
  // pv_handle->OutputDailyEnergyYeild(std::cout, panel_eff, panel_area);

  std::cout << "Address: ";
  std::string address{};
  std::getline(std::cin, address);
  auto plant_processor = weatherer::CropDataProcessor(weatherer::util::Geolocation::GetLocationData(address));

  auto data = plant_processor.GetData();
  for (const auto& value : *data | std::views::values) {
    std::cout << *value;
  }

  system("pause");
  return 0;
}
