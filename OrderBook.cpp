#include "Logger.h"
#include "Order.h"
#include "Transaction.h"
#include <bits/stdc++.h>
#include <chrono>
#include <unordered_map>

class OrderBook {
private:
  // Change: id should be long long to match Order class
  std::unordered_map<long long, Order> orders;

  // Change: price keys changed to double
  std::map<double, std::map<uint64_t, int>> sellOrders;
  std::map<double, std::map<uint64_t, int>, std::greater<double>>
      buyOrders; // Change: greater<double>

  std::vector<Transaction> transactions;

  void matchBuyOrder(long long id) {
    Order &order = orders[id];
    double offerPrice = order.getPrice();
    int &qty = order.getQuantity();

    while (qty > 0) {
      // Change: Check if empty OR if best price is too high
      if (sellOrders.empty() || sellOrders.begin()->first > offerPrice) {
        buyOrders[offerPrice][order.getTimestamp()] = order.getId();
        // Order is already in map
        break; // Change: Break to prevent infinite loop
      }

      Order &bestOrder = orders[sellOrders.begin()->second.begin()->second];

      // Change: Calculate execution based on quantity, not price
      int execQty = std::min(qty, bestOrder.getQuantity());

      qty -= execQty;
      bestOrder.getQuantity() -= execQty;

      // Change: Store the transaction
      transactions.push_back(createTransaction(bestOrder.getPrice(), execQty));

      if (bestOrder.getQuantity() == 0) {
        del(bestOrder.getId());
      }
    }
  }

  void matchSellOrder(long long id) {
    Order &order = orders[id];
    double offerPrice = order.getPrice();
    int &qty = order.getQuantity();

    while (qty > 0) {
      // Change: Check if empty OR if best price is too low
      if (buyOrders.empty() || buyOrders.begin()->first < offerPrice) {
        sellOrders[offerPrice][order.getTimestamp()] = order.getId();
        // Order is already in map
        break; // Change: Break to prevent infinite loop
      }

      Order &bestOrder = orders[buyOrders.begin()->second.begin()->second];

      // Change: Calculate execution based on quantity
      int execQty = std::min(qty, bestOrder.getQuantity());

      qty -= execQty;
      bestOrder.getQuantity() -= execQty;

      // Change: Store the transaction
      transactions.push_back(createTransaction(bestOrder.getPrice(), execQty));

      if (bestOrder.getQuantity() == 0) {
        del(bestOrder.getId());
      }
    }
  }

  void match(long long id) {
#ifndef DISABLE_LOGGING
    Logger::ScopedTimer timer("OrderBook::match",
                              Logger::TimeUnit::ResultInMicroseconds);
#endif
    if (orders.find(id) == orders.end())
      return;

    Order &order = orders[id];

    int type = order.getType();
    type == 1 ? matchBuyOrder(id) : matchSellOrder(id);
  }

  // Change: price is double
  void updateBuyOrder(long long id, double price, int quantity, int timestamp) {
    Order &order = orders[id];
    buyOrders[order.getPrice()].erase(order.getTimestamp());
    buyOrders[price][timestamp] = id;
  }

  // Change: price is double
  void updateSellOrder(long long id, double price, int quantity,
                       int timestamp) {
    Order &order = orders[id];
    sellOrders[order.getPrice()].erase(order.getTimestamp());
    sellOrders[price][timestamp] = id;
  }

  void deleteBuyOrderFromOrderBook(Order &order) {
    buyOrders[order.getPrice()].erase(order.getTimestamp());
    // Change: Clean up map if price level is empty (optional but good practice)
    if (buyOrders[order.getPrice()].empty()) {
      buyOrders.erase(order.getPrice());
    }
  }

  void deleteSellOrderFromOrderBook(Order &order) {
    sellOrders[order.getPrice()].erase(order.getTimestamp());
    // Change: Clean up map if price level is empty
    if (sellOrders[order.getPrice()].empty()) {
      sellOrders.erase(order.getPrice());
    }
  }

  // Change: price is double
  Transaction createTransaction(double price, int quantity) {
    static long long globalId = 1;
    uint64_t ts = getCurrentTimestamp();
    return Transaction(globalId++, price, quantity, ts);
  }

  uint64_t getCurrentTimestamp() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }

public:
  // Change: price is double
  Order create(double price, int quantity, int type) {
#ifndef DISABLE_LOGGING
    Logger::ScopedTimer timer("OrderBook::create",
                              Logger::TimeUnit::ResultInMicroseconds);
#endif
    static long long globalId = 1;
    uint64_t ts = getCurrentTimestamp();

    // Change: Create, insert, THEN match by ID
    long long id = globalId++;
    Order order(id, price, quantity, type, ts);
    orders[id] = order;

    match(id);

    // Return the updated order state (might be filled)
    return orders[id];
  }

  // Change: price is double, id is long long
  void update(long long id, double price, int quantity) {
#ifndef DISABLE_LOGGING
    Logger::ScopedTimer timer("OrderBook::update",
                              Logger::TimeUnit::ResultInMicroseconds);
#endif
    // Change: Check if ID exists to prevent crash
    if (orders.find(id) == orders.end())
      return;

    Order &order = orders[id];
    uint64_t ts = getCurrentTimestamp();
    order.getType() ? updateBuyOrder(id, price, quantity, ts)
                    : updateSellOrder(id, price, quantity, ts);
    order.getPrice() = price;
    order.getQuantity() = quantity;
    order.getTimestamp() = ts;
  }

  // Change: id is long long
  void del(long long id) {
#ifndef DISABLE_LOGGING
    Logger::ScopedTimer timer("OrderBook::del",
                              Logger::TimeUnit::ResultInMicroseconds);
#endif
    // Change: Check if ID exists
    if (orders.find(id) == orders.end())
      return;

    Order order = orders[id]; // Copy for deletion logic
    if (order.getType()) {
      deleteBuyOrderFromOrderBook(order);
    } else {
      deleteSellOrderFromOrderBook(order);
    }
    orders.erase(id);
  }
};