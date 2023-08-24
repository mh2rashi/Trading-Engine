#pragma once
#include "..\include\TradingEngine\order_status.h"

#include<unordered_map>
using std::unordered_map;

#include <string>
using std::string;

unordered_map<OrderStatus, string> order_status_map = {
	{OrderStatus::NEW_ORDER,"NEW ORDER"},
	{OrderStatus::PARTIALLY_FILLED,"PARTIALLY FILLED"},
	{OrderStatus::FULFILLED,"FULFILLED"},
	{OrderStatus::STORED,"STORED"}
};
