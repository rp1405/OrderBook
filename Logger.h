#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace Logger {

enum class TimeUnit {
  ResultInSeconds,
  ResultInMilliseconds,
  ResultInMicroseconds,
  ResultInNanoseconds
};

class ScopedTimer {
public:
  ScopedTimer(const std::string &name,
              TimeUnit unit = TimeUnit::ResultInMilliseconds)
      : name_(name), unit_(unit),
        start_(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    switch (unit_) {
    case TimeUnit::ResultInSeconds: {
      auto duration =
          std::chrono::duration_cast<std::chrono::seconds>(end - start_);
      std::cout << "[LOG] " << name_ << " took " << duration.count() << " s"
                << std::endl;
      break;
    }
    case TimeUnit::ResultInMilliseconds: {
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start_);
      std::cout << "[LOG] " << name_ << " took " << duration.count() << " ms"
                << std::endl;
      break;
    }
    case TimeUnit::ResultInMicroseconds: {
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
      std::cout << "[LOG] " << name_ << " took " << duration.count() << " us"
                << std::endl;
      break;
    }
    case TimeUnit::ResultInNanoseconds: {
      auto duration =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_);
      std::cout << "[LOG] " << name_ << " took " << duration.count() << " ns"
                << std::endl;
      break;
    }
    }
  }

private:
  std::string name_;
  TimeUnit unit_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

} // namespace Logger
