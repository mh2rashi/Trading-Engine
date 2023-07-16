#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\limit_order.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\transactions.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_tree.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_book.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>


/* Function to split incoming_order read from file */
std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        parts.push_back(item);
    }

    return parts;
}  

int main() {

    /* Open the file to read it */
    std::fstream file("sample_large_order_10k.txt", std::ios::in);

    /* Create your OrderBook object */
    OrderBook orderBook;

    /* Total Orders submitted */
    int total_orders_submitted = 0;

    /* Total filled Orders */
    int total_filled_orders = 0;

    /* Start the timer */
    auto startTime = std::chrono::high_resolution_clock::now();

    /* Vector for order details */
    std::vector<std::string> parts;

    std::string line;
    while (std::getline(file, line)) {

        /* Vector for order components */
        parts = splitString(line, ',');

        /* Access the individual values */
        OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
        unsigned int order_id = std::stoi(parts[1]);
        float quantity = std::stof(parts[2]);
        float price = std::stof(parts[3]);

        /* Create the order object */
        std::shared_ptr<Order> order = std::make_shared<Order>(std::move(order_id), std::move(order_type), std::move(quantity), std::move(price));

        /* Process the order through the order book */
        total_orders_submitted += 1;
        orderBook.execute_order(order).get_executed_orders();
    }

    /* Stop the timer */
    auto endTime = std::chrono::high_resolution_clock::now();

    /* Calculate the elapsed time in seconds */
    double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();

    /* Print the overall elapsed time for order execution */
    std::cout << "A total of " << total_orders_submitted << " orders submitted " << std::endl;
    std::cout << "A total of " << total_orders_submitted - orderBook.get_buy_tree().get_length() + orderBook.get_sell_tree().get_length() << " orders were filled in " << elapsedTime << " seconds" << std::endl;
    std::cout << "A total of " << orderBook.get_buy_tree().get_length() + orderBook.get_sell_tree().get_length() << " orders still within OrderBook " << std::endl;

    /* Close file */
    file.close();

    return 0;

}







