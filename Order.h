#pragma once
#include <cstdint>
#include <stdexcept>

class Order
{
	long long id;
	double price; // Change: double everywhere
	int quantity;
	int type; // 1 means buy, 0 means sell
	uint64_t timestamp;

	bool validate(double price, int quantity, int type) const
	{
		return price >= 0 && quantity > 0 && (type == 0 || type == 1);
	}

public:
	// Change: Default constructor required for map[] operator
	Order() {}

	Order(long long id, double price, int quantity, int type, uint64_t timestamp)
		: id(id), price(price), quantity(quantity), type(type), timestamp(timestamp)
	{
		if (!validate(price, quantity, type))
		{
			throw std::invalid_argument("Illegal quantity or price");
		}
	}

	long long getId() const { return id; }
	double &getPrice() { return price; }
	int &getQuantity() { return quantity; }
	// Change: Fixed typo (was assigning quantity = quantity)
	void setQuantity(int qty) { this->quantity = qty; }
	int getType() const { return type; }
	uint64_t &getTimestamp() { return timestamp; }
};