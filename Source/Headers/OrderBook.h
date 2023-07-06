#pragma once
#include "OrderTree.h"

/* Composition class responsible for managing Buy and Sell Order Trees.*/

class OrderBook {

public:
	OrderTree buy_tree;
	OrderTree sell_tree;

	// Constructor
	OrderBook() : buy_tree(OrderTree()), sell_tree(OrderTree()) {};

	// Execute incoming Order
	void execute_order(const std::shared_ptr<Order>& incoming_order);

	// Cancel Order
	void cancel_order(const std::shared_ptr<Order>& cancel_order);

	void print_real_time_OrderBook();
};