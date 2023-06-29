#pragma once
#include "Headers/Order.h"
#include "Headers/LimitOrder.h"

#include <iostream>
using std::cout;
using std::endl;

using std::shared_ptr;
using std::make_shared;


shared_ptr<Order> LimitOrder::getHead() {
	return head;
}


void LimitOrder::setHead(const Order& incoming_order) {
	head = make_shared<Order>(incoming_order);
}


shared_ptr<Order> LimitOrder::getTail() {
	return tail;
}


void LimitOrder::setTail(const Order& incoming_order) {
	tail = make_shared<Order>(incoming_order);
}

int LimitOrder::getLength() const {
	return list_length;
}

void LimitOrder::add_order(Order& incoming_order) {
	//First Order
	if (head == nullptr) {
		this->
	}
}