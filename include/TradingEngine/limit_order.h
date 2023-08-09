/*
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
#include <unordered_set>

#include "transactions.h"
#include "order_type.h"
#include "order_status.h"
#include "order.h"

class LimitOrder {
private:
	std::shared_ptr<Order> limit_order_head;
	std::shared_ptr<Order> limit_order_tail;
	int limit_order_length;

public:

	LimitOrder() : limit_order_head(nullptr), limit_order_tail(nullptr), limit_order_length(0) {};

	const std::shared_ptr<Order>& getHead();
	void setHead(const std::shared_ptr<Order>& incoming_order);

	const std::shared_ptr<Order>& getTail();
	void setTail(const std::shared_ptr<Order>& incoming_order);

	int getListLength() const;

	int updateListLength(const int& update_increment);

	/* Adds an incoming Order, and stores it as the tail of the LinkedList. */
	void addOrder(Order& incoming_order);

	/* Deletes an Order from the LinkedList. */
	void deleteOrder(const std::shared_ptr<Order>& order_to_delete);

	/* Matches Order starting from the head of the LinkedList.
	Updates Order dictionary by removing the Order and appending Order details to the executed_orders upon a successful match. */
	void matchOrder(Order& incoming_order, std::unordered_set<unsigned int>& order_set, Transactions& executed_orders);

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_LIMITORDER_H_