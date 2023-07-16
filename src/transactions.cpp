#pragma once
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\transactions.h"

#include <string>
using std::string;

#include <vector>
using std::vector;

/* Size of total modified order's */
int Transactions::get_length() const {
	return raw_orders.size();
}

/* Size of executed order's */
int Transactions::get_executed_orders() const {
	return get_length() / 2;
}

/* Add executed order's */
void Transactions::add_order(const std::string& raw_order) {
	raw_orders.emplace_back(raw_order);
}
