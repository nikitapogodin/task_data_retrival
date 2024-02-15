#include "data_server.hpp"

#include "exceptions.hpp"

#include <chrono>

namespace server {

DataServer::DataServer(std::unique_ptr<DataCache> data_cache, int lifetime_ms) :
    data_cache_(std::move(data_cache)), lifetime_ms_(lifetime_ms) {
    std::cout << "Create data server with data lifetime " << lifetime_ms << " ms" << std::endl;
}

int DataServer::getDataForSource(int index) const {
    // Get data
    TimedData data = data_cache_->getData(index);
    // Data age
    auto now = TimePoint::clock::now();
    auto diff = now - data.ts;
    std::cout << "Data = " << data.data
              << ", Age = " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()
              << ". ";

    // Check lifetime
    if (diff > std::chrono::milliseconds(lifetime_ms_)) {
        std::cout << "Data is outdated" << std::endl;
        throw DataOutdatedException(index);
    } else {
        std::cout << "OK" << std::endl;
    }

    return data.data;
}

} // namespace server
