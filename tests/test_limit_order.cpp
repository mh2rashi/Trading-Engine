#define CONFIG_CATCH_MAIN
#include "..\include\third_party_library\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <unordered_set>
using std::unordered_set;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\limit_order.h"

TEST_CASE("Test head properties of LinkedList class", "[Head properties]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	shared_ptr<Order> order_1 = make_shared<Order>("ETH", 1, OrderType::BUY, 100.0, 10.0);
	shared_ptr<Order> order_2 = make_shared<Order>("ETH", 2, OrderType::BUY, 100.0, 9.0);

	/* Set head */
	limitOrder1.setHead(order_1);
	REQUIRE(limitOrder1.getHead() == order_1);

	/* Set different head */
	limitOrder1.setHead(order_2);
	REQUIRE(limitOrder1.getHead() == order_2);

}

TEST_CASE("Test tail properties of LinkedList class", "[Tail properties]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	shared_ptr<Order> order_1 = make_shared<Order>("ETH", 1, OrderType::BUY, 100.0, 10.0);
	shared_ptr<Order> order_2 = make_shared<Order>("ETH", 2, OrderType::BUY, 100.0, 9.0);

	/* Set head */
	limitOrder1.setTail(order_1);
	REQUIRE(limitOrder1.getTail() == order_1);

	/* Set different head */
	limitOrder1.setTail(order_2);
	REQUIRE(limitOrder1.getTail() == order_2);

}

TEST_CASE("Test LinkedList length properties of LinkedList class", "[LinkedList length properties]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 10.0);
	Order order_2("ETH", 2, OrderType::BUY, 100.0, 9.0);

	/* Add Order's */
	limitOrder1.addOrder(order_1);
	limitOrder1.addOrder(order_2);

	/* Test list length */
	REQUIRE(limitOrder1.getListLength() == 2);

	/* Update list length */
	limitOrder1.updateListLength(-1);
	REQUIRE(limitOrder1.getListLength() == 1);
	limitOrder1.updateListLength(1);
	REQUIRE(limitOrder1.getListLength() == 2);

}

TEST_CASE("Test delete order properties of LinkedList class", "[delete_order]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	const auto& order_1 = std::make_shared<Order>("ETH", 1, OrderType::BUY, 100.0, 10.0);
	const auto& order_2 = std::make_shared<Order>("ETH", 2, OrderType::BUY, 100.0, 9.0);
	const auto& order_3 = std::make_shared<Order>("ETH", 3, OrderType::BUY, 100.0, 8.0);
	const auto& order_4 = std::make_shared<Order>("ETH", 4, OrderType::BUY, 100.0, 7.0);


	/* Add Order's */
	limitOrder1.addOrder(*order_1);
	limitOrder1.addOrder(*order_2);
	limitOrder1.addOrder(*order_3);
	limitOrder1.addOrder(*order_4);

	/* Delete head order */
	limitOrder1.deleteOrder(order_1);
	REQUIRE(limitOrder1.getHead() == order_2); /* order_2 will become the new head */
	REQUIRE(limitOrder1.getListLength() == 3);

	/* Delete middle order */
	limitOrder1.deleteOrder(order_3);
	REQUIRE(limitOrder1.getHead() == order_2);
	REQUIRE(limitOrder1.getTail() == order_4);
	REQUIRE(limitOrder1.getListLength() == 2);

	/* Delete tail order */
	limitOrder1.deleteOrder(order_4);
	REQUIRE(limitOrder1.getHead() == order_2);
	REQUIRE(limitOrder1.getTail() == order_2); /* order 2 will become the new tail */
	REQUIRE(limitOrder1.getListLength() == 1);

}

TEST_CASE("Test where incoming_order matches all orders within the LimitOrder", "[match_order]") {

	/* Create Order's */
	Order order_1(1, OrderType::SELL, 100.0, 1.0);
	Order order_2(2, OrderType::SELL, 100.0, 2.0);
	Order order_3(3, OrderType::SELL, 100.0, 3.0);

	/* Add Order's to order set */
    unordered_set<unsigned int> order_set1;
	order_set1.insert(order_1.getID());
	order_set1.insert(order_2.getID());
	order_set1.insert(order_3.getID());

	/* Create LimitOrder and add Order's */
	LimitOrder limitOrder1;
	limitOrder1.addOrder(order_1);
	limitOrder1.addOrder(order_2);
	limitOrder1.addOrder(order_3);

	/* Create a Transaction to record matched Order's */
	Transactions transaction1;

	/* Create incoming_order and start matching */
	Order order_4(4, OrderType::BUY, 100.0, 6.0);
	limitOrder1.matchOrder(order_4, order_set1, transaction1);

	/* All Order's are filled */
	REQUIRE(order_1.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_2.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_3.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_4.getStatus() == OrderStatus::FILLED);

	/* The Order dictionary is empty now */
	REQUIRE(order_set1.size() == 0);

}

TEST_CASE("Test where incoming_order matches all orders within the LimitOrder but itself is Partially Filled ", "[match_order]") {

	/* Create Order's */
	Order order_5(5, OrderType::SELL, 100.0, 1.0);
	Order order_6(6, OrderType::SELL, 100.0, 2.0);
	Order order_7(7, OrderType::SELL, 100.0, 3.0);

	/* Add Order's to Order dictionary */
	unordered_set<unsigned int> order_set2;
	order_set2.insert(order_5.getID());
	order_set2.insert(order_6.getID());
	order_set2.insert(order_7.getID());

	/* Create LimitOrder and add Order's */
	LimitOrder limitOrder2;
	limitOrder2.addOrder(order_5);
	limitOrder2.addOrder(order_6);
	limitOrder2.addOrder(order_7);

	/* Create a Transaction to record matched Order's */
	Transactions transaction2;

	/* Create incoming_order and start matching */
	Order order_8(8, OrderType::BUY, 100.0, 7.0);
	limitOrder2.matchOrder(order_8, order_set2, transaction2);

	/* All Order's are filled */
	REQUIRE(order_5.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_6.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_7.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_8.getStatus() == OrderStatus::PARTIALLY_FILLED);

	/* The Order dictionary is empty now */
	REQUIRE(order_set2.size() == 0);

	/* Incoming order still has 1 unit unfullfilled */
	REQUIRE(order_8.getOriginalQuantity() != order_8.getTotalQuantityTransacted());
	REQUIRE(order_8.getPeakQuantity() == 1);

}

TEST_CASE("Test where incoming order is Filled but last order in the LimitOrder is Partially Filled ", "[match_order]") {

	/* Create Order's */
	Order order_9(9, OrderType::SELL, 100.0, 1.0);
	Order order_10(10, OrderType::SELL, 100.0, 2.0);
	Order order_11(11, OrderType::SELL, 100.0, 3.0);

	/* Add Order's to Order dictionary */
	std::unordered_set<unsigned int> order_set3;
	order_set3.insert(order_9.getID());
	order_set3.insert(order_10.getID());
	order_set3.insert(order_11.getID());

	/* Create LimitOrder and add Order's */
	LimitOrder limitOrder3;
	limitOrder3.addOrder(order_9);
	limitOrder3.addOrder(order_10);
	limitOrder3.addOrder(order_11);

	/* Create a Transaction to record matched Order's */
	Transactions transaction3;

	/* Create incoming_order and start matching */
	Order order_12(12, OrderType::BUY, 100.0, 5.0);
	limitOrder3.matchOrder(order_12, order_set3, transaction3);

	/* All Order's are filled */
	REQUIRE(order_9.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_10.getStatus() == OrderStatus::FILLED);
	REQUIRE(order_11.getStatus() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_12.getStatus() == OrderStatus::FILLED);

	/* The Order dictionary is empty now */
	REQUIRE(order_set3.size() == 1);

	/* Incoming order still has 1 unit unfullfilled */
	REQUIRE(order_11.getOriginalQuantity() != order_11.getTotalQuantityTransacted());
	REQUIRE(order_11.getPeakQuantity() == 1);

}