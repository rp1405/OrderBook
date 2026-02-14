#include "OrderBook.cpp"
#include <cassert>
#include <iostream>

void testBasicMatch() {
  std::cout << "Test: Basic Match... ";
  OrderBook book;
  Order buy = book.create(100.0, 10, 1);  // Buy 10 @ 100
  Order sell = book.create(100.0, 10, 0); // Sell 10 @ 100

  // In a perfect match, the second order (sell) should match immediately.
  // The quantity returned should be 0 if fully executed.
  assert(sell.getQuantity() == 0);

  // Verify first order is gone by trying to match against it again.
  Order sell2 = book.create(100.0, 10, 0);
  assert(sell2.getQuantity() == 10); // Should Not match (buy is gone)

  std::cout << "Passed\n";
}

void testPartialFill() {
  std::cout << "Test: Partial Fill... ";
  OrderBook book;
  Order buy = book.create(100.0, 20, 1);  // Buy 20
  Order sell = book.create(100.0, 10, 0); // Sell 10

  assert(sell.getQuantity() == 0); // Fully executed against buy

  // Remaining Buy should be 10.
  // Test by matching another 10.
  Order sell2 = book.create(100.0, 10, 0);
  assert(sell2.getQuantity() == 0); // Should execute against remainder

  // Now Buy should be empty.
  Order sell3 = book.create(100.0, 10, 0);
  assert(sell3.getQuantity() == 10); // No match

  std::cout << "Passed\n";
}

void testPricePriority() {
  std::cout << "Test: Price Priority... ";
  OrderBook book;
  // Sell Orders: 10 @ 100, 10 @ 101
  book.create(100.0, 10, 0);
  book.create(101.0, 10, 0);

  // Buy 15 @ 102 (Aggressive) -> Should match 10 @ 100 first, then 5 @ 101.
  Order buy = book.create(102.0, 15, 1);
  assert(buy.getQuantity() == 0);

  // Initial sells: 100 is gone, 101 has 5 left.
  // Verify by trying to buy at 100.5 (should no longer match 100)
  Order buyProbe = book.create(100.5, 10, 1);
  assert(buyProbe.getQuantity() == 10); // No match (Lowest sell is 101)

  // Verify by match against remaining 5 @ 101
  Order buyCleanup = book.create(101.0, 5, 1);
  assert(buyCleanup.getQuantity() == 0);

  std::cout << "Passed\n";
}

int main() {
  testBasicMatch();
  testPartialFill();
  testPricePriority();
  std::cout << "All Correctness Tests Passed!" << std::endl;
  return 0;
}
