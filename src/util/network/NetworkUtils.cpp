#include "NetworkUtils.hpp"

std::string weather::NetworkUtils::get_http_response(const std::string& uri) {
	const cpr::Response response = cpr::Get(cpr::Url{uri});
	if (response.status_code != 200) {
		throw std::runtime_error(
			"An internal error occurred whilst fetching the HTTP data: " + std::to_string(
				response.status_code));
	}
	return response.text;
}
