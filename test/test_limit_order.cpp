#define CONFIG_CATCH_MAIN
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\libs\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\limit_order.h"

TEST_CASE("Test head properties of LinkedList class", "[Head properties]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 10.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 100.0, 9.0);

	/* Set head */
	limitOrder1.set_head(order_1);
	REQUIRE(limitOrder1.get_head() == order_1);

	/* Set different head */
	limitOrder1.set_head(order_2);
	REQUIRE(limitOrder1.get_head() == order_2);

}

TEST_CASE("Test tail properties of LinkedList class", "[Tail properties]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 10.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 100.0, 9.0);

	/* Set head */
	limitOrder1.set_tail(order_1);
	REQUIRE(limitOrder1.get_tail() == order_1);

	/* Set different head */
	limitOrder1.set_tail(order_2);
	REQUIRE(limitOrder1.get_tail() == order_2);

}

TEST_CASE("Test LinkedList length properties of LinkedList class", "[LinkedList length properties]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 10.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 100.0, 9.0);

	/* Add Order's */
	limitOrder1.add_order(order_1);
	limitOrder1.add_order(order_2);

	/* Test list length */
	REQUIRE(limitOrder1.get_list_length() == 2);

	/* Update list length */
	limitOrder1.update_list_length(-1);
	REQUIRE(limitOrder1.get_list_length() == 1);
	limitOrder1.update_list_length(1);
	REQUIRE(limitOrder1.get_list_length() == 2);

}

TEST_CASE("Test delete order properties of LinkedList class", "[delete_order]") {

	/* Create LimitOrder */
	LimitOrder limitOrder1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 10.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 100.0, 9.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 100.0, 8.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 100.0, 7.0);

	/* Add Order's */
	limitOrder1.add_order(order_1);
	limitOrder1.add_order(order_2);
	limitOrder1.add_order(order_3);
	limitOrder1.add_order(order_4);

	/* Delete head order */
	limitOrder1.delete_order(order_1);
	REQUIRE(limitOrder1.get_head() == order_2); /* order_2 will become the new head */
	REQUIRE(limitOrder1.get_list_length() == 3);

	/* Delete middle order */
	limitOrder1.delete_order(order_3);
	REQUIRE(limitOrder1.get_head() == order_2);
	REQUIRE(limitOrder1.get_tail() == order_4);
	REQUIRE(limitOrder1.get_list_length() == 2);

	/* Delete tail order */
	limitOrder1.delete_order(order_4);
	REQUIRE(limitOrder1.get_head() == order_2);
	REQUIRE(limitOrder1.get_tail() == order_2); /* order 2 will become the new tail */
	REQUIRE(limitOrder1.get_list_length() == 1);

}

TEST_CASE("Test where incoming_order matches all orders within the LimitOrder", "[match_order]") {

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 100.0, 2.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 100.0, 3.0);

	/* Add Order's to Order dictionary */
	std::unordered_map<unsigned int, shared_ptr<Order>> order_map1;
	order_map1[1] = order_1;
	order_map1[2] = order_2;
	order_map1[3] = order_3;

	/* Create LimitOrder and add Order's */
	LimitOrder limitOrder1;
	limitOrder1.add_order(order_1);
	limitOrder1.add_order(order_2);
	limitOrder1.add_order(order_3);

	/* Create a Transaction to record matched Order's */
	Transactions transaction1;

	/* Create incoming_order and start matching */
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 100.0, 6.0);
	limitOrder1.match_order(order_4, order_map1, transaction1);

	/* All Order's are filled */
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);
	REQUIRE(order_3->get_status() == OrderStatus::FILLED);
	REQUIRE(order_4->get_status() == OrderStatus::FILLED);

	/* The Order dictionary is empty now */
	REQUIRE(order_map1.size() == 0);

}

TEST_CASE("Test where incoming_order matches all orders within the LimitOrder but itself is Partially Filled ", "[match_order]") {

	/* Create Order's */
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::SELL, 100.0, 1.0);
	std::shared_ptr<Order> order_6 = std::make_shared<Order>(6, OrderType::SELL, 100.0, 2.0);
	std::shared_ptr<Order> order_7 = std::make_shared<Order>(7, OrderType::SELL, 100.0, 3.0);

	/* Add Order's to Order dictionary */
	std::unordered_map<unsigned int, shared_ptr<Order>> order_map2;
	order_map2[5] = order_5;
	order_map2[6] = order_6;
	order_map2[7] = order_7;

	/* Create LimitOrder and add Order's */
	LimitOrder limitOrder2;
	limitOrder2.add_order(order_5);
	limitOrder2.add_order(order_6);
	limitOrder2.add_order(order_7);

	/* Create a Transaction to record matched Order's */
	Transactions transaction2;

	/* Create incoming_order and start matching */
	std::shared_ptr<Order> order_8 = std::make_shared<Order>(8, OrderType::BUY, 100.0, 7.0);
	limitOrder2.match_order(order_8, order_map2, transaction2);

	/* All Order's are filled */
	REQUIRE(order_5->get_status() == OrderStatus::FILLED);
	REQUIRE(order_6->get_status() == OrderStatus::FILLED);
	REQUIRE(order_7->get_status() == OrderStatus::FILLED);
	REQUIRE(order_8->get_status() == OrderStatus::PARTIALLY_FILLED);

	/* The Order dictionary is empty now */
	REQUIRE(order_map2.size() == 0);

	/* Incoming order still has 1 unit unfullfilled */
	REQUIRE(order_8->get_original_quantity() != order_8->get_total_quantity_transacted());
	REQUIRE(order_8->get_peak_quantity() == 1);

}

TEST_CASE("Test where incoming order is Filled but last order in the LimitOrder is Partially Filled ", "[match_order]") {

	/* Create Order's */
	std::shared_ptr<Order> order_9 = std::make_shared<Order>(9, OrderType::SELL, 100.0, 1.0);
	std::shared_ptr<Order> order_10 = std::make_shared<Order>(10, OrderType::SELL, 100.0, 2.0);
	std::shared_ptr<Order> order_11 = std::make_shared<Order>(11, OrderType::SELL, 100.0, 3.0);

	/* Add Order's to Order dictionary */
	std::unordered_map<unsigned int, shared_ptr<Order>> order_map3;
	order_map3[9] = order_9;
	order_map3[10] = order_10;
	order_map3[11] = order_11;

	/* Create LimitOrder and add Order's */
	LimitOrder limitOrder3;
	limitOrder3.add_order(order_9);
	limitOrder3.add_order(order_10);
	limitOrder3.add_order(order_11);

	/* Create a Transaction to record matched Order's */
	Transactions transaction3;

	/* Create incoming_order and start matching */
	std::shared_ptr<Order> order_12 = std::make_shared<Order>(12, OrderType::BUY, 100.0, 5.0);
	limitOrder3.match_order(order_12, order_map3, transaction3);

	/* All Order's are filled */
	REQUIRE(order_9->get_status() == OrderStatus::FILLED);
	REQUIRE(order_10->get_status() == OrderStatus::FILLED);
	REQUIRE(order_11->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_12->get_status() == OrderStatus::FILLED);

	/* The Order dictionary is empty now */
	REQUIRE(order_map3.size() == 1);

	/* Incoming order still has 1 unit unfullfilled */
	REQUIRE(order_11->get_original_quantity() != order_11->get_total_quantity_transacted());
	REQUIRE(order_11->get_peak_quantity() == 1);

}



