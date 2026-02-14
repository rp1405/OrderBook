#define DISABLE_LOGGING
#include "OrderBook.cpp"
#include <iostream>

// Simple benchmark wrapper
void benchmarkMatchingThroughput() {
  std::cout << "Starting Benchmark: Matching Throughput (1M pairs)..."
            << std::endl;
  // We want to measure the throughput of create+match operations.
  // 1M Buys matching 1M Sells.

  OrderBook book;

  {
    Logger::ScopedTimer timer("Matching 1M pairs",
                              Logger::TimeUnit::ResultInMilliseconds);
    // Pre-fill One side? Or alternating?
    // Alternating simulates real trading better (crossing spread).
    // 1. Create Sell Limit 100 @ 10
    // 2. Create Buy Limit 100 @ 10 -> Match

    for (int i = 0; i < 1000000; ++i) {
      book.create(100.0, 10, 0); // Sell
      book.create(100.0, 10, 1); // Buy (Matches immediately)
    }
  }
  // timer destructor prints time
}

void benchmarkOrderCreationOnly() {
  std::cout << "Starting Benchmark: Order Creation (1M, no match)..."
            << std::endl;
  OrderBook book;

  {
    Logger::ScopedTimer timer("Creating 1M Orders",
                              Logger::TimeUnit::ResultInMilliseconds);
    // All Sells at different prices (no matching)
    for (int i = 0; i < 1000000; ++i) {
      book.create(i * 1.0, 10, 0);
    }
  }
}

int main() {
  benchmarkOrderCreationOnly();
  benchmarkMatchingThroughput();
  return 0;
}
