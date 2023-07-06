#pragma once
#include "Headers/Order.h"
#include "Headers/LimitOrder.h"
#include "Headers/Transactions.h"

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <unordered_map>
using std::unordered_map;


shared_ptr<Order> LimitOrder::get_head() {
	return head;
}

void LimitOrder::set_head(const std::shared_ptr<Order>& incoming_order) {
	head = incoming_order;
}

void LimitOrder::delete_head() {
	delete_order(head);
}

shared_ptr<Order> LimitOrder::get_tail() {
	return tail;
}

void LimitOrder::set_tail(const std::shared_ptr<Order>& incoming_order) {
	tail = incoming_order;
}

void LimitOrder::delete_tail() {
	delete_order(tail);
}

int LimitOrder::get_length() const {
	return list_length;
}

void LimitOrder::add_order(const std::shared_ptr<Order>& incoming_order) {
	
	//Update LinkedList length
	list_length += 1;

	//First Order
	if (head == nullptr) {
		set_head(incoming_order);
		set_tail(incoming_order);
	}
	// Otherwise, we add an order to the end of the list
	else {
		incoming_order->prev_order = tail;
		tail->next_order = incoming_order;
		tail = incoming_order;
	}
}


void LimitOrder::delete_order(const std::shared_ptr<Order>& order_to_delete) {

	//Update LinkedList length
	list_length -= 1;

	//Remove last element
	if (list_length == 0) {
		head = nullptr;
		tail = nullptr;
	}

	// Orders' the deleted order points to. These will help us identify where the 
	// 'order_to_delete' is located within the LinkedList
	shared_ptr<Order> nxt_order = order_to_delete->next_order;
	shared_ptr<Order> prv_order = order_to_delete->prev_order;

	// Delete order from the middle of the LinkedList
	if (nxt_order != nullptr && prv_order != nullptr) {
		nxt_order->prev_order = prv_order;
		prv_order->next_order = nxt_order;
	}

	// Delete the head
	else if (nxt_order != nullptr) {
		nxt_order->prev_order = nullptr;
		set_head(nxt_order);  // Update the head pointer
	}

	// Delete the tail
	else if (prv_order != nullptr) {
		prv_order->next_order = nullptr;
		set_tail(prv_order);  // Update the tail pointer
	}
}


void LimitOrder::match_order(const std::shared_ptr<Order>& incoming_order, unordered_map<unsigned int, shared_ptr<Order>>& order_dict, Transactions& executed_orders) {
	
	// Out first Order in the LinkedList
	shared_ptr<Order> current_linked_list_order = head;

	// Checking if the incoming order has any unfilled quantity and if the
	// ListLinked has any order for it to match
	while (incoming_order->peak_quantity > 0 && get_length() > 0) {

		bool matched_order = current_linked_list_order->match(incoming_order);

		// Complete trade meaning, the current LinkedList order is fully completed.
		if (matched_order) {
			// Next order within the LinkedList
			shared_ptr<Order> next_current_linked_list_order = current_linked_list_order->next_order;

			// Checking if the current LinkedList order has been completely matched
			if (current_linked_list_order->peak_quantity == 0) {
				// Add order to executed orders list
				executed_orders.add_order(current_linked_list_order->raw_order_info());
				// Delete order from order dictionary
				order_dict.erase(current_linked_list_order->id);
				// Delete order from LinkexList
				delete_order(current_linked_list_order);
			}

			// Next order in LinkedList
			current_linked_list_order = next_current_linked_list_order;

			// Start over if reached the end of the LinkedList
			if (next_current_linked_list_order == nullptr) {
				current_linked_list_order = head;
			}	
		}
	}

	// Adding all the orders in the LinkedList used to complete the incoming order
	shared_ptr<Order> current_order = this->get_head();
	for (; current_order != nullptr; current_order = current_order->next_order) {
		if (current_order->quantity_transacted > 0) {
			executed_orders.add_order(current_order->raw_order_info());
		}
	}

}

