/*
Contains declaration for OrderStatus enum, OrderType enum and Order class.
OrderStatus and OrderType are properties of member variable's for the Order class.
Order class represents a data structure for handling incoming orders'.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_ORDER_H_
#define TRADING_ENGINE_SOURCE_HEADERS_ORDER_H_

#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "..\include\TradingEngine\order_type.h"
#include "..\include\TradingEngine\order_status.h"


class Order {
public:
  std::string symbol;
  unsigned int id;
	
  OrderType type;
	
  float price;
  float original_quantity;

  OrderStatus order_status;

  float peak_quantity; /* Current quantity remaining to be transacted.*/
  float quantity_transacted; /* Quantity transacted in current transaction.*/
  float total_quantity_transacted; /* Total quantity transacted including all transactions.*/

  std::shared_ptr<Order> next_order; /* Order's are maintined in a doubly linked-list where each Order points to a potential next and previous Order. */
  std::shared_ptr<Order> prev_order; 

  Order(std::string input_symbol, unsigned int input_id, OrderType input_type, float input_price, float input_original_quantity);

  /* Getter functions. */
  const std::string& getSymbol();
  const unsigned int& getID() const;
  const OrderType& getType() const; 
  const float& getPrice() const; 
  const OrderStatus& getStatus() const; 
  const float& getOriginalQuantity() const; 
  const float& getQuantityTransacted() const;
  const float& getPeakQuantity() const;
  const float& getTotalQuantityTransacted() const;

  /* Setter functions. */
  void setOrderStatus(const OrderStatus& new_status);
  void setTotalQuantityTransacted(const float& input_quantity);
  void setQuantityTransacted(const float& input_quantity);
  void setPeakQuantity(const float& input_quantity);

  /* Matches partial or complete orders.
  Returns true for a complete trade and false for a partial trade. */
  bool Match(Order& incoming_oder);

  /* Executes the matched order, and updates Order quantities. */
  void executeTransaction(const float& input_transaction_quantity);

  /* Store order information. */
  void storeOrderInfo(std::ostringstream& output_data, const std::string& input_color);

};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_ORDER_H_
