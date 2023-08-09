#include "..\include\TradingEngine\order.h"
#include "..\include\TradingEngine\limit_order.h"
#include "..\include\TradingEngine\transactions.h"
#include "..\include\TradingEngine\order_tree.h"
#include "..\include\TradingEngine\order_book.h"


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <mutex>
#include <thread>
#include<atomic>
#include <optional>
#include <condition_variable>
#include <future>


using std::shared_ptr;
using std::queue;
using std::mutex;
using std::thread;
using std::string;
using std::atomic;
using std::condition_variable;
using std::vector;
using std::optional;


//////////////////////////////////////////////////////////// Single Thread /////////////////////////////////////////////////////////////////////////////////////////////

/* Function to split incoming_order read from file */
Order convertOrder(const std::string& input) {
    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string item;


    while (std::getline(ss, item, ',')) {
        parts.push_back(item);
    }

    /* Access the individual values */
    OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
    unsigned int order_id = std::stoi(parts[1]);
    float quantity = std::stof(parts[2]);
    float price = std::stof(parts[3]);

    return Order(std::move(order_id), std::move(order_type), std::move(quantity), std::move(price));
}

int main() {

    /* Open the file to read it */
    std::fstream file("sample_large_order_10k.txt", std::ios::in);

    /* Create your OrderBook object */
    OrderBook orderBook;

    /* Total Orders submitted */
    int total_orders_submitted = 0;

    std::string line;

    /* Start the timer */
    auto startTime = std::chrono::high_resolution_clock::now();

    while (std::getline(file, line)) {

        //std::cout << line << std::endl;

        /* Read order and convert it */
        Order order(convertOrder(line));

        /* Process the order through the order book */
        orderBook.processOrder(std::ref(order));
        total_orders_submitted += 1;
    }

    /* Stop the timer */
    auto endTime = std::chrono::high_resolution_clock::now();

    /* Calculate the elapsed time in seconds */
    double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();

    /* Print the overall elapsed time for order execution */
    std::cout << "A total of " << total_orders_submitted << " orders submitted " << std::endl;
    std::cout << "A total of " << total_orders_submitted - (orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength()) << " orders were filled in " << elapsedTime << " seconds" << std::endl;
    std::cout << "A total of " << orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength() << " orders still within OrderBook " << std::endl;


    /* Close file */
    file.close();

    return 0;

}

/////////////////////////////////////////////////////////////////////////// Code using Locks and Mutexes /////////////////////////////////////////////////////////////////

///* Mutex to ensure thread safe access to the queue */
//mutex queueMutex; 
//
///* Atomic boolean flag to notify order processing thread to complete */
//std::atomic<bool> stopProcessing(false); 
//
//queue<Order> orderQueue;
//
//
///* Function to split incoming_order read from file */
//Order convertOrder(const std::string& input) {
//    std::vector<std::string> parts;
//    std::stringstream ss(input);
//    std::string item;
//
//
//    while (std::getline(ss, item, ',')) {
//        parts.push_back(item);
//    }
//
//    /* Access the individual values */
//    OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
//    unsigned int order_id = std::stoi(parts[1]);
//    float quantity = std::stof(parts[2]);
//    float price = std::stof(parts[3]);
//
//    return Order(std::move(order_id), std::move(order_type), std::move(quantity), std::move(price));
//}
//
///* Function to process incoming order file */
//void readOrdersFromFile(const std::string& filename) {
//    /* Open the file to read it */
//    std::ifstream file(filename, std::ios::in);
//    
//    std::string line;
//
//    while (std::getline(file, line)) {
//
//        //std::cout << line << std::endl;
//
//        /* Read order and convert it */
//        Order order = convertOrder(line);
//
//        // Push the order into the queue
//        std::unique_lock<mutex> lock(queueMutex);
//
//        orderQueue.emplace(std::move(order));
//
//        lock.unlock();
//    }
//
//    /* Set the termination flag to true */
//    stopProcessing = true;
//
//    /* Close file */
//    file.close();
//}
////
///* Function to feed order to Mathcing Engine */
//void processOrders(OrderBook& order_book, int& total_orders_submitted) {
//    while (true) {
//        std::unique_lock<mutex> lock(queueMutex);
//        if (!orderQueue.empty()) {
//            // Pop the order from the queue
//            Order order = orderQueue.front();
//            orderQueue.pop();
//            //std::cout << order->GetID() << "out of queue" << std::endl;
//            lock.unlock();
//
//           // std::cout << order.getID() << std::endl;
//            // Feed the order into the trading engine for processing
//            order_book.processOrder(std::ref(order));
//            total_orders_submitted++;
//        }
//        else {
//            /* Check the termination flag */
//            if (stopProcessing) {
//                /* Exit the loop if no more ordersand stopProcessing is true */
//                //std::cout << "exit" << std::endl;
//                break;
//            }
//
//            lock.unlock();
//        }
//    }
//}
//
//int main() {
//  
//    /* Create your OrderBook object */
//    OrderBook orderBook;
//
//    /* Total Orders submitted */
//    int orders_submitted = 0;
//
//    /* Start the timer */
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    /* Start the order reading thread */
//    thread orderReaderThread(readOrdersFromFile, "sample_large_order_10k.txt");
//
//    /* Start the order processing thread */
//    thread orderProcessorThread(processOrders, std::ref(orderBook), std::ref(orders_submitted));
//
//    /* Join the threads and wait for them to finish */
//    orderReaderThread.join();
//    orderProcessorThread.join();
//
//    /* Stop the timer */
//    auto endTime = std::chrono::high_resolution_clock::now();
//
//    /* Calculate the elapsed time in seconds */
//    double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
//
//    /* Print the overall elapsed time for order execution */
//    std::cout << "A total of " << orders_submitted << " orders submitted " << std::endl;
//    std::cout << "A total of " << orders_submitted - (orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength()) << " orders were filled in " << elapsedTime << " seconds" << std::endl;
//    std::cout << "A total of " << orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength() << " orders still within OrderBook " << std::endl;
//
//    return 0;
//
//}


///////////////////////////////////////////////////////////////////////////////////////////////// Code using Conditional Variable  //////////////////////////////////////////////////////////////////

//mutex queueMutex;
//
//condition_variable queueCV;
//
//queue<Order> orderQueue;
//
//bool stopProcessing(false);
//
//
///* Function to split incoming_order read from file */
//Order convertOrder(const std::string& input) {
//    std::vector<std::string> parts;
//    std::stringstream ss(input);
//    std::string item;
//
//
//    while (std::getline(ss, item, ',')) {
//        parts.push_back(item);
//    }
//
//    /* Access the individual values */
//    OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
//    unsigned int order_id = std::stoi(parts[1]);
//    float quantity = std::stof(parts[2]);
//    float price = std::stof(parts[3]);
//
//    return Order(std::move(order_id), std::move(order_type), std::move(quantity), std::move(price));
//}
//
//
///* Function to process incoming order file */
//void readOrdersFromFile(const std::string& filename) {
//    std::ifstream file(filename, std::ios::in);
//
//    /* If the file doesn't open */
//    if (!file) {
//        std::cerr << "Error opening file: " << filename << std::endl;
//        return; // Handle the error gracefully
//    }
//
//    std::string line;
//    while (std::getline(file, line)) {
//        //std::cout << line << std::endl;
//
//        // Read order and convert it
//        Order order = convertOrder(line);
//
//        // Emplace the Order into the queue
//        {
//            std::unique_lock<mutex> lock(queueMutex);
//            orderQueue.emplace(std::move(order));
//        }
//
//        queueCV.notify_one();
//    }
//
//    stopProcessing = true; // Set the termination flag
//
//    file.close();
//};
//
///* Function to feed order to Matching Engine */
//void processOrders(OrderBook& order_book, int& total_orders_submitted) {
//    while (true) {
//        // Wait until there are new orders or termination is requested
//        std::unique_lock<mutex> lock(queueMutex);
//        queueCV.wait(lock, [&]() { return !orderQueue.empty() || stopProcessing; });
//
//        if (!orderQueue.empty()) {
//            // Get the order from the queue using move semantics
//            Order order = std::move(orderQueue.front());
//            orderQueue.pop();
//
//            // Unlock the mutex before processing the order
//            lock.unlock();
//
//            // Feed the order into the trading engine for processing
//            order_book.processOrder(std::ref(order));
//            total_orders_submitted++;
//        }
//        else {
//            // Exit the loop if no more orders and stopProcessing is true
//            break;
//        }
//    }
//}
//
//int main() {
//  
//    /* Create your OrderBook object */
//    OrderBook orderBook;
//
//    /* Total Orders submitted */
//    int orders_submitted = 0;
//
//    /* Start the timer */
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    /* Start the order reading thread */
//    thread orderReaderThread(readOrdersFromFile, "sample_large_order_10k.txt");
//
//    /* Start the order processing thread */
//    thread orderProcessorThread(processOrders, std::ref(orderBook), std::ref(orders_submitted));
//
//    /* Join the threads and wait for them to finish */
//    orderReaderThread.join();
//    orderProcessorThread.join();
//
//    /* Stop the timer */
//    auto endTime = std::chrono::high_resolution_clock::now();
//
//    /* Calculate the elapsed time in seconds */
//    double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
//
//    /* Print the overall elapsed time for order execution */
//    std::cout << "A total of " << orders_submitted << " orders submitted " << std::endl;
//    std::cout << "A total of " << orders_submitted - (orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength()) << " orders were filled in " << elapsedTime << " seconds" << std::endl;
//    std::cout << "A total of " << orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength() << " orders still within OrderBook " << std::endl;
//
//    return 0;
//
//}

//////////////////////////////////////////////////////////////////////////////// Conditional Variable  with batching ////////////////////////////////////////////////////////////////////////////////////////

//std::mutex queueMutex;
//std::condition_variable queueCV;
//std::queue<Order> orderQueue;
//bool stopProcessing = false;
//
///* Function to split incoming_order read from file */
//Order convertOrder(const std::string& input) {
//    std::vector<std::string> parts;
//    std::stringstream ss(input);
//    std::string item;
//
//    while (std::getline(ss, item, ',')) {
//        parts.push_back(item);
//    }
//
//    /* Access the individual values */
//    OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
//    unsigned int order_id = std::stoi(parts[1]);
//    float quantity = std::stof(parts[2]);
//    float price = std::stof(parts[3]);
//
//    return Order(order_id, order_type, quantity, price);
//}
//
///* Function to process incoming order file */
//void readOrdersFromFile(const std::string& filename, const int& batchSize) {
//    std::ifstream file(filename, std::ios::in);
//
//    /* If the file doesn't open */
//    if (!file) {
//        std::cerr << "Error opening file: " << filename << std::endl;
//        return; // Handle the error gracefully
//    }
//
//
//    std::string line;
//    vector<Order> batchOrders;
//    int currentBatchSize = 0;
//
//    while (std::getline(file, line)) {
//        //std::cout << line << std::endl;
//
//        //// Read order and convert it
//        //Order order = convertOrder(line);
//
//        // Add the order to the batch
//        batchOrders.push_back(convertOrder(line));
//        ++currentBatchSize;
//
//        // If the batch size is reached, emplace the batch into the queue and clear the batch
//        if (currentBatchSize >= batchSize) {
//            {
//                std::unique_lock<std::mutex> lock(queueMutex);
//                for (auto& batchOrder : batchOrders) {
//                    orderQueue.emplace(std::move(batchOrder));
//                }
//            }
//            queueCV.notify_one();
//            batchOrders.clear();
//            currentBatchSize = 0;
//        }
//    }
//
//    // If there are remaining orders in the batch after processing all the lines in the file, emplace them into the queue
//    if (!batchOrders.empty()) {
//        {
//            std::lock_guard<std::mutex> lock(queueMutex);
//            for (auto& batchOrder : batchOrders) {
//                orderQueue.emplace(std::move(batchOrder));
//            }
//        }
//        queueCV.notify_one();
//    }
//
//    stopProcessing = true; // Set the termination flag
//
//    file.close();
//}
//
///* Function to feed order to Matching Engine */
//void processOrders(OrderBook& order_book, int& total_orders_submitted) {
//    while (true) {
//        // Wait until there are new orders or termination is requested
//        std::unique_lock<std::mutex> lock(queueMutex);
//        queueCV.wait(lock, [&]() { return !orderQueue.empty() || stopProcessing; });
//
//        if (!orderQueue.empty()) {
//            // Get the order from the queue using move semantics
//            Order order = std::move(orderQueue.front());
//            orderQueue.pop();
//
//            // Unlock the mutex before processing the order
//            lock.unlock();
//
//            // Feed the order into the trading engine for processing
//            order_book.processOrder(order);
//            total_orders_submitted++;
//        }
//        else {
//            // Exit the loop if no more orders and stopProcessing is true
//            break;
//        }
//    }
//}
//
//int main() {
//
//    /* Create your OrderBook object */
//    OrderBook orderBook;
//
//    /* Total Orders submitted */
//    int orders_submitted = 0;
//
//    /* Start the timer */
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    /* Start the order reading thread */
//    thread orderReaderThread(readOrdersFromFile, "sample_large_order_600k.txt", 10000);
//
//    /* Start the order processing thread */
//    thread orderProcessorThread(processOrders, std::ref(orderBook), std::ref(orders_submitted));
//
//    /* Join the threads and wait for them to finish */
//    orderReaderThread.join();
//    orderProcessorThread.join();
//
//    /* Stop the timer */
//    auto endTime = std::chrono::high_resolution_clock::now();
//
//    /* Calculate the elapsed time in seconds */
//    double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
//
//    /* Print the overall elapsed time for order execution */
//    std::cout << "A total of " << orders_submitted << " orders submitted " << std::endl;
//    std::cout << "A total of " << orders_submitted - (orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength()) << " orders were filled in " << elapsedTime << " seconds" << std::endl;
//    std::cout << "A total of " << orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength() << " orders still within OrderBook " << std::endl;
//
//    return 0;
//}

////////////////////////////////////////////////////////////////////////////////// Tasks //////////////////////////////////////////////////////////////////////////////////////////////

///* Function to split incoming_order read from file */
//Order convertOrder(const std::string& input) {
//    std::vector<std::string> parts;
//    std::stringstream ss(input);
//    std::string item;
//
//
//    while (std::getline(ss, item, ',')) {
//        parts.push_back(item);
//    }
//
//    /* Access the individual values */
//    OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
//    unsigned int order_id = std::stoi(parts[1]);
//    float quantity = std::stof(parts[2]);
//    float price = std::stof(parts[3]);
//
//    return Order(std::move(order_id), std::move(order_type), std::move(quantity), std::move(price));
//}
//
//
///* Read and process Orders */
//void readAndProcessOrders(const std::string& filename, OrderBook& orderBook, int& totalOrdersSubmitted) {
//    std::ifstream file(filename);
//    std::string line;
//
//    while (std::getline(file, line)) {
//        Order order = (convertOrder(line));
//        orderBook.processOrder(std::ref(order)); // Assuming processOrder takes an Order object
//
//        totalOrdersSubmitted++;
//    }
//}
//
//int main() {
//
//    OrderBook orderBook;
//    int ordersSubmitted = 0;
//
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    // Using async to asynchronously read and process orders
//    std::future<void> asyncResult = std::async(std::launch::async, readAndProcessOrders, "sample_large_order_10k.txt", std::ref(orderBook), std::ref(ordersSubmitted));
//
//    // Wait for the async operation to complete
//    asyncResult.get();
//
//    auto endTime = std::chrono::high_resolution_clock::now();
//    double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
//
//    std::cout << "A total of " << ordersSubmitted << " orders submitted " << std::endl;
//    std::cout << "A total of " << ordersSubmitted - (orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength()) << " orders were filled in " << elapsedTime << " seconds" << std::endl;
//    std::cout << "A total of " << orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength() << " orders still within OrderBook " << std::endl;
//
//    return 0;
//}


////////////////////////////////////////////////////////////////////////////// Tasks with 1000 Simulations //////////////////////////////////////////////////////////////////////

///* Function to split incoming_order read from file */
//Order convertOrder(const std::string& input) {
//    std::vector<std::string> parts;
//    std::stringstream ss(input);
//    std::string item;
//
//
//    while (std::getline(ss, item, ',')) {
//        parts.push_back(item);
//    }
//
//    /* Access the individual values */
//    OrderType order_type = (parts[0] == "BUY") ? OrderType::BUY : OrderType::SELL;
//    unsigned int order_id = std::stoi(parts[1]);
//    float quantity = std::stof(parts[2]);
//    float price = std::stof(parts[3]);
//
//    return Order(std::move(order_id), std::move(order_type), std::move(quantity), std::move(price));
//}
//
///* Read and process Orders */
//void readAndProcessOrders(const std::string& filename, OrderBook& orderBook, int& totalOrdersSubmitted) {
//    std::ifstream file(filename);
//    std::string line;
//
//    while (std::getline(file, line)) {
//        Order order = convertOrder(line);
//        orderBook.processOrder(order); // Assuming processOrder takes an Order object
//
//        totalOrdersSubmitted++;
//    }
//}
//
//int main() {
//    const int numSimulations = 1000;
//    //std::vector<double> simulationTimes;
//
//    for (int i = 0; i < numSimulations; ++i) {
//        OrderBook orderBook;
//        int ordersSubmitted = 0;
//
//        auto startTime = std::chrono::high_resolution_clock::now();
//
//        // Using async to asynchronously read and process orders
//        std::future<void> asyncResult = std::async(std::launch::async, readAndProcessOrders, "sample_large_order_10k.txt", std::ref(orderBook), std::ref(ordersSubmitted));
//
//        // Wait for the async operation to complete
//        asyncResult.get();
//
//        auto endTime = std::chrono::high_resolution_clock::now();
//        double elapsedTime = std::chrono::duration<double>(endTime - startTime).count();
//
//        std::cout << "Simulation " << i << ": " << elapsedTime << std::endl;
//
//        //simulationTimes.push_back(elapsedTime);
//
//       /* std::cout << "Simulation " << i + 1 << " - ";
//        std::cout << "A total of " << ordersSubmitted << " orders submitted ";
//        std::cout << "A total of " << ordersSubmitted - (orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength()) << " orders were filled in " << elapsedTime << " seconds ";
//        std::cout << "A total of " << orderBook.getBuyTree().getLength() + orderBook.getSellTree().getLength() << " orders still within OrderBook" << std::endl;*/
//    }
//
//    // Print the simulation times for graphing
// /*   std::cout << "Simulation times for graphing:" << std::endl;
//    for (int i = 0; i < numSimulations; ++i) {
//        std::cout << "Simulation " << i + 1 << ": " << simulationTimes[i] << " seconds" << std::endl;
//    }
//
//    return 0;*/
//}

