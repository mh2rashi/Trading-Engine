#pragma once
#include "..\include\TradingEngine\order.h"

#include <algorithm>
using std::min;

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::shared_ptr;

#include <string>
using std::string;
using std::to_string;


#include "..\include\TradingEngine\order_type.h"
#include "..\include\TradingEngine\order_status.h"

Order::Order(unsigned int input_id, OrderType input_type, float input_price, float input_original_quantity): 
	id(input_id), type(input_type), price(input_price), original_quantity(input_original_quantity){
	
	order_status = OrderStatus::NEW_ORDER; 

	peak_quantity = input_original_quantity;
	quantity_transacted = 0;
	total_quantity_transacted = 0;

	next_order = nullptr;
	prev_order = nullptr;
	 
}

const unsigned int& Order::getID() const {
	return id;
}

const OrderType& Order::getType() const {
	return type;
}

const float& Order::getPrice() const {
	return price;
}

const OrderStatus& Order::getStatus() const {
	return order_status;
}

const float& Order::getOriginalQuantity() const {
	return original_quantity;
}

const float& Order::getQuantityTransacted() const {
	return quantity_transacted;
}

const float& Order::getPeakQuantity() const {
	return peak_quantity;
}

const float& Order::getTotalQuantityTransacted() const {
	return total_quantity_transacted;
}

void Order::setOrderStatus(const OrderStatus& new_status) {
	order_status = new_status;
}

void Order::setTotalQuantityTransacted(const float& input_quantity) {
	total_quantity_transacted += input_quantity;
}

void Order::setQuantityTransacted(const float& input_quantity) {
	quantity_transacted = input_quantity;
}

void Order::setPeakQuantity(const float& input_quantity) {
	peak_quantity += input_quantity;
}

/* A match is a complete trade if the current order's quantity is less than or equal 
to the incoming_order's quantity. A match is a partial trade if the incoming_order's quantity
is greater than the current order's quantity. */
bool Order::Match(Order& incoming_order){

	/* Complete trade. */
	if (getPeakQuantity() <= incoming_order.getPeakQuantity()) {

		/* Update Order status. */
		setOrderStatus(OrderStatus::FILLED);
		if (getPeakQuantity() == incoming_order.getPeakQuantity()) {
			incoming_order.setOrderStatus(OrderStatus::FILLED);
		}
		else {
			incoming_order.setOrderStatus(OrderStatus::PARTIALLY_FILLED);
		}

		// Execute trade for both orders
		auto transaction_quantity = peak_quantity;
		this->executeTransaction(transaction_quantity);
		incoming_order.executeTransaction(transaction_quantity);

		return true;
	}
	/* Partial trade. */
	else {

		/* Update Order status. */
		setOrderStatus(OrderStatus::PARTIALLY_FILLED);
		incoming_order.setOrderStatus(OrderStatus::FILLED);

		// Execute trade for both orders
		auto transaction_quantity = incoming_order.peak_quantity;
		this->executeTransaction(transaction_quantity);
		incoming_order.executeTransaction(transaction_quantity);

		return false;
	}
}

/* Execute the matched order, and update quantities for the Order. */
void Order::executeTransaction(const float& input_transaction_quantity) {
	
	setTotalQuantityTransacted(input_transaction_quantity);
	setQuantityTransacted(input_transaction_quantity);
	setPeakQuantity(-input_transaction_quantity);
}

/* Order information details. */
std::string Order::orderInfo() const {
	return "Order:- ID: " + to_string(getID()) +
		", Type: " + order_type_map[getType()] +
		", Price: " + to_string(getPrice()) +
		", Order_status: " + order_status_map[getStatus()] +
		", Original_quantity: " + to_string(getOriginalQuantity()) +
		", Transacted_quantity: " + to_string(getQuantityTransacted()) +
		", Quantity_left: " + to_string(getPeakQuantity()) +
		", Total_transacted_quantity: " + to_string(getTotalQuantityTransacted());
}