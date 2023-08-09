#pragma once
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\transactions.h"

#include <string>
using std::string;

#include <vector>
using std::vector;

/* Size of total modified order's */
size_t Transactions::getLength() const {
	return raw_orders.size();
}

/* Size of executed order's */
size_t Transactions::getExecutedOrders() const {
	return getLength() / 2;
}

void Transactions::addOrder(const std::string& raw_order) {
	raw_orders.emplace_back(raw_order);
}