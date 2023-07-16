#pragma once
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"

#include <algorithm>
using std::min;

#include <chrono>
using std::chrono::system_clock;

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::shared_ptr;

#include <string>
using std::string;
using std::to_string;

/* Order status map. */
std::unordered_map<OrderStatus, std::string> order_status_map = {
	{OrderStatus::NEW_ORDER, "New Order"},
	{OrderStatus::PARTIALLY_FILLED, "Partially Filled"},
	{OrderStatus::FILLED, "Filled"}
};

/* Order type map. */
std::unordered_map<OrderType, std::string> order_type_map = {
	{OrderType::BUY, "Buy"},
	{OrderType::SELL, "Sell"}
};

/* Constructor with custom input_id_ */
Order::Order(unsigned int input_id, OrderType input_type, float input_price, float input_original_quantity): 
	id_(input_id), type_(input_type), price_(input_price), original_quantity_(input_original_quantity){
	
	order_status = OrderStatus::NEW_ORDER; 

	peak_quantity = input_original_quantity;
	peak_quantity_left = original_quantity_ - peak_quantity;
	quantity_transacted = 0;
	total_quantity_transacted = 0;
	 
	order_time = system_clock::now();

	next_order = nullptr;
	prev_order = nullptr;
	 
}

///* Constructor with UUID for input_id_ */
//Order::Order(OrderType input_type, float input_price, float input_original_quantity): // need to remove input ID and make it an input type enum value
//	type_(input_type), price_(input_price), original_quantity_(input_original_quantity) {
//	
//	//input_id_;
//	order_status = OrderStatus::NEW_ORDER;
//
//	peak_quantity = input_original_quantity;
//	peak_quantity_left = original_quantity_ - peak_quantity;
//	quantity_transacted = 0;
//	total_quantity_transacted = 0;
//
//	order_time = system_clock::now();
//
//	next_order = nullptr;
//	prev_order = nullptr;
//
//}

/* Return Order id. */
const unsigned int& Order::get_id() const {
	return id_;
}

/* Return Order type. */
const OrderType& Order::get_type() const {
	return type_;
}

/* Return Order price. */
const float& Order::get_price() const {
	return price_;
}

/* Return Order status. */
const OrderStatus& Order::get_status() const {
	return order_status;
}

/* Return Order original quantity. */
const float& Order::get_original_quantity() const {
	return original_quantity_;
}

/* Return Order quantity transacted. */
const float& Order::get_quantity_transacted() const {
	return quantity_transacted;
}

/* Return Order peak transacted. */
const float& Order::get_peak_quantity() const {
	return peak_quantity;
}

/* Return Order total quantity transacted. */
const float& Order::get_total_quantity_transacted() const {
	return total_quantity_transacted;
}

/* Update Order status. */
void Order::set_order_status(const OrderStatus& new_status) {
	order_status = new_status;
}

/* Update Order total quantity transacted. */
void Order::set_total_quantity_transacted(const float& input_quantity) {
	total_quantity_transacted += input_quantity;
}

/* Update Order quantity transacted. */
void Order::set_quantity_transacted(const float& input_quantity) {
	quantity_transacted = input_quantity;
}

/* Update Order quantity transacted. */
void Order::set_peak_quantity(const float& input_quantity) {
	peak_quantity += input_quantity;
}

/* A match is a complete trade if the current order's quantity is less than or equal 
to the incoming_order's quantity. A match is a partial trade if the incoming_order's quantity
is greater than the current order's quantity. */
bool Order::match(const std::shared_ptr<Order>& incoming_order){

	/* Complete trade. */
	if (get_peak_quantity() <= incoming_order->get_peak_quantity()) {

		/* Update Order status. */
		set_order_status(OrderStatus::FILLED);
		if (get_peak_quantity() == incoming_order->get_peak_quantity()) {
			incoming_order->set_order_status(OrderStatus::FILLED);
		}
		else {
			incoming_order->set_order_status(OrderStatus::PARTIALLY_FILLED);
		}

		// Execute trade for both orders
		auto transaction_quantity = peak_quantity;
		this->execute_transaction(transaction_quantity);
		incoming_order->execute_transaction(transaction_quantity);

		return true;
	}
	/* Partial trade. */
	else {

		/* Update Order status. */
		set_order_status(OrderStatus::PARTIALLY_FILLED);
		incoming_order->set_order_status(OrderStatus::FILLED);

		// Execute trade for both orders
		auto transaction_quantity = incoming_order->peak_quantity;
		this->execute_transaction(transaction_quantity);
		incoming_order->execute_transaction(transaction_quantity);

		return false;
	}
}

/* Execute the matched order, and update quantities for the Order. */
void Order::execute_transaction(float const& intput_transaction_quantity) {
	
	set_total_quantity_transacted(intput_transaction_quantity);
	set_quantity_transacted(intput_transaction_quantity);
	set_peak_quantity(-intput_transaction_quantity);
}

/* Order information details. */
std::string Order::order_info() const {
	return "Order:- ID: " + to_string(get_id()) +
		", Type: " + order_type_map[get_type()] +
		", Price: " + to_string(get_price()) +
		", Order_status: " + order_status_map[get_status()] +
		", Original_quantity: " + to_string(get_original_quantity()) +
		", Transacted_quantity: " + to_string(get_quantity_transacted()) +
		", Quantity_left: " + to_string(get_peak_quantity()) +
		", Total_transacted_quantity: " + to_string(get_total_quantity_transacted());
}