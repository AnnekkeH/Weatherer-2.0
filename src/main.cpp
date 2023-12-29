#include <chrono>
#include <iostream>
#include <utility>
#include "api/PvHandler.hpp"
#include "api/PvMetrics.hpp"
#include "api/models/Coordinates.hpp"
#include "util/Time.hpp"

int main() {
  const weatherer::Coordinates coordinates{52.52, 13.41};
  const weatherer::BulkPvDataPtr map = weatherer::PvHandler::AggregateData(
      coordinates, {"2023-12-01", "2023-12-18"});
  for (const auto& [key, value] : *map) {
    std::cout << key << "\n";
    std::cout << weatherer::PvMetrics::CalculateDailyEnergyYeild(
                     *value, coordinates, key, 0.95, 10)
              << " kWh\n\n";
  }
}
