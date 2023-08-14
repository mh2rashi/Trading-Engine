/*
Contains declaration for OrderStatus enum and Order status map. 
OrderStatus enum is a property of the Order class and Order
Status map is used for string representation of Order info.
*/

#ifndef TRADING_ENGINE_INCLUDE_ORDER_STATUS_H_
#define TRADING_ENGINE_INCLUDE_ORDER_STATUS_H_


#include <unordered_map>
#include <string>

enum class OrderStatus {
	NEW_ORDER,
	PARTIALLY_FILLED,
	FULFILLED,
	STORED
};

extern std::unordered_map<OrderStatus, std::string> order_status_map;

#endif // !TRADING_ENGINE_INCLUDE_ORDER_STATUS_H_
