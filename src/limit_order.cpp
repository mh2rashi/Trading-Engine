#pragma once
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\limit_order.h"

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::shared_ptr;

#include <unordered_map>
using std::unordered_map;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\transactions.h" 

/* Return LinkedList head. */
shared_ptr<Order>& LimitOrder::get_head() {
	return head_;
}

/* Set LinkedList head. */
void LimitOrder::set_head(const std::shared_ptr<Order>& incoming_order) {
	head_ = incoming_order;
}

/* Return LinkedList tail. */
shared_ptr<Order>& LimitOrder::get_tail() {
	return tail_;
}

/* Set LinkedList tail. */
void LimitOrder::set_tail(const std::shared_ptr<Order>& incoming_order) {
	tail_ = incoming_order;
}

/* Return LinkedList length. */
int LimitOrder::get_list_length() const {
	return list_length_;
}

/* Update LinkedList length. */
int LimitOrder::update_list_length(const int& update_increment) {
	return list_length_ += update_increment;
}

/* Add incoming Order to LinkedList. */
void LimitOrder::add_order(const std::shared_ptr<Order>& incoming_order) {
	
	/* Increment LinkedList length. */
	update_list_length(1);

	/* First Order */
	if (get_head() == nullptr) {
		set_head(incoming_order);
		set_tail(incoming_order);
	}
	/* Otherwise, we add an order to the end of the list */
	else {
		incoming_order->prev_order = get_tail();
		get_tail()->next_order = incoming_order;
		set_tail(incoming_order);
	}
}

/* Delete Order from LinkedList. */
void LimitOrder::delete_order(const std::shared_ptr<Order>& order_to_delete) {

	/* Decrement LinkedList length. */
	if (get_list_length() > 0) {
		update_list_length(-1);
	}
	
	/* Remove last element. */
	if (get_list_length() == 0) {
		set_head(nullptr);
		set_tail(nullptr);
	}

	/* These are order's that 'order_to_delete' points to. They help 
	us identify where the 'order_to_delete' is located within the LinkedList. */
	shared_ptr<Order> nxt_order = order_to_delete->next_order;
	shared_ptr<Order> prv_order = order_to_delete->prev_order;

	/* Delete 'order_to_delete' from the middle of the LinkedList */
	if (nxt_order != nullptr && prv_order != nullptr) {
		nxt_order->prev_order = prv_order;
		prv_order->next_order = nxt_order;
	}

	/* Delete the head */
	else if (nxt_order != nullptr) {
		nxt_order->prev_order = nullptr;
		set_head(nxt_order);  /* Update the head pointer. */
	}

	/* Delete the tail */
	else if (prv_order != nullptr) {
		prv_order->next_order = nullptr;
		set_tail(prv_order);  /* Update the tail pointer. */
	}
}


void LimitOrder::match_order(const std::shared_ptr<Order>& incoming_order, std::unordered_map<unsigned int, shared_ptr<Order>>& order_dict, Transactions& executed_orders) {
	
	/* The first Order in the LinkedList to start matching with */
	shared_ptr<Order> current_linked_list_order = get_head();

	/* We will match the incoming_order with Order's in our LinkedList until the incoming_order
	is complete or the LinkedList is empty. The first if condition indicates that the incoming_order
	is partially filled and the else condition indicates that the incoming_order is completely filled. */
	while (incoming_order->get_peak_quantity() > 0 && get_list_length() > 0) {

		bool matched_order = current_linked_list_order->match(incoming_order);

		/* Complete trade, meaning the current LinkedList order is fully completed. */
		if (matched_order) {
			/* Next order within the LinkedList. */
			shared_ptr<Order> next_current_linked_list_order = current_linked_list_order->next_order;

			/* Checking if the current LinkedList order has been completely matched. */
			if (current_linked_list_order->get_peak_quantity() == 0) {
				/* Add order to executed_orders list. */
				executed_orders.add_order(current_linked_list_order->order_info());
				executed_orders.add_order(incoming_order->order_info());
				/* Delete LinkedList order from order dictionary. */
				order_dict.erase(current_linked_list_order->get_id());
				/* Delete order from LinkedList. */
				delete_order(current_linked_list_order);
			}

			/* Next order in LinkedList. */
			current_linked_list_order = next_current_linked_list_order;

			/* Start over if reached the end of the LinkedList. */
			if (next_current_linked_list_order == nullptr) {
				current_linked_list_order = get_head();
			}	
		}

		/* Partial trade, meaning the incoming_order is fully completed. */
		else if (current_linked_list_order->quantity_transacted > 0 && incoming_order->quantity_transacted > 0) {
			executed_orders.add_order(current_linked_list_order->order_info());
			executed_orders.add_order(incoming_order->order_info());
		}
	}
}

/* Printing Orders in LimitOrder doubly LinkedList. */
void LimitOrder::print_limitOrder() {
	shared_ptr<Order> current_order = get_head();
	for (; current_order != nullptr; current_order = current_order->next_order) {
			cout << current_order->order_info() << endl;
		}
	}

