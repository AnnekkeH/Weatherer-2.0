#include "api/PvHandler.hpp"
#include <utility>
#include <chrono>
#include <iostream>

int main() {
	const weatherer::BulkPvDataPtr map =
		weatherer::PvHandler::aggregate({52.52, 13.41}, "2023-12-01", "2023-12-18");
	for (const auto& [key, value] : *map) {
		std::cout << key << "\n" << *value << "\n";
	}
}
