#pragma once
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_book.h"

#include <cmath>
#include <memory>
using std::shared_ptr;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\transactions.h" 
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\limit_order.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_tree.h"

/* Return buy tree */
OrderTree& OrderBook::get_buy_tree() {
	return buy_tree_;
}

/* Return sell tree */
OrderTree& OrderBook::get_sell_tree() {
	return sell_tree_;
}

/* Execute incoming_order or store if it's not completelty filled */
Transactions OrderBook::execute_order(const std::shared_ptr<Order>& incoming_order) {

	/* Pick the appropriate Tree */
	OrderTree& matching_tree = (incoming_order->get_type() == OrderType::BUY) ? get_sell_tree() : get_buy_tree();

	/* Match the incoming_order */
	Transactions transaction = matching_tree.match_price_order(incoming_order);

	/* Check if the Order has been fully matched, if not we will add it to its appropriate tree */
	if (incoming_order->get_status() != OrderStatus::FILLED) {

		/* Place in appropriate Tree */
		OrderTree& storing_tree = (incoming_order->get_type() == OrderType::BUY) ? get_buy_tree() : get_sell_tree();
		storing_tree.add_price_order(incoming_order);
	}

	return transaction;
}

/* Cancel Order */
void OrderBook::cancel_order(const std::shared_ptr<Order>& cancel_order) {

	if (cancel_order->get_type() == OrderType::BUY) {
		get_buy_tree().delete_price_order(cancel_order);
	}
	else {
		get_sell_tree().delete_price_order(cancel_order);
	}
}




