#pragma once
#include <string>
#include <ctime>
#include <chrono>

#include <memory>


enum OrderStatus {
	NewOrder = 1,
	PartiallyFilled = 2,
	Filled = 3
};


class Order {
	// Non-changeable properties
public:
	const unsigned int id;
	const std::string type;
	const float price;
	const float original_quantity;

	// Modifiable properties
	OrderStatus order_status;

	float peak_quantity;
	float peak_quantity_left;
	float quantity_transacted;

	std::chrono::time_point<std::chrono::system_clock> order_time;

	// Doubly Linked List
	std::shared_ptr<Order> next_order;
	std::shared_ptr<Order> prev_order;

	// Constructor
	Order(unsigned int input_id, std::string input_type, float input_price, float input_original_quantity);

	// Match partial or complete orders
	bool match(const std::shared_ptr<Order>& incoming_oder);

	// Execute the matched order, and update quantities
	void execute_transaction(float const& input_quantity);

	// Restore peak quantity
	void restore_peak_quantity();

	// Print Order
	void print_order();

	std::string raw_order_info() const;
};