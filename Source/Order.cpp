#pragma once
#include "Headers/Order.h"

#include <algorithm>
using std::min;

#include <chrono>
using std::chrono::system_clock;

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::shared_ptr;

// Constructor
Order::Order(unsigned int input_id, std::string input_type, float input_price, float input_original_quantity):
	id(input_id), type(input_type), price(input_price), original_quantity(input_original_quantity){
	order_status = OrderStatus::NewOrder; 

	peak_quantity = input_original_quantity;
	peak_quantity_left = original_quantity - peak_quantity;
	quantity_transacted = 0;

	order_time = system_clock::now();

	next_order = nullptr;
	prev_order = nullptr;
	 
}

// Match partial or complete orders
bool Order::match(const std::shared_ptr<Order>& incoming_order){

	// Can't match two of the same order types
	if (incoming_order->type == this->type) {
		return false;
	}

	// Complete trade
	if (this->peak_quantity <= incoming_order->peak_quantity) {

		order_status = OrderStatus::Filled;
		auto transaction_quantity = peak_quantity;

		// Execute trade for both orders
		this->execute_transaction(transaction_quantity);
		incoming_order->execute_transaction(transaction_quantity);

		return true;
	}
	// Partial trade
	else {

		order_status = OrderStatus::PartiallyFilled;
		auto transaction_quantity = incoming_order->peak_quantity;

		// Execute trade for both orders
		this->execute_transaction(transaction_quantity);
		incoming_order->execute_transaction(transaction_quantity);

		return false;
	}
}

// Execute the matched order, and update quantities for the order
void Order::execute_transaction(float const& intput_transaction_quantity) {
	
	quantity_transacted += intput_transaction_quantity;
	peak_quantity -= intput_transaction_quantity;

	if (peak_quantity == 0 and peak_quantity_left > 0) {
		if (peak_quantity_left >= this->original_quantity) {
			peak_quantity += this->original_quantity;
		}
		else {
			peak_quantity += peak_quantity_left;
		}
		peak_quantity_left -= peak_quantity;
	}
}

void Order::restore_peak_quantity() {

	if (peak_quantity_left > 0 && peak_quantity < this->original_quantity) {
		auto diff = min(this->original_quantity - peak_quantity, peak_quantity_left);
		peak_quantity += diff;
		peak_quantity_left -= diff;
	}
}


void Order::print_order() {
		
		cout << "id: " << this->id << endl;
		cout << "type: " << this->type << endl;
		cout << "price: " << this->price << endl;
		//cout << "order submission time: " << order_time;
		cout << "order status: " << order_status << endl;

		cout << "original quantity: " << this->original_quantity << endl;
		cout << "peak quantity: " << peak_quantity << endl;
		cout << "peak quantity left: " << peak_quantity_left << endl;
		cout << "transacted quantity: " << quantity_transacted << endl;
}


std::string Order::raw_order_info() const {
	return "Order: type: " + type +
		" id: " + std::to_string(id) +
		" price: " + std::to_string(price) +
		" qty: " + std::to_string(original_quantity);
}