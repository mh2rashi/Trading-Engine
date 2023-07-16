/* File: order.h

Contains declaration for OrderStatus enum, OrderType enum and Order class.
OrderStatus and OrderType are properties of member variable's for the Order class.
Order class represents a data structure for handling order's that are submitted.
*/

#ifndef TRADING_ENGINE_SOURCE_HEADERS_ORDER_H_
#define TRADING_ENGINE_SOURCE_HEADERS_ORDER_H_

#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include <memory>
#include <unordered_map>


enum class OrderStatus {
  NEW_ORDER,
  PARTIALLY_FILLED,
  FILLED             
};

extern std::unordered_map<OrderStatus, std::string> order_status_map;

enum class OrderType {
  BUY,
  SELL
};

extern std::unordered_map<OrderType, std::string> order_type_map;

class Order {
private:
  const unsigned int id_;
	
  const OrderType type_;
	
  const float price_;
  const float original_quantity_;

public:
  OrderStatus order_status;

  float peak_quantity; /* Current quantity remaining to be transacted.*/
  float peak_quantity_left;
  float quantity_transacted; /* Quantity transacted in current transaction.*/
  float total_quantity_transacted; /*Total quantity transacted including all transactions.*/

  std::chrono::time_point<std::chrono::system_clock> order_time; /* Order submission time.*/

  std::shared_ptr<Order> next_order; /* Order's are maintined in a doubly linked-list where each Order points to the next and previous Order. */
  std::shared_ptr<Order> prev_order;

  /* Two constructor declarations with the option to assign an Order id. */
  Order(unsigned int input_id, OrderType input_type, float input_price, float input_original_quantity);
  //Order(std::string input_type, float input_price, float input_original_quantity);

  /* Getter functions. */
  const unsigned int& get_id() const;
  const OrderType& get_type() const; 
  const float& get_price() const; 
  const OrderStatus& get_status() const; 
  const float& get_original_quantity() const; 
  const float& get_quantity_transacted() const;
  const float& get_peak_quantity() const;
  const float& get_total_quantity_transacted() const;

  /* Setter functions. */
  void set_order_status(const OrderStatus& new_status);
  void set_total_quantity_transacted(const float& input_quantity);
  void set_quantity_transacted(const float& input_quantity);
  void set_peak_quantity(const float& input_quantity);

  /* Matches partial or complete orders.
  Returns true for a complete trade and false for a partial trade. */
  bool match(const std::shared_ptr<Order>& incoming_oder);

  /* Executes the matched order, and updates Order quantities. */
  void execute_transaction(float const& input_quantity);

  /* Prints Order details. */
  std::string order_info() const;
};

#endif // !TRADING_ENGINE_SOURCE_HEADERS_ORDER_H_