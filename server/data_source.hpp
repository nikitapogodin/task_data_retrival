#pragma once

#include <atomic>
#include <thread>

namespace server {

// Data inside DataSource class changes at regular interval
// your task to implement such behavior
//
// data retrival process has some considerable delay
class DataSource {
public:
    DataSource(int change_interval_ms, int retrival_delay_ms);
    ~DataSource();

    int getData() const;

private:
    void changeData();

private:
    int change_interval_ms_;
    int retrival_delay_ms_;

    std::atomic<int> data_ = 0;

    // Data change thread
    std::thread thread_;
    std::atomic<bool> stop_flag_ = false;
};

} // namespace server