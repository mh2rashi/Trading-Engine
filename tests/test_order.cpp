#define CONFIG_CATCH_MAIN
#include "..\include\third_party_library\Catch2\catch_amalgamated.h"

#include <string>
using std::string;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"
#include "..\boost_1_82_0\boost\optional\optional_io.hpp"

TEST_CASE("Create a simple order using the Order class", "[Constructor][no UUID for order id]") {

	// Create Order
	Order order1("ETH", 1, OrderType::BUY, 100.00, 10.0);

	REQUIRE(order1.getType() == OrderType::BUY);
	REQUIRE(order1.getPrice() == 100.0);
	REQUIRE(order1.getOriginalQuantity() == 10.0);
	REQUIRE(order1.next_order == nullptr);
	REQUIRE(order1.prev_order == nullptr);

}

TEST_CASE("Test all getter functions", "[Getter functions]") {

	// Create Order
	Order order2("ETH", 2, OrderType::BUY, 100.00, 10.0);

	REQUIRE(order2.getID() == 2);
	REQUIRE(order2.getType() == OrderType::BUY);
	REQUIRE(order2.getPrice() == 100.0);
	REQUIRE(order2.getStatus() == OrderStatus::NEW_ORDER);
	REQUIRE(order2.getOriginalQuantity() == 10.0);
	REQUIRE(order2.getQuantityTransacted() == 0);
	REQUIRE(order2.getPeakQuantity() == 10.0);
	REQUIRE(order2.getTotalQuantityTransacted() == 0);

}

TEST_CASE("Test all setter functions", "[Setter functions]") {

	// Create Order
	Order order3("ETH", 3, OrderType::BUY, 100.00, 10.0);

	// Update Order
	order3.setOrderStatus(OrderStatus::PARTIALLY_FILLED);
	order3.setTotalQuantityTransacted(1);
	order3.setQuantityTransacted(1);
	order3.setPeakQuantity(-1);

	REQUIRE(order3.getStatus() == OrderStatus::PARTIALLY_FILLED);
	REQUIRE(order3.getTotalQuantityTransacted() == 1);
	REQUIRE(order3.getQuantityTransacted() == 1);
	REQUIRE(order3.getPeakQuantity() == 9);

}


TEST_CASE("Match orders for a complete trade", "[match][complete trade]") {

	// Create Orders
	Order order6("ETH", 6, OrderType::BUY, 100.0, 9.0);
	Order order_7("ETH", 7, OrderType::SELL, 99.0, 10.0);

	REQUIRE(order6.Match(order_7));
	REQUIRE(order6.getPeakQuantity() <= order_7.getPeakQuantity());
	REQUIRE(order6.getStatus() == OrderStatus::FULFILLED);

}

TEST_CASE("Match orders for a partial trade", "[match][partial trade]") {

	// Create Orders
	Order order8("ETH", 8, OrderType::BUY, 100.0, 11.0);
	Order order_9("ETH", 9, OrderType::SELL, 99.0, 10.0);

	REQUIRE_FALSE(order8.Match(order_9));
	REQUIRE(order8.getPeakQuantity() > order_9.getPeakQuantity());
	REQUIRE(order8.getStatus() == OrderStatus::PARTIALLY_FILLED);

}
