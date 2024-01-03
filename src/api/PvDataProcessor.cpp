#include "PvDataProcessor.hpp"

#include <memory>
#include <ranges>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "util/ChunkOperator.hpp"
#include "util/Time.hpp"

[[nodiscard]] cpr::Response weatherer::PvDataProcessor::FetchHttpData(
    const Coordinates& coords, const std::string& date) {
  return FetchHttpData(coords, {date, date});
}

cpr::Response weatherer::PvDataProcessor::FetchHttpData(
    const Coordinates& coords, util::TimeFrame const& time_frame) {
  cpr::Parameters prams{};
  prams.Add(cpr::Parameter{"longitude", std::to_string(coords.GetLatitude())});
  prams.Add(cpr::Parameter{"latitude", std::to_string(coords.GetLongitude())});
  prams.Add(cpr::Parameter{"daily", "sunrise"});
  prams.Add(cpr::Parameter{"daily", "sunset"});
  prams.Add(cpr::Parameter{"hourly", "temperature_2m"});
  prams.Add(cpr::Parameter{"hourly", "cloud_cover"});
  prams.Add(cpr::Parameter{"hourly", "wind_speed_10m"});
  prams.Add(cpr::Parameter{"hourly", "direct_radiation"});
  prams.Add(cpr::Parameter{"hourly", "diffuse_radiation"});
  prams.Add(cpr::Parameter{"start_date", time_frame.start_date_});
  prams.Add(cpr::Parameter{"end_date", time_frame.end_date_});
  prams.Add(cpr::Parameter{"temperature_unit", "celcius"});

  cpr::Response res = cpr::Get(cpr::Url{kApiUrl_}, prams);
  if (res.status_code != 200) {
    throw std::runtime_error(
        "An error has occurred whilst fetching HTTP data. Status code: " +
        std::to_string(res.status_code));
  }
  return res;
}

cpr::Response weatherer::PvDataProcessor::FetchHistoricalHttpData(
    const Coordinates& coords, const std::string& date) {
  return FetchHistoricalHttpData(coords, {date, date});
}

cpr::Response weatherer::PvDataProcessor::FetchHistoricalHttpData(
    const Coordinates& coords, const util::TimeFrame& time_frame) {
  cpr::Parameters prams{};
  prams.Add(cpr::Parameter{"longitude", std::to_string(coords.GetLatitude())});
  prams.Add(cpr::Parameter{"latitude", std::to_string(coords.GetLongitude())});
  prams.Add(cpr::Parameter{"daily", "sunrise"});
  prams.Add(cpr::Parameter{"daily", "sunset"});
  prams.Add(cpr::Parameter{"hourly", "temperature_2m"});
  prams.Add(cpr::Parameter{"hourly", "cloud_cover"});
  prams.Add(cpr::Parameter{"hourly", "wind_speed_10m"});
  prams.Add(cpr::Parameter{"hourly", "direct_radiation"});
  prams.Add(cpr::Parameter{"hourly", "diffuse_radiation"});
  prams.Add(cpr::Parameter{"start_date", time_frame.start_date_});
  prams.Add(cpr::Parameter{"end_date", time_frame.end_date_});

  cpr::Response res = cpr::Get(cpr::Url{kHistoricalApiUrl_}, prams);
  if (res.status_code != 200) {
    throw std::runtime_error(
        "An error has occurred whilst fetching HTTP data. Status code: " +
        std::to_string(res.status_code));
  }
  return res;
}

[[nodiscard]] weatherer::PvDataPtr weatherer::PvDataProcessor::RetrieveDayData(
    const Coordinates& coords, const std::string& date) {
  using Json = nlohmann::json;

  const cpr::Response res = FetchHttpData(coords, date);
  const Json json = Json::parse(res.text);

  auto pv_data = std::make_shared<PvData>();

  pv_data->SetSunriseTime(json.at("daily").at("sunrise").at(0));
  pv_data->SetSunsetTime(json.at("daily").at("sunset").at(0));
  pv_data->SetDiffuseRadiation(json.at("hourly").at("diffuse_radiation"));
  pv_data->SetDirectRadiation(json.at("hourly").at("direct_radiation"));
  pv_data->SetTemperature(json.at("hourly").at("temperature_2m"));
  pv_data->SetCloudCoverTotal(json.at("hourly").at("cloud_cover"));
  pv_data->SetWindSpeed(json.at("hourly").at("wind_speed_10m"));
  return pv_data;
}

weatherer::PvCollectionPtr weatherer::PvDataProcessor::GenerateBulkData(
    const std::string& response, const util::TimeFrame& time_frame) {
  using namespace util;
  using Json = nlohmann::json;
  int total_days =
      Time::DifferenceInDays(time_frame.start_date_, time_frame.end_date_);

  auto data = std::make_shared<std::map<std::string, PvDataPtr>>();
  Json json = Json::parse(response);

  const auto hourly = json.at("hourly");
  const int split_amount = total_days + 1;

  const auto diffuse_radiations =
      ChunkVector<int, 24>(hourly.at("diffuse_radiation"), split_amount);
  const auto direct_radiations =
      ChunkVector<int, 24>(hourly.at("direct_radiation"), split_amount);
  const auto temperatures =
      ChunkVector<double, 24>(hourly.at("temperature_2m"), split_amount);
  const auto cloud_cover_totals =
      ChunkVector<int, 24>(hourly.at("cloud_cover"), split_amount);
  const auto wind_speeds =
      ChunkVector<double, 24>(hourly.at("wind_speed_10m"), split_amount);

  for (const int i : std::ranges::views::iota(0, total_days)) {
    const auto [date, val] = Time::GetFutureDate(time_frame.start_date_, i + 1);

    auto pv_data = std::make_shared<PvData>();
    pv_data->SetSunriseTime(json.at("daily").at("sunrise").at(i));
    pv_data->SetSunsetTime(json.at("daily").at("sunset").at(i));
    pv_data->SetDiffuseRadiation(diffuse_radiations.at(i));
    pv_data->SetDirectRadiation(direct_radiations.at(i));
    pv_data->SetTemperature(temperatures.at(i));
    pv_data->SetCloudCoverTotal(cloud_cover_totals.at(i));
    pv_data->SetWindSpeed(wind_speeds.at(i));

    data->insert({date, pv_data});
  }
  return data;
}

[[nodiscard]] weatherer::PvCollectionPtr
weatherer::PvDataProcessor::AggregateAllData(
    const Coordinates& coords, util::TimeFrame const& time_frame) {
  using namespace util;

  // If we started in the past and ended in the past
  const std::string start_date = time_frame.start_date_;
  const std::string end_date = time_frame.end_date_;
  if (Time::DifferenceInDays(start_date, end_date) >= 14 &&
      Time::DifferenceInDays(end_date, Time::GetCurrentDate().time_) >= 5) {
    const std::string response =
        FetchHistoricalHttpData(coords, time_frame).text;
    return GenerateBulkData(response, time_frame);
  }
  // If we started in the past and ended in the future
  if (Time::DifferenceInDays(start_date, end_date) >= 14 &&
      Time::DifferenceInDays(end_date, Time::GetCurrentDate().time_) <= 5) {
    const auto five_days_ago = Time::GetPastDate(5).time_;
    const std::string first_reponse =
        FetchHistoricalHttpData(coords, {start_date, five_days_ago}).text;
    const std::string second_reponse =
        FetchHttpData(coords, {five_days_ago, end_date}).text;

    const PvCollectionPtr first_data = GenerateBulkData(
        first_reponse, {start_date, Time::GetPastDate(5).time_});
    const PvCollectionPtr second_data =
        GenerateBulkData(second_reponse, {five_days_ago, end_date});

    first_data->insert(second_data->begin(), second_data->end());
    return first_data;
  }
  // If we started in the future and ended in the future
  return GenerateBulkData(FetchHttpData(coords, time_frame).text, time_frame);
};
