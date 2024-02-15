#include "data_source.hpp"

#include <chrono>
#include <iostream>
#include <random>

namespace server {

DataSource::DataSource(int change_interval_ms, int retrival_delay_ms) :
    change_interval_ms_(change_interval_ms), retrival_delay_ms_(retrival_delay_ms) {
    std::cout << "Create data source with change interval " << change_interval_ms_
              << " ms, retrival delay " << retrival_delay_ms_ << " ms" << std::endl;
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
    std::this_thread::sleep_for(std::chrono::milliseconds(retrival_delay_ms_));

    return data_;
}

void DataSource::changeData() {
    while (!stop_flag_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(change_interval_ms_));

        // Change data
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1, 99);
        data_ = static_cast<int>(dist(rng));
    }
}

} // namespace server
