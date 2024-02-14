#pragma once

#include "data_cache.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace server {

class DataServer {
public:
    DataServer(std::unique_ptr<DataCache> data_cache, int lifetime_ms);

    int getDataForSource(int i) const;

private:
    std::unique_ptr<DataCache> data_cache_;
    int lifetime_ms_ = 0;
};

}