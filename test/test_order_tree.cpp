#define CONFIG_CATCH_MAIN
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\libs\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_tree.h"

TEST_CASE("Create simple Order Tree and test for its length", "[get_length]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 200.0, 1.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 200.0, 2.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 300.0, 2.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::BUY, 400.0, 1.0);

	/* Add Order's */
	orderTree1.add_price_order(order_1);
	orderTree1.add_price_order(order_2);
	orderTree1.add_price_order(order_3);
	orderTree1.add_price_order(order_4);
	orderTree1.add_price_order(order_5);
	REQUIRE(orderTree1.get_length() == 5);

}

TEST_CASE("Create simple Order Tree and test for min_price functions", "[min_price]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 200.0, 1.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 200.0, 2.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 300.0, 2.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::BUY, 400.0, 1.0);

	/* Add Order's */
	orderTree1.add_price_order(order_1);
	orderTree1.add_price_order(order_2);
	orderTree1.add_price_order(order_3);
	orderTree1.add_price_order(order_4);
	orderTree1.add_price_order(order_5);
	REQUIRE(orderTree1.get_min() == 100.0);

	/*Set new min_price */
	orderTree1.set_min(50.0);
	REQUIRE(orderTree1.get_min() == 50.0);

}

TEST_CASE("Create simple Order Tree and test for max_price functions", "[max_price]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 200.0, 1.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 200.0, 2.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 300.0, 2.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::BUY, 400.0, 1.0);

	/* Add Order's */
	orderTree1.add_price_order(order_1);
	orderTree1.add_price_order(order_2);
	orderTree1.add_price_order(order_3);
	orderTree1.add_price_order(order_4);
	orderTree1.add_price_order(order_5);
	REQUIRE(orderTree1.get_max() == 400.0);

	/*Set new min_price */
	orderTree1.set_max(500.0);
	REQUIRE(orderTree1.get_max() == 500.0);

}

TEST_CASE("Test for add_price_order functionality of an OrderTree", "[add_price_order]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Test first Order */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	orderTree1.add_price_order(order_1);

	REQUIRE(orderTree1.get_min() == 100.0);
	REQUIRE(orderTree1.get_max() == 100.0);

	/* Create more Order's */
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 200.0, 1.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 200.0, 2.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 300.0, 2.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::BUY, 400.0, 1.0);

	/* Add more Order's and test */
	orderTree1.add_price_order(order_2);
	orderTree1.add_price_order(order_3);
	orderTree1.add_price_order(order_4);
	orderTree1.add_price_order(order_5);

	REQUIRE(orderTree1.get_min() == 100.0);
	REQUIRE(orderTree1.get_max() == 400.0);
	REQUIRE(orderTree1.get_length() == 5.0);

}

TEST_CASE("Test for delete_price_order functionality of an OrderTree", "[delete_price_order]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);


	/* Test to delete an order that does not exist */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 200.0, 1.0);
	orderTree1.add_price_order(order_1);
	orderTree1.delete_price_order(order_2);

	REQUIRE(orderTree1.get_length() == 1);

	/* Create more Order's */
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 200.0, 2.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::BUY, 300.0, 2.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::BUY, 400.0, 1.0);

	/* Add more Order's */
	orderTree1.add_price_order(order_2);
	orderTree1.add_price_order(order_3);
	orderTree1.add_price_order(order_4);
	orderTree1.add_price_order(order_5);

	/* Remove Order which doesn't change the min or max price */
	REQUIRE(orderTree1.get_length() == 5);
	REQUIRE(orderTree1.get_min() == 100.0);
	REQUIRE(orderTree1.get_max() == 400.0);

	orderTree1.delete_price_order(order_3); /* Remove order with price 200.0 */

	REQUIRE(orderTree1.get_length() == 4);
	REQUIRE(orderTree1.get_min() == 100.0);
	REQUIRE(orderTree1.get_max() == 400.0);

	/* Remove Order which changes the min price */
	orderTree1.delete_price_order(order_1); /* Remove order with price 100.0 */
	REQUIRE(orderTree1.get_length() == 3);
	REQUIRE(orderTree1.get_min() == 200.0);
	REQUIRE(orderTree1.get_max() == 400.0);

	/* Remove Order which changes the max price */
	orderTree1.delete_price_order(order_5); /* Remove order with price 100.0 */
	REQUIRE(orderTree1.get_length() == 2);
	REQUIRE(orderTree1.get_min() == 200.0);
	REQUIRE(orderTree1.get_max() == 300.0);

}

TEST_CASE("Tests for when the incoming_order is not matched at all ", "[match_price_order][no match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);


	/* Test to match incoming_order when no order exists within the tree */
	
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(1, OrderType::BUY, 200.0, 1.0);
	
	Transactions test_match_1 = sell_orderTree.match_price_order(incoming_order_1);
	REQUIRE(test_match_1.get_length() == 0); /* No transactions */
	REQUIRE(sell_orderTree.get_length() == 0); /* No Order's exist within the tree */

	/* Test to match incoming_order of the same type as OrderTree */
	
	std::shared_ptr<Order> incoming_order_2 = std::make_shared<Order>(2, OrderType::SELL, 200.0, 1.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 200.0, 1.0);

	sell_orderTree.add_price_order(order_3);

	Transactions test_match_2 = sell_orderTree.match_price_order(incoming_order_2);
	REQUIRE(test_match_2.get_length() == 0); /* No transactions */
	REQUIRE(sell_orderTree.get_length() == 1); /* 1 Order exists within the tree */

	/* Test to match incoming_order when its peak_quantity is 0 */
	std::shared_ptr<Order> incoming_order_3 = std::make_shared<Order>(3, OrderType::SELL, 200.0, 1.0);
	incoming_order_3->set_peak_quantity(0);

	Transactions test_match_3 = sell_orderTree.match_price_order(incoming_order_3);
	REQUIRE(test_match_3.get_length() == 0); /* No transactions */
	REQUIRE(sell_orderTree.get_length() == 1); /* 1 Order exists within the tree */

	/* Test to match an incoming_order with no appropriate price_to_match */

	std::shared_ptr<Order> incoming_order_4 = std::make_shared<Order>(4, OrderType::BUY, 100.0, 1.0);

	Transactions test_match_4 = sell_orderTree.match_price_order(incoming_order_4);
	REQUIRE(test_match_4.get_length() == 0); /* No transactions */
	REQUIRE(sell_orderTree.get_length() == 1); /* 1 Order exists within the tree */

}

TEST_CASE("Tests for when the incoming_order is fully matched using 1 complete Tree node ", "[match_price_order][full match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 99.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 99.0, 2.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 99.0, 2.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::SELL, 100.0, 1.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::SELL, 100.0, 2.0);
	std::shared_ptr<Order> order_6 = std::make_shared<Order>(6, OrderType::SELL, 100.0, 2.0);

	/* Add Order's */
	sell_orderTree.add_price_order(order_1);
	sell_orderTree.add_price_order(order_2);
	sell_orderTree.add_price_order(order_3);
	sell_orderTree.add_price_order(order_4);
	sell_orderTree.add_price_order(order_5);
	sell_orderTree.add_price_order(order_6);

	/* Match incoming_order */
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(7, OrderType::BUY, 100.0, 5.0);
	sell_orderTree.match_price_order(incoming_order_1);

	REQUIRE(sell_orderTree.get_length() == 3);
	REQUIRE(incoming_order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);
	REQUIRE(order_3->get_status() == OrderStatus::FILLED);
	REQUIRE(order_4->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_5->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_6->get_status() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is fully matched using 1 complete and 1 partial tree node ", "[match_price_order][full match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);

	/* Create Order's */

	/* 1st node */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 99.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 99.0, 2.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 99.0, 2.0);
	
	/* 2nd node */
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::SELL, 100.0, 1.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::SELL, 100.0, 2.0);
	std::shared_ptr<Order> order_6 = std::make_shared<Order>(6, OrderType::SELL, 100.0, 2.0);

	/* Add Order's */
	sell_orderTree.add_price_order(order_1);
	sell_orderTree.add_price_order(order_2);
	sell_orderTree.add_price_order(order_3);
	sell_orderTree.add_price_order(order_4);
	sell_orderTree.add_price_order(order_5);
	sell_orderTree.add_price_order(order_6);

	/* Match incoming_order */
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(7, OrderType::BUY, 100.0, 7.0);
	auto trans = sell_orderTree.match_price_order(incoming_order_1);

	REQUIRE(sell_orderTree.get_length() == 2);
	REQUIRE(incoming_order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);
	REQUIRE(order_3->get_status() == OrderStatus::FILLED);
	REQUIRE(order_4->get_status() == OrderStatus::FILLED);
	REQUIRE(order_5->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_6->get_status() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is fully matched using a partial tree node ", "[match_price_order][full match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);

	/* Create Order's */

	/* 1st node */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 99.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 99.0, 2.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 99.0, 2.0);

	/* 2nd node */
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::SELL, 100.0, 1.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::SELL, 100.0, 2.0);
	std::shared_ptr<Order> order_6 = std::make_shared<Order>(6, OrderType::SELL, 100.0, 2.0);

	/* Add Order's */
	sell_orderTree.add_price_order(order_1);
	sell_orderTree.add_price_order(order_2);
	sell_orderTree.add_price_order(order_3);
	sell_orderTree.add_price_order(order_4);
	sell_orderTree.add_price_order(order_5);
	sell_orderTree.add_price_order(order_6);

	/* Match incoming_order */
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(7, OrderType::BUY, 100.0, 2.0);
	auto trans = sell_orderTree.match_price_order(incoming_order_1);

	REQUIRE(sell_orderTree.get_length() == 5);
	REQUIRE(incoming_order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_3->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_4->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_5->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_6->get_status() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is partially matched due to no more Order's to match ", "[match_price_order][partial match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 99.0, 1.0);
	
	/* Add Order's */
	sell_orderTree.add_price_order(order_1);

	/* Match incoming_order */
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(7, OrderType::BUY, 100.0, 2.0);
	auto trans = sell_orderTree.match_price_order(incoming_order_1);

	REQUIRE(sell_orderTree.get_length() == 0);
	REQUIRE(incoming_order_1->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);

}

TEST_CASE("Tests for when the incoming_order is partially matched due to existing Order's have price discrepany ", "[match_price_order][partial match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 99.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 101.0, 1.0);


	/* Add Order's */
	sell_orderTree.add_price_order(order_1);
	sell_orderTree.add_price_order(order_2);

	/* Match incoming_order */
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(7, OrderType::BUY, 100.0, 2.0);
	auto trans = sell_orderTree.match_price_order(incoming_order_1);

	REQUIRE(sell_orderTree.get_length() == 1);
	REQUIRE(incoming_order_1->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is partially matched due to no more Order's left to match ", "[match_price_order][partial match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 99.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 99.0, 2.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 100.0, 2.0);


	/* Add Order's */
	sell_orderTree.add_price_order(order_1);
	sell_orderTree.add_price_order(order_2);
	sell_orderTree.add_price_order(order_3);

	/* Match incoming_order */
	std::shared_ptr<Order> incoming_order_1 = std::make_shared<Order>(7, OrderType::BUY, 100.0, 6.0);
	auto trans = sell_orderTree.match_price_order(incoming_order_1);

	REQUIRE(sell_orderTree.get_length() == 0);
	REQUIRE(incoming_order_1->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);
	REQUIRE(order_3->get_status() == OrderStatus::FILLED);

}

