#include "OrderBook.cpp"
#include <iostream>
#include <thread>

int main() {
  OrderBook orderBook;

  std::cout << "--- Creating Buy Order ---" << std::endl;
  Order buyOrder = orderBook.create(100.5, 10, 1);

  std::this_thread::sleep_for(
      std::chrono::milliseconds(10)); // Simulate some delay

  std::cout << "\n--- Creating Sell Order ---" << std::endl;
  Order sellOrder = orderBook.create(100.5, 5, 0);

  std::cout << "\n--- Updating Order ---" << std::endl;
  orderBook.update(buyOrder.getId(), 101.0, 5);

  std::cout << "\n--- Deleting Order ---" << std::endl;
  orderBook.del(buyOrder.getId());

  return 0;
}
