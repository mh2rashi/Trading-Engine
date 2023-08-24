/*
Contains declaration for OrderType enum and Order type map.
OrderType enum is a property of the Order class and Order
type map is used for string representation of Order info.
*/

#ifndef TRADING_ENGINE_INCLUDE_ORDER_TYPE_H_
#define TRADING_ENGINE_INCLUDE_ORDER_TYPE_H_


#include <unordered_map>
#include <string>

enum class OrderType {
	BUY,
	SELL,
	CANCEL
};

extern std::unordered_map<OrderType, std::string> order_type_map;

#endif // !TRADING_ENGINE_INCLUDE_ORDER_TYPE_H_
