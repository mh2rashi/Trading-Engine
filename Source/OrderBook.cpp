#pragma once
#include "Headers/OrderBook.h"


void OrderBook::execute_order(const std::shared_ptr<Order>& incoming_order) {

	// Pick the appropriate Tree
	OrderTree matching_tree = (incoming_order->type == "Buy") ? sell_tree : buy_tree;

	// Match the incoming Order
	matching_tree.match_price_order(incoming_order);

	// Check if the Order has been fully matched, if not we will add it to its appropriate tree
	if (incoming_order->peak_quantity != 0) {

		// Restore original quantity
		incoming_order->restore_peak_quantity();
		// Place in appropriate Tree
		OrderTree storing_tree = (incoming_order->type == "Buy") ? buy_tree : sell_tree;
		storing_tree.add_price_order(incoming_order);
	}

	// Shouldn't this code be within the if condition.
	incoming_order->quantity_transacted = 0;

	return;
}

void OrderBook::cancel_order(const std::shared_ptr<Order>& cancel_order) {

	if (cancel_order->type == "Buy" && buy_tree.order_map.count(cancel_order->id) != 0) {
		buy_tree.delete_price_order(cancel_order);
	}
	else if (cancel_order->type == "Sell" && sell_tree.order_map.count(cancel_order->id) != 0) {
		sell_tree.delete_price_order(cancel_order);
	}
}

void OrderBook::print_real_time_OrderBook() {

}



