#define CONFIG_CATCH_MAIN
#include "Catch2/catch_amalgamated.h"

#include "../Source/Headers/Order.h"

#include "string"
using std::string;

TEST_CASE("Create a simple order using the Order class", "[Constructor]") {

    // Order properties
	unsigned int id = 123;
	string type = "Buy";
	float price = 100.0;
	float quantity = 10.0;

	// Create Order
	Order order1(id, type, price, quantity);

	REQUIRE(order1.order_status == 1);
	REQUIRE(order1.peak_quantity == 10.0);
	REQUIRE(order1.peak_quantity_left == 0);
	REQUIRE(order1.quantity_transacted == 0);
	REQUIRE(order1.next_order == nullptr);
	REQUIRE(order1.prev_order == nullptr);

}


TEST_CASE("Don't match orders of the same type", "[match][same orders]") {

	// Create Orders
	Order order1(123, "Buy", 100.0, 10.0);
	Order order2(124, "Buy", 100.0, 10.0);

	REQUIRE(order1.match(order2) == false);

}

TEST_CASE("Don't match orders of opposite type where sell price > buy price", "[match][opposite orders with price discrepancy]") {

	// Create Orders
	Order order1(123, "Buy", 100.0, 10.0);
	Order order2(124, "Sell", 101.0, 10.0);

	REQUIRE(order1.match(order2) == false);
	REQUIRE(order1.price < order2.price);

}

TEST_CASE("Match orders for a complete trade", "[match][complete trade]") {

	// Create Orders
	Order order1(123, "Buy", 100.0, 9.0);
	Order order2(124, "Sell", 99.0, 10.0);

	REQUIRE(order1.match(order2) == true);
	REQUIRE(order1.peak_quantity <= order2.peak_quantity);
	REQUIRE(order1.order_status == 3);

}

TEST_CASE("Match orders for a partial trade", "[match][partial trade]") {

	// Create Orders
	Order order1(123, "Buy", 100.0, 11.0);
	Order order2(124, "Sell", 99.0, 10.0);

	REQUIRE(order1.match(order2) == false);
	REQUIRE(order1.peak_quantity > order2.peak_quantity);
	REQUIRE(order1.order_status == 2);

}