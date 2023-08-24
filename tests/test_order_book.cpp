#define CONFIG_CATCH_MAIN
#include "..\include\third_party_library\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_book.h"

TEST_CASE("Create simple OrderBook and test for getter functions ", "[Getter functions]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 200.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);

	REQUIRE(orderBook1.getBuyTree().getLength() == 1);
	REQUIRE(orderBook1.getBuyTree().getMinPrice() == 100.0);
	REQUIRE(orderBook1.getBuyTree().getMaxPrice() == 100.0);

	REQUIRE(orderBook1.getSellTree().getLength() == 1);
	REQUIRE(orderBook1.getSellTree().getMinPrice() == 200.0);
	REQUIRE(orderBook1.getSellTree().getMaxPrice() == 200.0);

}


TEST_CASE("Test to check the execute_order functionality of OrderBook where all incoming_order's are just stored ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::BUY, 50.0, 1.0);
	Order order_3("ETH", 3, OrderType::BUY, 25.0, 1.0);

	Order order_4("ETH", 4, OrderType::SELL, 200.0, 1.0);
	Order order_5("ETH", 5, OrderType::SELL, 225.0, 1.0);
	Order order_6("ETH", 6, OrderType::SELL, 250.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);
	orderBook1.processOrder(order_3);
	orderBook1.processOrder(order_4);
	orderBook1.processOrder(order_5);
	orderBook1.processOrder(order_6);

	REQUIRE(orderBook1.getBuyTree().getLength() == 3);
	REQUIRE(orderBook1.getBuyTree().getMinPrice() == 25.0);
	REQUIRE(orderBook1.getBuyTree().getMaxPrice() == 100.0);

	REQUIRE(orderBook1.getSellTree().getLength() == 3);
	REQUIRE(orderBook1.getSellTree().getMinPrice() == 200.0);
	REQUIRE(orderBook1.getSellTree().getMaxPrice() == 250.0);

}

TEST_CASE("Test to check the execute_order functionality of OrderBook where a single buy order is matched with a single sell order ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 100.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);

	REQUIRE(orderBook1.getBuyTree().getLength() == 0);
	REQUIRE(orderBook1.getSellTree().getLength() == 0);

}

TEST_CASE("Test to check the execute_order functionality where a single sell order is matched with a single buy order having multiple buy order's ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;
	ostringstream;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::BUY, 99.0, 1.0);
	Order order_3("ETH", 3, OrderType::SELL, 98.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);
	orderBook1.processOrder(order_3);

	/* Highest buy order (100.0) will match with lowest sell order (98.0) */
	REQUIRE(orderBook1.getBuyTree().getLength() == 1);
	REQUIRE(orderBook1.getSellTree().getLength() == 0);
	REQUIRE(order_2.getStatus() == OrderStatus::STORED);

}

TEST_CASE("Test to check the execute_order functionality where a single buy order is matched with a single sell order having multiple sell order's ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::SELL, 101.0, 1.0);
	Order order_2("ETH", 2, OrderType::SELL, 99.0, 1.0);
	Order order_3("ETH", 3, OrderType::BUY, 100.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);
	orderBook1.processOrder(order_3);

	/* Highest buy order (100.0) will match with lowest sell order (99.0) */
	REQUIRE(orderBook1.getBuyTree().getLength() == 0);
	REQUIRE(orderBook1.getSellTree().getLength() == 1);
	REQUIRE(order_1.getStatus() == OrderStatus::STORED);

}

TEST_CASE("Test to check the execute_order functionality where buy order is matched with sell order when multiple order exists on both sides ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 1.0);
	Order order_2("ETH", 2, OrderType::BUY, 99.0, 1.0);
	Order order_3("ETH", 3, OrderType::SELL, 101.0, 1.0);
	Order order_4("ETH", 4, OrderType::SELL, 98.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);
	orderBook1.processOrder(order_3);
	orderBook1.processOrder(order_4);

	/* Highest buy order (100.0) will match with lowest sell order (98.0) */
	REQUIRE(orderBook1.getBuyTree().getLength() == 1);
	REQUIRE(orderBook1.getSellTree().getLength() == 1);

	REQUIRE(order_2.getStatus() == OrderStatus::STORED);
	REQUIRE(order_3.getStatus() == OrderStatus::STORED);

}

TEST_CASE("Test to check the execute_order functionality where incoming_order is partially filled ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 5.0);
	Order order_2("ETH", 2, OrderType::SELL, 100.0, 2.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);

	/* Highest buy order (100.0) will match with lowest sell order (100.0) */
	REQUIRE(orderBook1.getBuyTree().getLength() == 1);
	REQUIRE(orderBook1.getSellTree().getLength() == 0);
	REQUIRE(order_1.getStatus() == OrderStatus::STORED);

}

TEST_CASE("Test to check the execute_order functionality where incoming_order is fully matched and OrderBook is empty ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 100.0, 5.0);
	Order order_2("ETH", 2, OrderType::SELL, 100.0, 5.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);

	/* Highest buy order (100.0) will match with lowest sell order (100.0) */
	REQUIRE(orderBook1.getBuyTree().getLength() == 0);
	REQUIRE(orderBook1.getSellTree().getLength() == 0);

}

TEST_CASE("Test to check the cancel_order functionality of OrderBook class ", "[cancel_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	Order order_1("ETH", 1, OrderType::BUY, 98.0, 1.0);
	Order order_2("ETH", 2, OrderType::BUY, 99.0, 1.0);
	Order order_3("ETH", 3, OrderType::SELL, 101.0, 1.0);
	Order order_4("ETH", 4, OrderType::SELL, 100.0, 1.0);

	/* Add Order's */
	orderBook1.processOrder(order_1);
	orderBook1.processOrder(order_2);
	orderBook1.processOrder(order_3);
	orderBook1.processOrder(order_4);
	orderBook1.cancelOrder(make_shared<Order>(order_1));

	/* Highest buy order (100.0) will match with lowest sell order (100.0) */
	REQUIRE(orderBook1.getBuyTree().getLength() == 1);
	REQUIRE(orderBook1.getSellTree().getLength() == 2);

}