#pragma once

#include "LimitOrder.h"
#include "Transactions.h"

#include <map>
#include <unordered_map>
#include <memory>


/*Composition class responsible for managing objects of LimitOrder class through a balanced binary
search tree (red-black tree). The Tree will be sorted based on Price only, other factors like
event time are not considered. The class also contains data structures for efficient access to
LimitOrder and Order objects*/

class OrderTree {

public:
	// Map (red-black tree) with Price as key and LimitOrder as value
	std::map<float, LimitOrder> order_tree;

	// Unordered map with Order id as key and Order as value
	std::unordered_map<unsigned int, std::shared_ptr<Order>> order_map;

	// Unordered map with Order price as key and LimitOrder as value - Maybe this could be made into 
	// a set or could be a pointer to a LimitOrder?
	std::unordered_map<float, LimitOrder> price_map;

	//Min and Max price within the Tree
	float min_price;
	float max_price;

	// Constructor
	OrderTree() : order_tree({}), order_map({}), price_map({}),
		min_price(std::numeric_limits<float>::quiet_NaN()),
		max_price(std::numeric_limits<float>::quiet_NaN()) {};

	// length of Order Tree, measured as total orders stored within the Tree
	int get_length() const;

	// Add a new limit_price into the Tree, aka a LimitOrder (a doubly linked-list of Order objects)
	void add_limit_price(const float& limit_price);

	// Delete price from Tree, price map and order map
	void delete_limit_price(const float& limit_price);

	// Add Order into the Tree, aka adding an order into a LimitOrder list
	void add_price_order(const std::shared_ptr<Order>& incoming_order);

	// Delete Order from 
	void delete_price_order(const std::shared_ptr<Order>& order_to_delete);

	// Match incoming_order
	void match_price_order(const std::shared_ptr<Order>& incoming_order);



};