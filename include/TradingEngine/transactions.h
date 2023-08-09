/*
Contains declaration for Transactions class responsible for storing executed order's, consisting of two vectors.
One for storing orders' in raw format, and the other to store orders' in JSON format to save in database.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_TRANSACTIONS_H_
#define TRADING_ENGINE_SOURCE_HEADERS_TRANSACTIONS_H_

#pragma once
#include <string>
#include <vector>

class Transactions {
private:
  std::vector<std::string> raw_orders;
  std::vector<std::string> json_orders;

public:
	Transactions() : raw_orders({}), json_orders({}) {};

	/* Size of total modified order's */
	size_t getLength() const;

	/* Size of executed order's */
	size_t getExecutedOrders() const;

	void addOrder(const std::string& raw_order);
};
	
#endif // !TRADING_ENGINE_SOURCE_HEADERS_TRANSACTIONS_H_


