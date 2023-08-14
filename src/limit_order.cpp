#pragma once
#include "..\include\TradingEngine\limit_order.h"

#include <memory>
using std::shared_ptr;

#include <sstream>
using std::ostringstream;

#include <unordered_set>
using std::unordered_set;

#include "..\include\TradingEngine\transactions.h" 
#include "..\include\TradingEngine\order_type.h"
#include "..\include\TradingEngine\order_status.h"
#include "..\include\TradingEngine\order.h"

const shared_ptr<Order>& LimitOrder::getHead() {
	return limit_order_head;
}

void LimitOrder::setHead(const std::shared_ptr<Order>& incoming_order) {
	limit_order_head = incoming_order;
}

const shared_ptr<Order>& LimitOrder::getTail() {
	return limit_order_tail;
}

void LimitOrder::setTail(const std::shared_ptr<Order>& incoming_order) {
	limit_order_tail = incoming_order;
}

int LimitOrder::getListLength() const {
	return limit_order_length;
}

int LimitOrder::updateListLength(const int& update_increment) {
	return limit_order_length += update_increment;
}

void LimitOrder::addOrder(Order& incoming_order) {

	/* Create pointer to incoming order */
	const std::shared_ptr<Order>& incoming_order_ptr = std::make_shared<Order>(incoming_order);

	/* Increment LinkedList length. */
	limit_order_length++;

	/* First Order */
	if (limit_order_length == 1) {
		limit_order_head = incoming_order_ptr;
		limit_order_tail = incoming_order_ptr;
	}
	/* Otherwise, we add an order to the end of the list */
	else {
		incoming_order_ptr->prev_order = limit_order_tail;
		limit_order_tail->next_order = incoming_order_ptr;
		limit_order_tail = incoming_order_ptr;
	}

}

void LimitOrder::deleteOrder(const std::shared_ptr<Order>& order_to_delete) {

	/* Decrement LinkedList length. */
	limit_order_length--;

	/* Remove last element. */
	if (limit_order_length - 1 <= 0) {
		limit_order_head = nullptr;
		limit_order_tail = nullptr;
		return;
	}

	/* These are order's that 'order_to_delete' points to. They help
	us identify where the 'order_to_delete' is located within the LinkedList. */
	const shared_ptr<Order>& nxt_order = order_to_delete->next_order;
	const shared_ptr<Order>& prv_order = order_to_delete->prev_order;

	/* Delete 'order_to_delete' from the middle of the LinkedList */
	if (nxt_order != nullptr && prv_order != nullptr) {
		nxt_order->prev_order = prv_order;
		prv_order->next_order = nxt_order;
	}

	/* Delete the head */
	else if (nxt_order != nullptr) {
		nxt_order->prev_order = nullptr;
		limit_order_head = nxt_order;  /* Update the head pointer. */
	}

	/* Delete the tail */
	else if (prv_order != nullptr) {
		prv_order->next_order = nullptr;
		limit_order_tail = prv_order;  /* Update the tail pointer. */
	}

}

void LimitOrder::matchOrder(Order& incoming_order, unordered_set<unsigned int>& order_set, ostringstream& executed_orders) {

	/* The first Order in the LinkedList to start matching with */
	shared_ptr<Order> current_linked_list_order = limit_order_head;

	/* We will match the incoming_order with Order's in our LinkedList until the incoming_order
	is complete or the LinkedList is empty. The first if condition indicates that the incoming_order
	is partially filled and the else condition indicates that the incoming_order is completely filled. */
	while (incoming_order.getPeakQuantity() > 0 && limit_order_length > 0) {

		bool matched_order = current_linked_list_order->Match(incoming_order);

		/* Complete trade, meaning the current LinkedList order is fully completed. */
		if (matched_order) {
			/* Next order within the LinkedList. */
			shared_ptr<Order> next_current_linked_list_order = current_linked_list_order->next_order;

			/* Store fulfilled order */
			current_linked_list_order->storeOrderInfo(executed_orders, "\033[32m");
			
			/* Delete LinkedList order from order set */
			order_set.erase(current_linked_list_order->getID());
			/* Delete order from LinkedList. */
			deleteOrder(current_linked_list_order);
			/* Release the memory as order is no longer in use */
			current_linked_list_order.reset();

			/* Next order in LinkedList. */
			current_linked_list_order = next_current_linked_list_order;
		}
	}
	return;
}
