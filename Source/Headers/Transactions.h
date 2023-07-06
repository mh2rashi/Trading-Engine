#pragma once
#include <string>
#include <vector>

/*Class to store executed orders, Consisting of two vectors. One for storing orders' in raw format,
and the other to store orders' in JSON format to save in database.*/

struct Transactions {

	std::vector<std::string> raw_orders;
	std::vector<std::string> json_orders;

	// Constructor
	Transactions() : raw_orders({}), json_orders({}) {};

	// Size of executed orders
	int get_length() const;

	// Add exectued orders
	void add_order(const std::string& raw_order);
};
	


