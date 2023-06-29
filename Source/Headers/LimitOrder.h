#pragma once
#include <string>
#include <memory>
#include "Order.h"


/*Composition class responsible for managing objects of Order class through a doubly
LinkedList. The LinkedList ensures that Orders are added and removed in accordance
with the FIFO (first-in, first-out) principle. New Orders are added to the tail of
the LinkedList, while Order mathcing is performed from the head, maintaining the
desired order.*/



class LimitOrder {
	// Non-changeable properties
private:
	std::shared_ptr<Order> head;
	std::shared_ptr<Order> tail;
	int list_length;

public:
	// Constructor
	LimitOrder() : head(nullptr), tail(nullptr), list_length(0) {};

	// Head properties
	std::shared_ptr<Order> getHead();
	void setHead(const Order& incoming_order);

	// Tail properties
	std::shared_ptr<Order> getTail();
	void setTail(const Order& incoming_order);

	int getLength() const;

	void add_order(Order& incoming_order);


};

def add_order(self, order) :
	# First element
	if self.head is None :
order.nxt = None
order.prev = None
self.head = order
self.tail = order
# Add to tail
	else:
order.prev = self.tail
order.nxt = None
self.tail.nxt = order
self.tail = order

self._length += 1