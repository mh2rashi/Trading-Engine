#pragma once
#include "Headers/Transactions.h"

#include <string>
using std::string;

#include <vector>
using std::vector;


int Transactions::get_length() const {
	return raw_orders.size();
}

void Transactions::add_order(const std::string& raw_order) {
	raw_orders.emplace_back(raw_order);
}

