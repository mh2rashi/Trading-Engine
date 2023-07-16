/* File: limit_order.h

Contains declaration of composition class responsible for managing objects of Order
class through a doubly LinkedList. The LinkedList ensures that Orders are added and removed
in accordance with the FIFO (first-in, first-out) principle. New Orders are added to the tail
of the LinkedList, while Order matching is performed from the head, maintaining the
desired order.*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_LIMITORDER_H_
#define TRADING_ENGINE_SOURCE_HEADERS_LIMITORDER_H_

#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "transactions.h"
#include "order.h"

class LimitOrder {
private:
  std::shared_ptr<Order> head_;
  std::shared_ptr<Order> tail_;
  int list_length_;

public:
	/* Constructor. */
	LimitOrder() : head_(nullptr), tail_(nullptr), list_length_(0) {};

	/* Head properties. */
	std::shared_ptr<Order>& get_head();
	void set_head(const std::shared_ptr<Order>& incoming_order);

	/* Tail properties. */
	std::shared_ptr<Order>& get_tail();
	void set_tail(const std::shared_ptr<Order>& incoming_order);

	/* Returns the length of the LinkedList. */
	int get_list_length() const;

	/* Updates the length of the LinkedList. */
	int update_list_length(const int& update_increment) ;

	/* Adds an incoming Order, and stores it as the tail of the LinkedList. */
	void add_order(const std::shared_ptr<Order>& incoming_order);

	/* Deletes an Order from the LinkedList. */
	void delete_order(const std::shared_ptr<Order>& order_to_delete);

	/* Matches Order starting from the head of the LinkedList.
	Updates Order dictionary by removing the Order and appending Order details to the executed_orders upon a successful match. */
	void match_order(const std::shared_ptr<Order>& incoming_order, std::unordered_map<unsigned int, std::shared_ptr<Order>>& order_dict, Transactions& executed_orders);

	/* Print LimitOrder details. */
	void print_limitOrder();

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_LIMITORDER_H_

