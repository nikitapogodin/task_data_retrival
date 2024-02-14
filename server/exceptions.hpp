#pragma once

#include <stdexcept>

namespace server {

class DataOutdatedException : public std::runtime_error {
public:
    explicit DataOutdatedException(int index) :
        std::runtime_error("Data outdated for source #" + std::to_string(index)) {}
};

class InvalidIndexException : public std::runtime_error {
public:
    explicit InvalidIndexException(int index) :
        std::runtime_error("Index is out of range: #" + std::to_string(index)) {}
};

} // namespace server