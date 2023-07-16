/* File: order_book.h

Contains declaration for composition class responsible for
managing Buy and Sell OrderTree objects. This is the main class
used to run Order simulations.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_ORDER_BOOK_H_
#define TRADING_ENGINE_SOURCE_HEADERS_ORDER_BOOK_H_

#include "transactions.h"
#include "order.h"
#include "limit_order.h"
#include "order_tree.h"

class OrderBook {
private:
  OrderTree buy_tree_;
  OrderTree sell_tree_;

public:

  /* Constructor */
  OrderBook() : buy_tree_(OrderType::BUY), sell_tree_(OrderType::SELL) {};

  /* Getter functions */
  OrderTree& get_buy_tree();
  OrderTree& get_sell_tree();

  /* Execute incoming_order or store if it's not completelty filled */
  Transactions execute_order(const std::shared_ptr<Order>& incoming_order);

  /* Cancel Order */
  void cancel_order(const std::shared_ptr<Order>& cancel_order);

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_ORDER_BOOK_H_