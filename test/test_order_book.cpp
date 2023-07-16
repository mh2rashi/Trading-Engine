#define CONFIG_CATCH_MAIN
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\libs\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_book.h"

TEST_CASE("Create simple OrderBook and test for getter functions ", "[Getter functions]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 200.0, 1.0);


	/* Add Order's */
	orderBook1.execute_order(order_1);
	orderBook1.execute_order(order_2);

	REQUIRE(orderBook1.get_buy_tree().get_length() == 1);
	REQUIRE(orderBook1.get_buy_tree().get_min() == 100.0);
	REQUIRE(orderBook1.get_buy_tree().get_max() == 100.0);

	REQUIRE(orderBook1.get_sell_tree().get_length() == 1);
	REQUIRE(orderBook1.get_sell_tree().get_min() == 200.0);
	REQUIRE(orderBook1.get_sell_tree().get_max() == 200.0);

}


TEST_CASE("Test to check the execute_order functionality of OrderBook where all incoming_order's are just stored ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 50.0, 1.0);
	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 25.0, 1.0);
	
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::SELL, 200.0, 1.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::SELL, 225.0, 1.0);
	std::shared_ptr<Order> order_6 = std::make_shared<Order>(6, OrderType::SELL, 250.0, 1.0);


	/* Add Order's */
	orderBook1.execute_order(order_1);
	orderBook1.execute_order(order_2);
	orderBook1.execute_order(order_3);
	orderBook1.execute_order(order_4);
	orderBook1.execute_order(order_5);
	orderBook1.execute_order(order_6);

	REQUIRE(orderBook1.get_buy_tree().get_length() == 3);
	REQUIRE(orderBook1.get_buy_tree().get_min() == 25.0);
	REQUIRE(orderBook1.get_buy_tree().get_max() == 100.0);

	REQUIRE(orderBook1.get_sell_tree().get_length() == 3);
	REQUIRE(orderBook1.get_sell_tree().get_min() == 200.0);
	REQUIRE(orderBook1.get_sell_tree().get_max() == 250.0);

}

TEST_CASE("Test to check the execute_order functionality of OrderBook where a single buy order is matched with a single sell order ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 100.0, 1.0);


	/* Add Order's */
	orderBook1.execute_order(order_1);
	auto transaction_1 = orderBook1.execute_order(order_2);

	REQUIRE(orderBook1.get_buy_tree().get_length() == 0);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 0);

	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);

	REQUIRE(transaction_1.get_length() == 2);

}

TEST_CASE("Test to check the execute_order functionality where a single sell order is matched with a single buy order having multiple buy order's ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */

	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 99.0, 1.0);

	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 98.0, 1.0);


	/* Add Order's */
	orderBook1.execute_order(order_1);
	orderBook1.execute_order(order_2);
	auto transaction_1 = orderBook1.execute_order(order_3);

	/* Highest buy order (100.0) will match with lowest sell order (98.0) */
	REQUIRE(orderBook1.get_buy_tree().get_length() == 1);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 0);

	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_3->get_status() == OrderStatus::FILLED);

	REQUIRE(transaction_1.get_length() == 2);

}

TEST_CASE("Test to check the execute_order functionality where a single buy order is matched with a single sell order having multiple sell order's ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::SELL, 101.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 99.0, 1.0);

	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::BUY, 100.0, 1.0);


	/* Add Order's */
	orderBook1.execute_order(order_1);
	orderBook1.execute_order(order_2);
	auto transaction_1 = orderBook1.execute_order(order_3);

	/* Highest buy order (100.0) will match with lowest sell order (99.0) */
	REQUIRE(orderBook1.get_buy_tree().get_length() == 0);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 1);

	REQUIRE(order_1->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);
	REQUIRE(order_3->get_status() == OrderStatus::FILLED);

	REQUIRE(transaction_1.get_length() == 2);

}

TEST_CASE("Test to check the execute_order functionality where buy order is matched with sell order when multiple order exists on both sides ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 99.0, 1.0);

	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 101.0, 1.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::SELL, 98.0, 1.0);


	/* Add Order's */
	orderBook1.execute_order(order_1);
	orderBook1.execute_order(order_2);
	orderBook1.execute_order(order_3);
	auto transaction_1 = orderBook1.execute_order(order_4);

	/* Highest buy order (100.0) will match with lowest sell order (98.0) */
	REQUIRE(orderBook1.get_buy_tree().get_length() == 1);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 1);

	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_3->get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order_4->get_status() == OrderStatus::FILLED);

	REQUIRE(transaction_1.get_length() == 2);

}

TEST_CASE("Test to check the execute_order functionality where incoming_order is partially filled ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 5.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 100.0, 2.0);

	/* Add Order's */
	orderBook1.execute_order(order_1);
	auto transaction_1 = orderBook1.execute_order(order_2);

	/* Highest buy order (100.0) will match with lowest sell order (100.0) */
	REQUIRE(orderBook1.get_buy_tree().get_length() == 1);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 0);

	REQUIRE(order_1->get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);

	REQUIRE(transaction_1.get_length() == 2);

}

TEST_CASE("Test to check the execute_order functionality where incoming_order is fully matched and OrderBook is empty ", "[execute_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 100.0, 5.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::SELL, 100.0, 5.0);

	/* Add Order's */
	orderBook1.execute_order(order_1);
	auto transaction_1 = orderBook1.execute_order(order_2);

	/* Highest buy order (100.0) will match with lowest sell order (100.0) */
	REQUIRE(orderBook1.get_buy_tree().get_length() == 0);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 0);

	REQUIRE(order_1->get_status() == OrderStatus::FILLED);
	REQUIRE(order_2->get_status() == OrderStatus::FILLED);

	REQUIRE(transaction_1.get_length() == 2);

}

TEST_CASE("Test to check the cancel_order functionality of OrderBook class ", "[cancel_order]") {

	/* Create OrderBook */
	OrderBook orderBook1;

	/* Create Order's */
	std::shared_ptr<Order> order_1 = std::make_shared<Order>(1, OrderType::BUY, 98.0, 1.0);
	std::shared_ptr<Order> order_2 = std::make_shared<Order>(2, OrderType::BUY, 99.0, 1.0);

	std::shared_ptr<Order> order_3 = std::make_shared<Order>(3, OrderType::SELL, 101.0, 1.0);
	std::shared_ptr<Order> order_4 = std::make_shared<Order>(4, OrderType::SELL, 100.0, 1.0);

	/* Add Order's */
	orderBook1.execute_order(order_1);
	orderBook1.execute_order(order_2);
	orderBook1.execute_order(order_3);
	orderBook1.execute_order(order_4);
	orderBook1.cancel_order(order_1);

	/* Highest buy order (100.0) will match with lowest sell order (100.0) */
	REQUIRE(orderBook1.get_buy_tree().get_length() == 1);
	REQUIRE(orderBook1.get_sell_tree().get_length() == 2);

}