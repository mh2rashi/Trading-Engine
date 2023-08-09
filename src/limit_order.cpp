#pragma once
#include "..\include\TradingEngine\limit_order.h"

#include <memory>
using std::shared_ptr;

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
	updateListLength(1);

	/* First Order */
	if (getListLength() == 1) {
		setHead(incoming_order_ptr);
		setTail(incoming_order_ptr);
	}
	/* Otherwise, we add an order to the end of the list */
	else {
		incoming_order_ptr->prev_order = getTail();
		getTail()->next_order = incoming_order_ptr;
		setTail(incoming_order_ptr);
	}
}

void LimitOrder::deleteOrder(const std::shared_ptr<Order>& order_to_delete) {

	/* Decrement LinkedList length. */
	updateListLength(-1);

	/* Remove last element. */
	if (getListLength() <= 0) {
		setHead(nullptr);
		setTail(nullptr);
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
		setHead(nxt_order);  /* Update the head pointer. */
	}

	/* Delete the tail */
	else if (prv_order != nullptr) {
		prv_order->next_order = nullptr;
		setTail(prv_order);  /* Update the tail pointer. */
	}

}

void LimitOrder::matchOrder(Order& incoming_order, unordered_set<unsigned int>& order_set, Transactions& executed_orders) {

	/* The first Order in the LinkedList to start matching with */
	shared_ptr<Order> current_linked_list_order = getHead();

	/* We will match the incoming_order with Order's in our LinkedList until the incoming_order
	is complete or the LinkedList is empty. The first if condition indicates that the incoming_order
	is partially filled and the else condition indicates that the incoming_order is completely filled. */
	while (incoming_order.getPeakQuantity() > 0 && getListLength() > 0) {

		bool matched_order = current_linked_list_order->Match(incoming_order);

		/* Complete trade, meaning the current LinkedList order is fully completed. */
		if (matched_order) {
			/* Next order within the LinkedList. */
			shared_ptr<Order> next_current_linked_list_order = current_linked_list_order->next_order;

			/* Add order transaction info */
			executed_orders.addOrder(current_linked_list_order->orderInfo());
			executed_orders.addOrder(incoming_order.orderInfo());
			
			/* Delete LinkedList order from order dictionary. */
			order_set.erase(current_linked_list_order->getID());
			/* Delete order from LinkedList. */
			deleteOrder(current_linked_list_order);
			/* Release the memory as order is no longer in use */
			current_linked_list_order.reset();

			/* Next order in LinkedList. */
			current_linked_list_order = next_current_linked_list_order;
		}

		/* Partial trade, meaning the incoming_order is fully completed. */
		else if (current_linked_list_order->quantity_transacted > 0 && incoming_order.quantity_transacted > 0) {
			executed_orders.addOrder(current_linked_list_order->orderInfo());
			executed_orders.addOrder(incoming_order.orderInfo());
		}
	}
}
