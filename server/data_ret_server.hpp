#pragma once

#include <chrono>
#include <iostream>
#include <thread>

// Data inside DataSource class changes at regular interval
// your task to implement such behavior
//
// data retrival process has some considerable delay
class DataSource {

public:
  DataSource() {}

  int getData() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);

    // TODO: impliment the code to return data
  }

private:
  int data_;
};

class DataRetServer {
public:
  DataRetServer(int n, int lifetime) {
    std::cout << "Create data retriver server for " << n
              << " data sources; with data lifetime " << lifetime << std::endl;

    // TODO: Implement the code to initiate data retrieval from 'n' sources
  }

  // TODO: Implement the code to return the value for the ith DataSource in less
  // then 100ms
  // TODO: Implement a check to verify the returned data is not older then its
  // specified lifetime
  int getDataForSource(int i) {}

private:
};
