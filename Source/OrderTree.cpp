#pragma once
#include "Headers/OrderTree.h"

using std::shared_ptr;


int OrderTree::get_length() const {
	return order_map.size();
}

void OrderTree::add_limit_price(const float& limit_price) {
    auto limit_order = LimitOrder();

    // Add to Tree
    order_tree[limit_price] = limit_order;

    // Add to Price map
    price_map[limit_price] = limit_order;

    // Update max and min prices
    if (min_price == std::numeric_limits<float>::quiet_NaN() || min_price > limit_price) {
        min_price = limit_price;
    }

    if (max_price == std::numeric_limits<float>::quiet_NaN() || max_price < limit_price) {
        max_price = limit_price;
    }
}

void OrderTree::delete_limit_price(const float& limit_price) {

    // Remove corresponding orders' from order map
    shared_ptr<Order> current_order = price_map[limit_price].get_head();
    for (; current_order != nullptr; current_order = current_order->next_order) {
        order_map.erase(current_order->id);
        }

    // Remove from price map
    price_map.erase(limit_price);
    
    // Remove from Tree
    order_tree.erase(limit_price);

    // Update max and min prices
    if (max_price == limit_price) {

        if (order_tree.empty()) {
            max_price = std::numeric_limits<float>::quiet_NaN();
        }

        else {
            max_price = order_tree.rbegin()->first;
        }
    }

    if (min_price == limit_price) {

        if (order_tree.empty()) {
            min_price = std::numeric_limits<float>::quiet_NaN();
        }

        else {
            max_price = order_tree.begin()->first;
        }
    }
}

void OrderTree::add_price_order(const std::shared_ptr<Order>& incoming_order) {
    
    // Check and create the respective LimitOrder
    if (!price_map.count(incoming_order->price)) {
        add_limit_price(incoming_order->price);
    }
    
    // Add to Tree
    order_tree[incoming_order->price].add_order(incoming_order);
    
    // Add to price map
    price_map[incoming_order->price].add_order(incoming_order);

    // Add to Order Map
    order_map[incoming_order->id] = incoming_order;

}

void OrderTree::delete_price_order(const std::shared_ptr<Order>& order_to_delete) {
    
    // Remove from order map
    order_map.erase(order_to_delete->id);
   
    // Remove from price map
    price_map[order_to_delete->price].delete_order(order_to_delete);

    // Remove from Tree
    order_tree[order_to_delete->price].delete_order(order_to_delete);

    // Update max and min prices
    if (max_price == order_to_delete->price) {

        if (order_tree.empty()) {
            max_price = std::numeric_limits<float>::quiet_NaN();
        }

        else if (price_map.find(order_to_delete->price) == price_map.end()) {
            max_price = order_tree.rbegin()->first;
        }
    } 

    if (min_price == order_to_delete->price) {

        if (order_tree.empty()) {
            min_price = std::numeric_limits<float>::quiet_NaN();
        }

        else if (price_map.find(order_to_delete->price) == price_map.end()) {
            max_price = order_tree.begin()->first;
        }
    }
}

void OrderTree::match_price_order(const std::shared_ptr<Order>& incoming_order) {
    // Check if no Orders exist
    if ((price_map.empty())) {
        return;
    }

    // Buy Orders will match with the Min(Sell Tree).
    //   We'll start the lowest price node (each node is a LimitOrder, which is a doubly LinkedList)
    //   and match until the incoming_order if filled. If the orders within the node are fully matched,
    //   we will move to the next node (aka second lowest price node).
    // Sell Order will similarly be be matched with Max(Buy Tree).
    auto transactions = Transactions();
    auto price_to_match = (incoming_order->type == "Buy") ? min_price : max_price;

    // We will match 
    // Only match Buy and Sell orders where Sell price < Buy price
    while (incoming_order->peak_quantity > 0 &&
        ((incoming_order->type == "Buy" && incoming_order->price >= price_to_match) ||
            (incoming_order->type == "Sell" && incoming_order->price <= price_to_match))) {
        
        // We access the appropriate LimitOrder and then start matching
        LimitOrder matching_LimitOrder = price_map[price_to_match];
        matching_LimitOrder.match_order(incoming_order, order_map, transactions);

        // Delete executed prices
        if (matching_LimitOrder.get_length() == 0) {
            delete_limit_price(price_to_match);
            // If there are no Order's to match with the incoming Order, we exit the loop
            if (price_map.size() == 0) {
                break;
            }
            // Otherwise we go to the next best price to match
            auto price_to_match = (incoming_order->type == "Buy") ? min_price : max_price;
        }
    }
    return;
}