#include <memory>
#include "api/PVMetrics.hpp"

int main() {
	weatherer::PVMetrics const pv{ 52.52, 13.41 };
	std::shared_ptr<weatherer::PvData> data = pv.get_pv_data();
	// print the data
}