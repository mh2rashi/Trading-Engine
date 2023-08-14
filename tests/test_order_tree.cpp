#define CONFIG_CATCH_MAIN
#include "..\include\third_party_library\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <sstream>
using std::ostringstream;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_tree.h"

TEST_CASE("Create simple Order Tree and test for its length", "[get_length]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::BUY, 200.0, 1.0);
	Order order_3("ETH", 3, OrderType::BUY, 200.0, 2.0);
	Order order_4("ETH", 4, OrderType::BUY, 300.0, 2.0);
	Order order_5("ETH", 5, OrderType::BUY, 400.0, 1.0);


	/* Add Order's */
	orderTree1.addPriceOrder(order_1);
	orderTree1.addPriceOrder(order_2);
	orderTree1.addPriceOrder(order_3);
	orderTree1.addPriceOrder(order_4);
	orderTree1.addPriceOrder(order_5);
	REQUIRE(orderTree1.getLength() == 5);

}

TEST_CASE("Create simple Order Tree and test for min_price functions", "[min_price]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::BUY, 200.0, 1.0);
	Order order_3("ETH", 3, OrderType::BUY, 200.0, 2.0);
	Order order_4("ETH", 4, OrderType::BUY, 300.0, 2.0);
	Order order_5("ETH", 5, OrderType::BUY, 400.0, 1.0);


	/* Add Order's */
	orderTree1.addPriceOrder(order_1);
	orderTree1.addPriceOrder(order_2);
	orderTree1.addPriceOrder(order_3);
	orderTree1.addPriceOrder(order_4);
	orderTree1.addPriceOrder(order_5);
	REQUIRE(orderTree1.getMinPrice() == 100.0);

	/*Set new min_price */
	orderTree1.setMinPrice(50.0);
	REQUIRE(orderTree1.getMinPrice() == 50.0);

}

TEST_CASE("Create simple Order Tree and test for max_price functions", "[max_price]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Create Order's */
	Order order_1("BTC", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("BTC", 2, OrderType::BUY, 200.0, 1.0);
	Order order_3("BTC", 3, OrderType::BUY, 200.0, 2.0);
	Order order_4("BTC", 4, OrderType::BUY, 300.0, 2.0);
	Order order_5("BTC", 5, OrderType::BUY, 400.0, 1.0);

	/* Add Order's */
	orderTree1.addPriceOrder(order_1);
	orderTree1.addPriceOrder(order_2);
	orderTree1.addPriceOrder(order_3);
	orderTree1.addPriceOrder(order_4);
	orderTree1.addPriceOrder(order_5);
	REQUIRE(orderTree1.getMaxPrice() == 400.0);

	/*Set new min_price */
	orderTree1.setMaxPrice(500.0);
	REQUIRE(orderTree1.getMaxPrice() == 500.0);

}

TEST_CASE("Test for add_price_order functionality of an OrderTree", "[add_price_order]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);

	/* Test first Order */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	orderTree1.addPriceOrder(order_1);

	REQUIRE(orderTree1.getMinPrice() == 100.0);
	REQUIRE(orderTree1.getMaxPrice() == 100.0);

	/* Create more Order's */
	Order order_2("ETH", 2, OrderType::BUY, 200.0, 1.0);
	Order order_3("ETH", 3, OrderType::BUY, 200.0, 2.0);
	Order order_4("ETH", 4, OrderType::BUY, 300.0, 2.0);
	Order order_5("ETH", 5, OrderType::BUY, 400.0, 1.0);

	/* Add more Order's and test */
	orderTree1.addPriceOrder(order_2);
	orderTree1.addPriceOrder(order_3);
	orderTree1.addPriceOrder(order_4);
	orderTree1.addPriceOrder(order_5);

	REQUIRE(orderTree1.getMinPrice() == 100.0);
	REQUIRE(orderTree1.getMaxPrice() == 400.0);
	REQUIRE(orderTree1.getLength() == 5.0);

}

TEST_CASE("Test for delete_price_order functionality of an OrderTree", "[delete_price_order]") {

	/* Create OrderTree */
	OrderTree orderTree1(OrderType::BUY);


	/* Test to delete an order that does not exist */
	Order order_1("BTC", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("BTC", 2, OrderType::BUY, 200.0, 1.0);
	orderTree1.addPriceOrder(order_1);
	orderTree1.deletePriceOrder(make_shared<Order>(order_2));

	REQUIRE(orderTree1.getLength() == 1);

	/* Create more Order's */
	Order order_3("BTC", 3, OrderType::BUY, 200.0, 2.0);
	Order order_4("BTC", 4, OrderType::BUY, 300.0, 2.0);
	Order order_5("BTC", 5, OrderType::BUY, 400.0, 1.0);

	/* Add more Order's */
	orderTree1.addPriceOrder(order_2);
	orderTree1.addPriceOrder(order_3);
	orderTree1.addPriceOrder(order_4);
	orderTree1.addPriceOrder(order_5);

	/* Remove Order which doesn't change the min or max price */
	REQUIRE(orderTree1.getLength() == 5);
	REQUIRE(orderTree1.getMinPrice() == 100.0);
	REQUIRE(orderTree1.getMaxPrice() == 400.0);

	orderTree1.deletePriceOrder(make_shared<Order>(order_3)); /* Remove order with price 200.0 */

	REQUIRE(orderTree1.getLength() == 4);
	REQUIRE(orderTree1.getMinPrice() == 100.0);
	REQUIRE(orderTree1.getMaxPrice() == 400.0);

	/* Remove Order which changes the min price */
	orderTree1.deletePriceOrder(make_shared<Order>(order_1)); /* Remove order with price 100.0 */
	REQUIRE(orderTree1.getLength() == 3);
	REQUIRE(orderTree1.getMinPrice() == 200.0);
	REQUIRE(orderTree1.getMaxPrice() == 400.0);

	/* Remove Order which changes the max price */
	orderTree1.deletePriceOrder(make_shared<Order>(order_5)); /* Remove order with price 100.0 */
	REQUIRE(orderTree1.getLength() == 2);
	REQUIRE(orderTree1.getMinPrice() == 200.0);
	REQUIRE(orderTree1.getMaxPrice() == 300.0);

}

TEST_CASE("Tests for when the incoming_order is not matched at all ", "[match_price_order][no match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;


	/* Test to match incoming_order when no order exists within the tree */
	Order incoming_order_1("ETH", 1, OrderType::BUY, 200.0, 1.0);
	
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::NEW_ORDER); /* No transactions */
	REQUIRE(sell_orderTree.getLength() == 0); /* No Order's exist within the tree */

	/* Test to match incoming_order of the same type as OrderTree */
	Order incoming_order_2("ETH", 2, OrderType::SELL, 200.0, 1.0);
	Order order_3("ETH", 3, OrderType::SELL, 200.0, 1.0);


	sell_orderTree.addPriceOrder(order_3);

	sell_orderTree.matchPriceOrder(incoming_order_2, oss);
	REQUIRE(incoming_order_2.getStatus() == OrderStatus::NEW_ORDER); /* No transactions */
	REQUIRE(sell_orderTree.getLength() == 1); /* 1 Order exists within the tree */

	/* Test to match incoming_order when its peak_quantity is 0 */
	Order incoming_order_3("ETH", 3, OrderType::SELL, 200.0, 1.0);
	incoming_order_3.setPeakQuantity(0);

	sell_orderTree.matchPriceOrder(incoming_order_3, oss);
	REQUIRE(incoming_order_3.getStatus() == OrderStatus::NEW_ORDER); /* No transactions */
	REQUIRE(sell_orderTree.getLength() == 1); /* 1 Order exists within the tree */

	/* Test to match an incoming_order with no appropriate price_to_match */
	Order incoming_order_4("ETH", 4, OrderType::BUY, 100.0, 1.0);


	sell_orderTree.matchPriceOrder(incoming_order_4, oss);
	REQUIRE(incoming_order_4.getStatus() == OrderStatus::NEW_ORDER); /* No transactions */
	REQUIRE(sell_orderTree.getLength() == 1); /* 1 Order exists within the tree */

}

TEST_CASE("Tests for when the incoming_order is fully matched using 1 complete Tree node ", "[match_price_order][full match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::SELL, 99.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 99.0, 2.0);
	Order order_3("ETH", 3, OrderType::SELL, 99.0, 2.0);
	Order order_4("ETH", 4, OrderType::SELL, 100.0, 1.0);
	Order order_5("ETH", 5, OrderType::SELL, 100.0, 2.0);
	Order order_6("ETH", 6, OrderType::SELL, 100.0, 2.0);


	/* Add Order's */
	sell_orderTree.addPriceOrder(order_1);
	sell_orderTree.addPriceOrder(order_2);
	sell_orderTree.addPriceOrder(order_3);
	sell_orderTree.addPriceOrder(order_4);
	sell_orderTree.addPriceOrder(order_5);
	sell_orderTree.addPriceOrder(order_6);

	/* Match incoming_order */
	Order incoming_order_1("ETH", 7, OrderType::BUY, 100.0, 5.0);
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);

	REQUIRE(sell_orderTree.getLength() == 3);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::FULFILLED);

}

TEST_CASE("Tests for when the incoming_order is fully matched using 1 complete and 1 partial tree node ", "[match_price_order][full match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;

	/* Create Order's */

	/* 1st node */
	Order order_1("ETH", 1, OrderType::SELL, 99.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 99.0, 2.0);
	Order order_3("ETH", 3, OrderType::SELL, 99.0, 2.0);

	/* 2nd node */
	Order order_4("ETH", 4, OrderType::SELL, 100.0, 1.0);
	Order order_5("ETH", 5, OrderType::SELL, 100.0, 2.0);
	Order order_6("ETH", 6, OrderType::SELL, 100.0, 2.0);


	/* Add Order's */
	sell_orderTree.addPriceOrder(order_1);
	sell_orderTree.addPriceOrder(order_2);
	sell_orderTree.addPriceOrder(order_3);
	sell_orderTree.addPriceOrder(order_4);
	sell_orderTree.addPriceOrder(order_5);
	sell_orderTree.addPriceOrder(order_6);

	/* Match incoming_order */
	Order incoming_order_1("ETH", 7, OrderType::BUY, 100.0, 7.0);
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);

	REQUIRE(sell_orderTree.getLength() == 2);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::FULFILLED);
	REQUIRE(order_6.getStatus() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is fully matched using a partial tree node ", "[match_price_order][full match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;

	/* Create Order's */

	/* 1st node */
	Order order_1("ETH", 1, OrderType::SELL, 99.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 99.0, 2.0);
	Order order_3("ETH", 3, OrderType::SELL, 99.0, 2.0);

	/* 2nd node */
	Order order_4("ETH", 4, OrderType::SELL, 100.0, 1.0);
	Order order_5("ETH", 5, OrderType::SELL, 100.0, 2.0);
	Order order_6("ETH", 6, OrderType::SELL, 100.0, 2.0);


	/* Add Order's */
	sell_orderTree.addPriceOrder(order_1);
	sell_orderTree.addPriceOrder(order_2);
	sell_orderTree.addPriceOrder(order_3);
	sell_orderTree.addPriceOrder(order_4);
	sell_orderTree.addPriceOrder(order_5);
	sell_orderTree.addPriceOrder(order_6);

	/* Match incoming_order */
	Order incoming_order_1("ETH", 7, OrderType::BUY, 100.0, 2.0);
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);

	REQUIRE(sell_orderTree.getLength() == 5);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::FULFILLED);
	REQUIRE(order_3.getStatus() == OrderStatus::NEW_ORDER);
	REQUIRE(order_4.getStatus() == OrderStatus::NEW_ORDER);
	REQUIRE(order_5.getStatus() == OrderStatus::NEW_ORDER);
	REQUIRE(order_6.getStatus() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is partially matched due to no more Order's to match ", "[match_price_order][partial match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::SELL, 99.0, 1.0);
	
	/* Add Order's */
	sell_orderTree.addPriceOrder(order_1);

	/* Match incoming_order */
	Order incoming_order_1("ETH", 7, OrderType::BUY, 100.0, 2.0);
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);

	REQUIRE(sell_orderTree.getLength() == 0);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::PARTIALLY_FILLED);

}

TEST_CASE("Tests for when the incoming_order is partially matched due to existing Order's have price discrepany ", "[match_price_order][partial match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::SELL, 99.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 101.0, 1.0);

	/* Add Order's */
	sell_orderTree.addPriceOrder(order_1);
	sell_orderTree.addPriceOrder(order_2);

	/* Match incoming_order */
	Order incoming_order_1("ETH", 7, OrderType::BUY, 100.0, 2.0);
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);

	REQUIRE(sell_orderTree.getLength() == 1);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_2.getStatus() == OrderStatus::NEW_ORDER);

}

TEST_CASE("Tests for when the incoming_order is partially matched due to no more Order's left to match ", "[match_price_order][partial match]") {

	/* Create OrderTree */
	OrderTree sell_orderTree(OrderType::SELL);
	ostringstream oss;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::SELL, 99.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 99.0, 2.0);
	Order order_3("ETH", 3, OrderType::SELL, 100.0, 2.0);


	/* Add Order's */
	sell_orderTree.addPriceOrder(order_1);
	sell_orderTree.addPriceOrder(order_2);
	sell_orderTree.addPriceOrder(order_3);

	/* Match incoming_order */
	Order incoming_order_1("ETH", 7, OrderType::BUY, 100.0, 6.0);
	sell_orderTree.matchPriceOrder(incoming_order_1, oss);

	REQUIRE(sell_orderTree.getLength() == 0);
	REQUIRE(incoming_order_1.getStatus() == OrderStatus::PARTIALLY_FILLED);

}
