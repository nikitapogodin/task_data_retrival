#include "data_cache.hpp"

#include "exceptions.hpp"

#include <iostream>

namespace server {

DataCache::DataCache(std::vector<std::unique_ptr<DataSource>> sources) : sources_(std::move(sources)) {
    std::cout << "Initialize data cache for " << sources_.size() << " sources" << std::endl;
    // Create threads for requesting data from each source to ensure that we have the latest one
    for (size_t i = 0; i < sources_.size(); ++i) {
        threads_.emplace_back([this, i, &source = sources_[i]]() { requestData(i, *source); });
    }
    // Initialize cache
    data_cache_.resize(sources_.size());
}

DataCache::~DataCache() {
    // Stop all threads
    stop_flag_ = true;
    // Wait all threads for finish
    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

TimedData DataCache::getData(int index) const {
    if (index < 0 || index >= static_cast<int>(data_cache_.size())) {
        throw InvalidIndexException(index);
    }
    std::unique_lock<std::mutex> lock(data_mutex_);
    return data_cache_[index];
}

void DataCache::setData(size_t index, int data) {
    auto ts = TimePoint::clock::now();

    std::unique_lock<std::mutex> lock(data_mutex_);
    data_cache_[index] = {data, ts};
}

void DataCache::requestData(size_t index, DataSource& source) {
    while (!stop_flag_) {
        // Request data
        auto data = source.getData();
        // Update cache
        setData(index, data);
    }
}

}
