#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>

#include "Order.h"
#include "Transactions.h"


/*Composition class responsible for managing objects of Order class through a doubly
LinkedList. The LinkedList ensures that Orders are added and removed in accordance
with the FIFO (first-in, first-out) principle. New Orders are added to the tail of
the LinkedList, while Order mathcing is performed from the head, maintaining the
desired order.*/



class LimitOrder {
	// Non-changeable properties
private:
	std::shared_ptr<Order> head;
	std::shared_ptr<Order> tail;
	int list_length;

public:
	// Constructor
	LimitOrder() : head(nullptr), tail(nullptr), list_length(0) {};

	// Head properties
	std::shared_ptr<Order> get_head();
	void set_head(const std::shared_ptr<Order>& incoming_order);
	void delete_head();

	// Tail properties
	std::shared_ptr<Order> get_tail();
	void set_tail(const std::shared_ptr<Order>& incoming_order);
	void delete_tail();

	// Length of LinkedList
	int get_length() const;

	// Add an incoming order
	void add_order(const std::shared_ptr<Order>& incoming_order);

	// Delete an order
	void delete_order(const std::shared_ptr<Order>& order_to_delete);

	// Match order
	void match_order(const std::shared_ptr<Order>& incoming_order, std::unordered_map<unsigned int, shared_ptr<Order>>& order_dict, Transactions& executed_orders);

};
