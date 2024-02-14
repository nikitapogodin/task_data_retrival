#include "data_source.hpp"

#include <chrono>
#include <iostream>
#include <random>

namespace server {

using namespace std::chrono_literals;
constexpr std::chrono::duration RETRIVAL_DELAY = 500ms;
constexpr std::chrono::duration UPDATE_INTERVAL = 1s;

DataSource::DataSource() {
    std::cout << "Create data source" << std::endl;
    // Data inside DataSource class changes at regular interval
    thread_ = std::thread([this]() { changeData(); });
}

DataSource::~DataSource() {
    stop_flag_ = true;
    // wait thread for finish
    if (thread_.joinable()) {
        thread_.join();
    }
}

int DataSource::getData() const {
    // Data retrival process has some considerable delay
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(RETRIVAL_DELAY);

    return data_;
}

void DataSource::changeData() {
    while (!stop_flag_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL));

        // Change data
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1, 99);
        data_ = static_cast<int>(dist(rng));
    }
}

} // namespace server
