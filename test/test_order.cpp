#define CONFIG_CATCH_MAIN
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\libs\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"

TEST_CASE("Create a simple order using the Order class", "[Constructor][no UUID for order id]") {

	// Create Order
	Order order1(1, OrderType::BUY, 100.00, 10.0);

	REQUIRE(order1.get_type() == OrderType::BUY);
	REQUIRE(order1.get_price() == 100.0);
	REQUIRE(order1.get_original_quantity() == 10.0);
	REQUIRE(order1.next_order == nullptr);
	REQUIRE(order1.prev_order == nullptr);

}

TEST_CASE("Test all getter functions", "[Getter functions]") {

	// Create Order
	Order order2(2, OrderType::BUY, 100.00, 10.0);

	REQUIRE(order2.get_id() == 2);
	REQUIRE(order2.get_type() == OrderType::BUY);
	REQUIRE(order2.get_price() == 100.0);
	REQUIRE(order2.get_status() == OrderStatus::NEW_ORDER);
	REQUIRE(order2.get_original_quantity() == 10.0);
	REQUIRE(order2.get_quantity_transacted() == 0);
	REQUIRE(order2.get_peak_quantity() == 10.0);
	REQUIRE(order2.get_total_quantity_transacted() == 0);
	
}

TEST_CASE("Test all setter functions", "[Setter functions]") {

	// Create Order
	Order order3(3, OrderType::BUY, 100.00, 10.0);

	// Update Order
	order3.set_order_status(OrderStatus::PARTIALLY_FILLED);
	order3.set_total_quantity_transacted(1);
	order3.set_quantity_transacted(1);
	order3.set_peak_quantity(-1);

	REQUIRE(order3.get_status() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order3.get_total_quantity_transacted() == 1);
	REQUIRE(order3.get_quantity_transacted() == 1);
	REQUIRE(order3.get_peak_quantity() == 9);

}

TEST_CASE("Don't match orders of the same type", "[match][same orders]") {

	// Create Orders
	Order order4(4, OrderType::BUY, 100.0, 10.0);
	std::shared_ptr<Order> order_5 = std::make_shared<Order>(5, OrderType::BUY, 100.0, 10.0);

	REQUIRE_FALSE(order4.match(order_5));

}

TEST_CASE("Match orders for a complete trade", "[match][complete trade]") {

	// Create Orders
	Order order6(6, OrderType::BUY, 100.0, 9.0);
	std::shared_ptr<Order> order_7 = std::make_shared<Order>(7, OrderType::SELL, 99.0, 10.0);

	REQUIRE(order6.match(order_7));
	REQUIRE(order6.get_peak_quantity() <= order_7->get_peak_quantity());
	REQUIRE(order6.get_status() == OrderStatus::FILLED);

}

TEST_CASE("Match orders for a partial trade", "[match][partial trade]") {

	// Create Orders
	Order order8(8, OrderType::BUY, 100.0, 11.0);
	std::shared_ptr<Order> order_9 = std::make_shared<Order>(9, OrderType::SELL, 99.0, 10.0);

	REQUIRE_FALSE(order8.match(order_9));
	REQUIRE(order8.get_peak_quantity() > order_9->get_peak_quantity());
	REQUIRE(order8.get_status() == OrderStatus::PARTIALLY_FILLED);

}