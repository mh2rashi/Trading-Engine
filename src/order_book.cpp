#pragma once
#include "..\include\TradingEngine\order_book.h"

#include <cmath>

#include <memory>
using std::shared_ptr;

#include "..\include\TradingEngine\transactions.h" 
#include "..\include\TradingEngine\order_type.h"
#include "..\include\TradingEngine\order_status.h"
#include "..\include\TradingEngine\order.h"
#include "..\include\TradingEngine\limit_order.h"
#include "..\include\TradingEngine\order_tree.h"

OrderTree& OrderBook::getBuyTree() {
	return buy_tree;
}

OrderTree& OrderBook::getSellTree() {
	return sell_tree;
}

/* Execute incoming_order or store if it's not completelty filled */
Transactions OrderBook::processOrder(Order& incoming_order) {

	/* Pick the appropriate Tree */
	OrderTree& matching_tree = (incoming_order.getType() == OrderType::BUY) ? getSellTree() : getBuyTree();

	/* Match the incoming_order */
	Transactions transaction = matching_tree.matchPriceOrder(incoming_order);

	/* Check if the Order has been fully matched, if not we will add it to its appropriate tree */
	if (incoming_order.getStatus() != OrderStatus::FILLED) {

		/* Place in appropriate Tree */
		OrderTree& storing_tree = (incoming_order.getType() == OrderType::BUY) ? getBuyTree() : getSellTree();
		storing_tree.addPriceOrder(incoming_order);
	}

	return transaction;
}

void OrderBook::cancelOrder(const std::shared_ptr<Order>& cancel_order) {

	if (cancel_order->getType() == OrderType::BUY) {
		getBuyTree().deletePriceOrder(cancel_order);
	}
	else {
		getSellTree().deletePriceOrder(cancel_order);
	}
}