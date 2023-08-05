#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"


class OrderBook 
{
    public:
    /** contruct reading a csv data file*/
        OrderBook(std::string fileName);
        /** Vector of all know products in dataset*/
        std::vector<std::string> getKnownProducts();
        /** return vector of orders according to sent*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                                std::string product,
                                                std::string timestamp);

        /** Returns the earliest time in the order book*/
        std::string getEarliestTime();

        /** Returns the next time after the sent time 
         * If there is no next it wraps around to the start.
        */
        std::string getNextTime(std::string timeStamp);

        /**Inserting an order into the book*/
        void insertOrder(OrderBookEntry& order);
    
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        

    private:
        std::vector<OrderBookEntry> orders;

};  