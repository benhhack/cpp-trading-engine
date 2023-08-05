#include "OrderBook.h"
#include "CSVReader.h"
#include <map>



/** contruct reading a csv data file*/
OrderBook::OrderBook(std::string fileName)
{
    orders = CSVReader::readCSV(fileName);
}

/** Gets all known products in dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }

    // flatten the map into a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

/** Gets all orders according to sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                        std::string product,
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;

    for (OrderBookEntry& e : orders) 
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timeStamp == timestamp)
            {
                orders_sub.push_back(e);
            }
    }

    return orders_sub;
}

/** Returns the highest price of a vector of orders.*/
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders) {
        if (e.price > max) max = e.price;
    }
    return max;
}

/** Returns the lowest price of a vector of orders.*/
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders) {
        if (e.price < min) min = e.price;
    }
    return min;
}

/** Returns the earliest time in the order book*/
std::string OrderBook::getEarliestTime()
{
    // assuming orders are sorted
    return orders[0].timeStamp;
}

/** Returns the next time after the sent time 
* If there is no next it wraps around to the start.
*/
std::string OrderBook::getNextTime(std::string timeStamp)
{
    std::string next_timeStamp = "";
    // again assuming ordered by time
    for (OrderBookEntry& e : orders) 
    {
        if (e.timeStamp > timeStamp)
        {
            next_timeStamp = e.timeStamp;
            break;
        }
    }

    // wrapping to start
    if (next_timeStamp == "") next_timeStamp = orders[0].timeStamp;

    return next_timeStamp;
}

/**Inserting an order into the book*/
void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);

    // sorts the orders by timestamp
    std::sort(orders.begin(), orders.end(), OrderBookEntry::campareByTimestamp);
}

/** Matching algorithm (price-time priority with pro rata based partial order completion)*/
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // get all the asks and bids
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                product,
                                                timestamp);
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                product,
                                                timestamp);

    std::vector<OrderBookEntry> sales;
    
    // sort asks in ascending order and bids in decending order (price priority)
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    // iterate through all bids and asks
    for (OrderBookEntry &ask : asks)
    {
    for (OrderBookEntry &bid : bids)
    {
        // order can be placed 
        if (bid.price >= ask.price)
        {   
            // create a new sale
            OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

            // update the order type for our simulated user 
            if (bid.username == "simuser")
            {
                sale.username = "simuser";
                sale.orderType = OrderBookType::bidsale;
            }
            if (ask.username == "simuser")
            {
                sale.username = "simuser";
                sale.orderType = OrderBookType::asksale;
            };

            // if the bid and ask amounts are the same, transfer all over to bidder
            if (bid.amount == ask.amount)

            {
                sale.amount = ask.amount;
                sales.push_back(sale);
                bid.amount = 0;
                break;
            }

            // if the bid amount > ask amount, only take the amount that was offered
            if (bid.amount > ask.amount)
            {
                sale.amount = ask.amount;
                sales.push_back(sale);
                bid.amount = bid.amount - ask.amount;
                break;
            }

            // give the highest bidder the amount they asked for, leave the rest in the ask
            if (bid.amount < ask.amount &&
                bid.amount > 0)
            {
                sale.amount = bid.amount;
                sales.push_back(sale);
                ask.amount = ask.amount - bid.amount;
                bid.amount = 0;
                continue;
            }
        }
    }
    }
    return sales;
}