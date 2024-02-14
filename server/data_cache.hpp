#pragma once

#include "data_source.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace server {

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

struct TimedData {
    int data = 0;
    TimePoint ts{};
};

// Caches data and it timestamps for given DataSources
class DataCache {
public:
    explicit DataCache(std::vector<std::unique_ptr<DataSource>> sources);
    ~DataCache();

    TimedData getData(int index) const;

private:
    void requestData(size_t index, DataSource& source);

    void setData(size_t index, int data);

private:
    std::vector<std::unique_ptr<DataSource>> sources_;
    std::vector<TimedData> data_cache_;

    // Data request threads
    std::vector<std::thread> threads_;
    mutable std::mutex data_mutex_;
    std::atomic<bool> stop_flag_{false};
};

}