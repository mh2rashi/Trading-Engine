#pragma once
#include "..\include\TradingEngine\order_tree.h"

#include <cmath>

#include <sstream>
using std::ostringstream;

#include <memory>
using std::shared_ptr;

#include "..\include\TradingEngine\transactions.h" 
#include "..\include\TradingEngine\order_type.h"
#include "..\include\TradingEngine\order_status.h"
#include "..\include\TradingEngine\order.h"
#include "..\include\TradingEngine\limit_order.h"

size_t OrderTree::getLength() const {
    return order_set.size();
}

const float& OrderTree::getMinPrice() {
    return min_price;
}

void OrderTree::setMinPrice(const float& new_min) {
    min_price = new_min;
}

const float& OrderTree::getMaxPrice() {
    return max_price;
}

void OrderTree::setMaxPrice(const float& new_max) {
    max_price = new_max;
}

const OrderType& OrderTree::getType() const {
    return type;
}

void OrderTree::addPriceOrder(Order& incoming_order) {

    /* Add to tree */
    order_tree[incoming_order.getPrice()]++;

    /* Add to price_map */
    price_map[incoming_order.getPrice()].addOrder(incoming_order);

    /* Add to order_set */
    order_set.insert(incoming_order.getID());

    // Update max and min prices
    if (std::isnan(min_price) || min_price > incoming_order.getPrice()) {
        min_price = incoming_order.getPrice();
    }

    if (std::isnan(max_price) || max_price < incoming_order.getPrice()) {
        max_price = incoming_order.getPrice();
    }
}


void OrderTree::deletePriceOrder(const std::shared_ptr<Order>& order_to_delete) {

    /* Remove from order_set */
    if (order_set.erase(order_to_delete->getID()) == 0) {
        return;
    }

    /* Remove Order from price_map */
    price_map[order_to_delete->getPrice()].deleteOrder(order_to_delete);

    /* If the LimitOrder has no Order's left, we will remove it from the price dictionary as well */
    if (price_map[order_to_delete->getPrice()].getListLength() <= 0) {
        price_map.erase(order_to_delete->getPrice());
    }

    /* Remove from Tree */
    order_tree[order_to_delete->getPrice()]--;

    /* If the LimitOrder has no Order's left, we will remove it from the Tree as well */
    if (order_tree[order_to_delete->getPrice()] <= 0) {
        order_tree.erase(order_to_delete->getPrice());
    }

    /* Update max and min prices */
    if (max_price == order_to_delete->getPrice()) {

        if (order_tree.empty()) {
            max_price = std::numeric_limits<float>::quiet_NaN();
        }

        else if (price_map.find(order_to_delete->getPrice()) == price_map.end()) {
            max_price = order_tree.rbegin()->first;
        }
    }

    if (min_price == order_to_delete->getPrice()) {

        if (order_tree.empty()) {
            min_price = std::numeric_limits<float>::quiet_NaN();
        }

        else if (price_map.find(order_to_delete->getPrice()) == price_map.end()) {
            min_price = order_tree.begin()->first;
        }
    }
}

void OrderTree::deleteLimitPrice(const float& limit_price) {

    /* Remove from price_map */
    price_map.erase(limit_price);

    /* Remove from tree */
    order_tree.erase(limit_price);

    /* Update max and min prices */
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
            min_price = order_tree.begin()->first;
        }
    }
}

/* Match incoming_order and return executed transactions */
void OrderTree::matchPriceOrder(Order& incoming_order, ostringstream& store_transaction) {

    /* Check if no Orders exist */
    if (price_map.empty()) {
        return;
    }

    /* Can't match two of the same Order types. */
    if (incoming_order.getType() == type) {
        return;
    }

    /* Buy Orders will match with the Min(Sell Tree).  We'll start the lowest price node
       (each node is a LimitOrder, which is a doubly LinkedList) and match until the incoming_order
       if filled. If the orders within the node are fully matched, we will move to the next node
       (aka second lowest price node). Sell Order will similarly be be matched with Max (Buy Tree). */

    auto price_to_match = (incoming_order.getType() == OrderType::BUY) ? getMinPrice() : getMaxPrice();
    auto& orderPeakQuantity = incoming_order.getPeakQuantity();
    auto& orderPrice = incoming_order.getPrice();
    auto& orderType = incoming_order.getType();

    /* Only match Buy and Sell orders where Sell price < Buy price */
    while (orderPeakQuantity > 0 &&
        ((orderType == OrderType::BUY && orderPrice >= price_to_match) ||
            (orderType == OrderType::SELL && orderPrice <= price_to_match))) {

        /* We access the appropriate LimitOrderand then start matching */
        LimitOrder& matching_LimitOrder = price_map[price_to_match];
        matching_LimitOrder.matchOrder(incoming_order, order_set, store_transaction);

        /* Delete LimitOrder if its empty */
        if (matching_LimitOrder.getListLength() == 0) {
            deleteLimitPrice(price_to_match);
            /* If there are no Order's left to match with the incoming Order, we exit the loop */
            if (price_map.size() == 0) {
                break;
            }
            /* Otherwise we go to the next best price to match */
            price_to_match = (incoming_order.getType() == OrderType::BUY) ? getMinPrice() : getMaxPrice();
        }
    }
    return;
}
