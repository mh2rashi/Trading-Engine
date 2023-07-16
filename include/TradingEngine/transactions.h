/* File: transactions.h

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
	/* Constructor */
	Transactions() : raw_orders({}), json_orders({}) {};

	/* Size of total modified order's */
	int get_length() const;

	/* Size of executed order's */
	int get_executed_orders() const;

	/* Add executed order's */
	void add_order(const std::string& raw_order);
};
	
#endif // !TRADING_ENGINE_SOURCE_HEADERS_TRANSACTIONS_H_


