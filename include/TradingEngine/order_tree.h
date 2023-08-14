/*
Contains declaration for composition class responsible for managing objects of LimitOrder class
through a balanced binary search tree (red-black tree). The Tree will be sorted based on Price only,
other factors like event time are not considered. The class also contains data structures for efficient
access to LimitOrder and Order objects.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_ORDER_TREE_H_
#define TRADING_ENGINE_SOURCE_HEADERS_ORDER_TREE_H_

#pragma once
#include <map>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <sstream>

#include "transactions.h"
#include "order_type.h"
#include "order_status.h"
#include "order.h"
#include "limit_order.h"


class OrderTree {
public:
	std::map<float, int> order_tree; /* Map (red-black tree) with LimitOrder price as key and LimitOrder length as value */

	std::unordered_set<unsigned int> order_set; /* Unordered set with unique orders */

	std::unordered_map<float, LimitOrder> price_map; /* Unordered map with LimitOrder price as key and LimitOrder as value */

	float min_price;
	float max_price;
	OrderType type;

public:
	OrderTree(OrderType tree_type) : order_tree({}), order_set({}), price_map({}), min_price(std::numeric_limits<float>::quiet_NaN()), max_price(std::numeric_limits<float>::quiet_NaN()), type(tree_type) {};

	/* Length of Order Tree, measured as total orders stored within the order_map */
	size_t getLength() const;

	/* Min properties */
	const float& getMinPrice();
	void setMinPrice(const float& new_min);

	/* Max properties */
	const float& getMaxPrice();
	void setMaxPrice(const float& new_max);

	const OrderType& getType() const;

	void addPriceOrder(Order& incoming_order);

	void deletePriceOrder(const std::shared_ptr<Order>& order_to_delete);

	void deleteLimitPrice(const float& limit_price);

	void matchPriceOrder(Order& incoming_order, std::ostringstream& store_transaction);

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_ORDER_TREE_H_
