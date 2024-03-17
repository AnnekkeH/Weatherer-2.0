#include "PvDataProcessor.hpp"

#include <memory>
#include <ranges>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "util/ChunkOperator.hpp"
#include "util/Date.hpp"

cpr::Response weatherer::PvDataProcessor::IngestData(
    const Coordinates& coords, util::TimeFrame const& time_frame,
    const bool historical) {
  cpr::Parameters prams{};
  // Set the parameters for the HTTP request.
  prams.Add(cpr::Parameter{"longitude", std::to_string(coords.GetLongitude())});
  prams.Add(cpr::Parameter{"latitude", std::to_string(coords.GetLatitude())});
  prams.Add(cpr::Parameter{"daily", "sunrise"});
  prams.Add(cpr::Parameter{"daily", "sunset"});
  prams.Add(cpr::Parameter{"hourly", "temperature_2m"});
  prams.Add(cpr::Parameter{"hourly", "cloud_cover"});
  prams.Add(cpr::Parameter{"hourly", "wind_speed_10m"});
  prams.Add(cpr::Parameter{"hourly", "shortwave_radiation"});
  prams.Add(
      cpr::Parameter{"start_date", time_frame.GetStartDate().StripTime()});
  prams.Add(cpr::Parameter{"end_date", time_frame.GetEndDate().StripTime()});
  prams.Add(cpr::Parameter{"temperature_unit", "celsius"});
  prams.Add(cpr::Parameter{"timeformat", "unixtime"});
  prams.Add(cpr::Parameter{"timezone", "auto"});

  // Perform the HTTP GET request to the regular Open-Metro API.
  cpr::Response res = !historical
                          ? cpr::Get(cpr::Url{kApiUrl_}, prams)
                          : cpr::Get(cpr::Url{kHistoricalApiUrl_}, prams);

  // If the status code is not 200, throw an exception.
  if (res.status_code != 200) {
    throw std::runtime_error(
        "An error has occurred whilst fetching HTTP data. Status code: " +
        std::to_string(res.status_code));
  }
  return res;
}

[[nodiscard]] weatherer::PvCollectionPtr
weatherer::PvDataProcessor::OrganizeWeatherData(
    const std::string& response, const util::TimeFrame& time_frame) {
  using namespace util;
  using Json = nlohmann::json;

  // Calculate the total number of days in the specified time frame.
  std::time_t total_days =
      (time_frame.GetEndDate() - time_frame.GetStartDate()) /
      Date::kSecondsPerDay;

  auto data = std::make_shared<std::unordered_map<std::string, PvDataPtr>>();
  // Parse the JSON response.
  Json json = Json::parse(response);

  const auto hourly = json.at("hourly");
  const auto daily = json.at("daily");

  const std::size_t split_amount = total_days + 1;
  // Create chunked vectors for each weather attribute.
  const auto shortwave_radiations =
      ChunkVector<int, 24>(hourly.at("shortwave_radiation"), split_amount);
  const auto temperatures =
      ChunkVector<double, 24>(hourly.at("temperature_2m"), split_amount);
  const auto cloud_cover_totals =
      ChunkVector<int, 24>(hourly.at("cloud_cover"), split_amount);
  const auto wind_speeds =
      ChunkVector<double, 24>(hourly.at("wind_speed_10m"), split_amount);

  // Loop through each day in the specified time frame.
  for (const int i : std::ranges::views::iota(0, total_days)) {
    auto pv_data = std::make_shared<PvData>();
    // Set weather data attributes for the current day using relevant hourly information.
    pv_data->SetDate(
        Date{static_cast<std::time_t>(daily.at("time").at(i))}.StripTime());
    pv_data->SetSunriseTime(daily.at("sunrise").at(i));
    pv_data->SetSunsetTime(daily.at("sunset").at(i));
    pv_data->SetShortwaveRadiation(shortwave_radiations.at(i));
    pv_data->SetTemperature(temperatures.at(i));
    pv_data->SetCloudCoverTotal(cloud_cover_totals.at(i));
    pv_data->SetWindSpeed(wind_speeds.at(i));

    // Add the date and corresponding weather data to the map.
    data->insert({pv_data->GetDate(), pv_data});
  }
  return data;
}

[[nodiscard]] weatherer::PvCollectionPtr
weatherer::PvDataProcessor::CollectData(
    const Coordinates& coords, util::TimeFrame const& time_frame) {
  using namespace util;

  Date today{};
  today.ResetToMidnight();
  // Extract start and end dates from the time frame.
  Date start_date = Date{time_frame.GetStartDate()};
  start_date.ResetToMidnight();

  Date end_date = Date{time_frame.GetEndDate()};
  end_date.ResetToMidnight();

  // If the time frame covers a period starting and ending in the past
  if ((end_date - start_date) / Date::kSecondsPerDay >= 14 &&
      (today - end_date) / Date::kSecondsPerDay >= 5) {
    const std::string response = IngestData(coords, time_frame, true).text;
    return OrganizeWeatherData(response, time_frame);
  }

  // If the time frame starts in the past and ends in the future
  if ((end_date - start_date) / Date::kSecondsPerDay >= 14 &&
      (today - end_date) / Date::kSecondsPerDay <= 5) {
    const auto five_days_ago = today - (5 * Date::kSecondsPerDay);
    const std::string first_reponse =
        IngestData(
            coords, TimeFrame{start_date.ToString(), five_days_ago.ToString()},
            true)
            .text;
    const std::string second_reponse =
        IngestData(coords,
                      TimeFrame{five_days_ago.ToString(), end_date.ToString()})
            .text;

    // Generate bulk data for each period.
    const PvCollectionPtr first_data = OrganizeWeatherData(
        first_reponse,
        TimeFrame{start_date.ToString(),
                  (today - (5 * Date::kSecondsPerDay)).ToString()});
    const PvCollectionPtr second_data = OrganizeWeatherData(
        second_reponse,
        TimeFrame{five_days_ago.ToString(), end_date.ToString()});

    // Combine data from both periods and return.
    first_data->insert(second_data->begin(), second_data->end());
    return first_data;
  }
  // If the time frame starts and ends in the future, fetch and generate bulk data for the future.
  return OrganizeWeatherData(IngestData(coords, time_frame).text, time_frame);
}
