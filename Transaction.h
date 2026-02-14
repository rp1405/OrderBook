#pragma once
#include <cstdint>
#include <stdexcept>

class Transaction
{
    long long id;
    double price;
    int quantity;
    uint64_t timestamp;

    bool validate(double price, int quantity) const
    {
        return price >= 0 && quantity > 0;
    }

public:
    Transaction(long long id, double price, int quantity, uint64_t timestamp)
        : id(id), price(price), quantity(quantity), timestamp(timestamp)
    {
        if (!validate(price, quantity))
        {
            throw std::invalid_argument("Illegal quantity or price");
        }
    }

    long long getId() const { return id; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    uint64_t getTimestamp() const { return timestamp; }
};