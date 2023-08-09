
# Multithreaded Limit Orderbook and Trading Engine

A fast, multithreaded and efficient trade matching engine built in C++ utilizing Chrono, BOOST, and STL libraries. A demo can be found [here.]()

- **FIFO** (First in first out) approach is used to store orders'.

- **Price Priority** algorithm is used for matching orders - where oldest buy order at the highest buying price is matched with the oldest sell order at the lowest selling price.

## Architecture

Code is developed using efficient data structures (Binary Trees, Unordered Set and Unordered Map) keeping in mind High Frequency Trading (**HFT**) objectives. 

Time complexity for key operations are as follows:

    - Add: O(log M) for the first order at a limit, O(1) for all others
    - Cancel: O(1)
    - Execute: O(1)

    Where M is the number of price limits and N is the number of orders (generally M << N).

## Input Data Format

Order scripts expect the following input format (.csv or .txt file)

    - Buy, Sell or Cancel: BUY or SELL or CANCEL
    - Order ID: Unique or autoincrementing Integer (0,1,2...)
    - Price: Positive float (1.00, 2.5...) 
    - Quantity: Positive float (100.00, 200.00...)

Sample orders are as follows:

    - SELL,1,31502,7500
    - BUY,2,31601,20000
    - CANCEL,1,31502,7500

## References

1. [Limit Order Book](https://web.archive.org/web/20110410160306/http://howtohft.wordpress.com:80/2011/02/15/how-to-build-a-fast-limit-order-book)
2. [How to Build a Fast Limit Order Book](https://web.archive.org/web/20110410160306/http://howtohft.wordpress.com:80/2011/02/15/how-to-build-a-fast-limit-order-book)
