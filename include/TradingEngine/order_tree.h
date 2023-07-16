/* File: order_tree.h

Contains declaration for composition class responsible for managing objects of LimitOrder class 
through a balanced binary search tree (red-black tree). The Tree will be sorted based on Price only, 
other factors like event time are not considered. The class also contains data structures for efficient 
access to LimitOrder and Order objects.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_ORDER_TREE_H_
#define TRADING_ENGINE_SOURCE_HEADERS_ORDER_TREE_H_

#pragma once
#include <map>
#include <unordered_map>
#include <memory>

#include "transactions.h"
#include "order.h"
#include "limit_order.h"


class OrderTree {
private:
  std::map<float, int> order_tree_; /* Map (red-black tree) with LimitOrder price as key and LimitOrder length as value */

  std::unordered_map<unsigned int, std::shared_ptr<Order>> order_map_; /* Unordered map with Order id as key and pointer to Order as value */

  std::unordered_map<float, LimitOrder> price_map_; /* Unordered map with LimitOrder price as key and LimitOrder as value */

  float min_price_;
  float max_price_;
  OrderType type_;

public:
  /* Constructor */
  OrderTree(OrderType tree_type) : order_tree_({}), order_map_({}), price_map_({}), min_price_(std::numeric_limits<float>::quiet_NaN()), max_price_(std::numeric_limits<float>::quiet_NaN()), type_(tree_type) {};

  /* Length of Order Tree, measured as total orders stored within the order_map */
  int get_length() const;

  /* Min properties */
  float get_min();
  void set_min(const float& new_min);

  /* Max properties */
  float get_max();
  void set_max(const float& new_max);

  /* Return Tree type */
  const OrderType& get_type() const;

  /* Add Order into Tree */
  void add_price_order(const std::shared_ptr<Order>& incoming_order);

  /* Delete Order from Tree */
  void delete_price_order(const std::shared_ptr<Order>& order_to_delete);

  /* Add new limit_order into Tree */
  void add_limit_price(const float& limit_price);

 /* Delete limit_order from Tree, price_map and order_map */
  void delete_limit_price(const float& limit_price);

/* Match incoming_order */
  Transactions match_price_order(const std::shared_ptr<Order>& incoming_order);

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_ORDER_TREE_H_