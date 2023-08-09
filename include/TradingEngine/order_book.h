/*
Contains declaration for composition class responsible for
managing Buy and Sell OrderTree objects. This is the main class
used to run Order simulations.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_ORDER_BOOK_H_
#define TRADING_ENGINE_SOURCE_HEADERS_ORDER_BOOK_H_


#include "transactions.h"
#include "order_type.h"
#include "order_status.h"
#include "order.h"
#include "limit_order.h"
#include "order_tree.h"

class OrderBook {
private:
  OrderTree buy_tree;
  OrderTree sell_tree;

public:
  OrderBook() : buy_tree(OrderType::BUY), sell_tree(OrderType::SELL) {};

  /* Getter functions */
  OrderTree& getBuyTree();
  OrderTree& getSellTree();

  /* Execute incoming_order or store if it's not completelty filled */
  Transactions processOrder(Order& incoming_order);

  void cancelOrder(const std::shared_ptr<Order>& cancel_order);

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_ORDER_BOOK_H_