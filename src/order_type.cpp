#pragma once
#include "..\include\TradingEngine\order_type.h"

#include<unordered_map>
using std::unordered_map;

#include <string>
using std::string;


unordered_map<OrderType, string> order_type_map = {
	{OrderType::BUY, "Buy"},
	{OrderType::SELL, "Sell"}
};