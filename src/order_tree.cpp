#pragma once
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order_tree.h"

#include <cmath>
#include <memory>
using std::shared_ptr;

#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\order.h"
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\transactions.h" 
#include "C:\Users\Hamza Rashid\Desktop\C++\Trading Engine\include\TradingEngine\limit_order.h"

/* Return length of Order's within the Tree */
int OrderTree::get_length() const {
	return order_map_.size();
}

/* Get min price within Ordertree */
float OrderTree::get_min() {
    return min_price_;
}

/* Set min price within OrderTree */
void OrderTree::set_min(const float& new_min) {
    min_price_ = new_min;
}

/* Get max price within Ordertree */
float OrderTree::get_max() {
    return max_price_;
}

/* Set min price within OrderTree */
void OrderTree::set_max(const float& new_max) {
    max_price_ = new_max;
}

/* Return Tree type */
const OrderType& OrderTree::get_type() const {
    return type_;
}

/* Add Order into Tree */
void OrderTree::add_price_order(const std::shared_ptr<Order>& incoming_order) {
    
    /* Check and create the respective LimitOrder */
    if (!price_map_.count(incoming_order->get_price())) {
        add_limit_price(incoming_order->get_price());
    }
    
    /* Add to tree */
    order_tree_[incoming_order->get_price()] += 1; // This could be just += 1
    
    /* Add to price_map */
    price_map_[incoming_order->get_price()].add_order(incoming_order);

    /* Add to order_map */
    order_map_[incoming_order->get_id()] = incoming_order;

    // Update max and min prices
    if (std::isnan(get_min()) || get_min() > incoming_order->get_price()) {
        set_min(incoming_order->get_price());
    }

    if (std::isnan(get_max()) || get_max() < incoming_order->get_price()) {
        set_max(incoming_order->get_price());
    }
}

/* Delete Order from Tree */
void OrderTree::delete_price_order(const std::shared_ptr<Order>& order_to_delete) {
    
    /* Remove from order_map */
    if (order_map_.erase(order_to_delete->get_id()) == 0) {
        return;
    }
   
    /* Remove Order from price_map */
    price_map_[order_to_delete->get_price()].delete_order(order_to_delete);
    
    /* If the LimitOrder has no Order's left, we will remove it from the price dictionary as well */
    if (price_map_[order_to_delete->get_price()].get_list_length() <= 0) {
        price_map_.erase(order_to_delete->get_price());
    }

    /* Remove from Tree */
    order_tree_[order_to_delete->get_price()] -= 1; 

    /* If the LimitOrder has no Order's left, we will remove it from the Tree as well */
    if (order_tree_[order_to_delete->get_price()] <= 0) {
        order_tree_.erase(order_to_delete->get_price());
    }

    /* Update max and min prices */
    if (get_max() == order_to_delete->get_price()) {

        if (order_tree_.empty()) {
            set_max(std::numeric_limits<float>::quiet_NaN());
        }

        else if (price_map_.find(order_to_delete->get_price()) == price_map_.end()) {
            set_max(order_tree_.rbegin()->first);
        }
    } 

    if (get_min() == order_to_delete->get_price()) {

        if (order_tree_.empty()) {
            set_min(std::numeric_limits<float>::quiet_NaN());
        }

        else if (price_map_.find(order_to_delete->get_price()) == price_map_.end()) {
            set_min(order_tree_.begin()->first);
        }
    }
}

/* Add new limit_order into Tree */
void OrderTree::add_limit_price(const float& limit_price) {
    auto limit_order = LimitOrder();

    /* Add to price_map */
    price_map_[limit_price] = limit_order;

    /* Add to tree */
    order_tree_[limit_price] = price_map_[limit_price].get_list_length();
}

/* Delete limit_order from tree and price_map */
void OrderTree::delete_limit_price(const float& limit_price) {

    /* Remove from price_map */
    price_map_.erase(limit_price);
    
    /* Remove from tree */
    order_tree_.erase(limit_price);

    /* Update max and min prices */
    if (get_max() == limit_price) {

        if (order_tree_.empty()) {
           set_max(std::numeric_limits<float>::quiet_NaN());
        }

        else {
            set_max(order_tree_.rbegin()->first);
        }
    }

    if (get_min() == limit_price) {

        if (order_tree_.empty()) {
            set_min(std::numeric_limits<float>::quiet_NaN());
        }

        else {
            set_min(order_tree_.begin()->first);
        }
    }
}

/* Match incoming_order and return executed transactions */
Transactions OrderTree::match_price_order(const std::shared_ptr<Order>& incoming_order) {
    /* Transactions for the incoming_order */
    auto transactions = Transactions();
    
    /* Check if no Orders exist */ 
    if (price_map_.empty()) { // Maybe we coud have some other condition for this?
        return transactions;
    }

    /* Can't match two of the same Order types. */
    if (incoming_order->get_type() == get_type()) {
        return transactions;
    }

    /* Buy Orders will match with the Min(Sell Tree).  We'll start the lowest price node
       (each node is a LimitOrder, which is a doubly LinkedList) and match until the incoming_order
       if filled. If the orders within the node are fully matched, we will move to the next node
       (aka second lowest price node). Sell Order will similarly be be matched with Max(Buy Tree). */

    auto price_to_match = (incoming_order->get_type() == OrderType::BUY) ? get_min() : get_max();

    /* Only match Buy and Sell orders where Sell price < Buy price */
    while (incoming_order->peak_quantity > 0 &&
        ((incoming_order->get_type() == OrderType::BUY && incoming_order->get_price() >= price_to_match) ||
            (incoming_order->get_type() == OrderType::SELL && incoming_order->get_price() <= price_to_match))) {
        
        /* We access the appropriate LimitOrderand then start matching */
        LimitOrder matching_LimitOrder = price_map_[price_to_match];
        matching_LimitOrder.match_order(incoming_order, order_map_, transactions);

        /* Delete LimitOrder if its empty */
        if (matching_LimitOrder.get_list_length() == 0) {
            delete_limit_price(price_to_match);
            /* If there are no Order's left to match with the incoming Order, we exit the loop */
            if (price_map_.size() == 0) {
                break;
            }
            /* Otherwise we go to the next best price to match */
            price_to_match = (incoming_order->get_type() == OrderType::BUY) ? get_min() : get_max();
        }
    }
    return transactions;
}